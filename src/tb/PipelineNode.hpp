//
// Created by Bartłomiej Grzesik on 03/06/2021.
//

#include <vector>
#include <queue>
#include <memory>
#include <type_traits>

#include <boost/pool/object_pool.hpp>

#ifndef PIPELINENODE_HPP
#define PIPELINENODE_HPP

#include "PipelineContext.hpp"

namespace tb
{

class PipelineContext;

template<typename Input>
class PipelineInputNode
{
public:
    typedef Input InputType;

    virtual ~PipelineInputNode() = default;

    bool hasPending()
    {
        return !this->pending.empty();
    }

    virtual bool deferComputation()
    {
        return false;
    }

    virtual void cleanup()
    {
    }

    virtual Job *getJobFor(PipelineContext *ctx, Input input)
    {
        return jobs_pool.construct(ctx, this, input);
    }

protected:
    boost::object_pool<PipelineJob<Input>> jobs_pool;

    virtual void process(Input input, PipelineContext *ctx) = 0;

    template<typename ConverterInput, typename ConverterOutput>
    friend class ConverterNode;

    friend class PipelineContext;

    template<class JobInput>
    friend class PipelineJob;
};

template<typename Input, typename Output>
class PipelineNode: public PipelineInputNode<Input>
{
public:
    virtual ~PipelineNode() = default;

    void outputsTo(std::shared_ptr<PipelineInputNode<Output>> node)
    {
        this->adjacent.push_back(node);
    }

    template<
        typename Node,
        typename NodeInput = typename Node::InputType,
        typename std::enable_if<std::is_base_of<tb::PipelineInputNode<NodeInput>, Node>::value, bool>::type = true,
        typename std::enable_if<std::is_convertible<Output, NodeInput>::value, bool>::type = true
    >
    void outputsTo(std::shared_ptr<Node> node)
    {
        auto cast = std::reinterpret_pointer_cast<tb::PipelineInputNode<Output>>(node);
        this->outputsTo(cast);
    }

    template<typename Node>
    std::shared_ptr<Node> &operator|(std::shared_ptr<Node> &node)
    {
        this->outputsTo(node);
        return node;
    }

protected:
    std::vector<std::shared_ptr<PipelineInputNode<Output>>> adjacent;

    virtual void next(PipelineContext *ctx, Output element)
    {
        for (auto &item : this->adjacent) {
            ctx->pushJob(item, element);
        }
    }

};

template<class Left, class Right>
std::shared_ptr<Right> &operator|(std::shared_ptr<Left> &left,
                                  std::shared_ptr<Right> &right)
{
    return left->operator|(right);
}

template<typename Type>
class NopNode: public PipelineNode<Type, Type>
{
protected:
    void process(Type input, PipelineContext *ctx) override
    {
        this->next(ctx, input);
    }
};

template<typename Type>
using BroadcasterNode = NopNode<Type>;

template<typename Type>
std::shared_ptr<BroadcasterNode<Type>> broadcast()
{
    return std::make_shared<BroadcasterNode<Type>>();
}

template<typename Input, typename Output>
class ConverterNode: public PipelineInputNode<Input>
{
    typedef std::shared_ptr<PipelineInputNode<Output>> InnerNodePtr;

public:
    InnerNodePtr node;

    explicit ConverterNode(InnerNodePtr node)
        : node(node) {}

protected:
    void process(Input input) override
    {
        this->node->process(input);
    }
};

}


#endif //PIPELINENODE_HPP
