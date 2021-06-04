//
// Created by Bartłomiej Grzesik on 03/06/2021.
//

#include <vector>
#include <type_traits>

#ifndef PIPELINENODE_HPP
#define PIPELINENODE_HPP


namespace tb
{

template<typename Input, typename Output>
class ConverterNode;

template<typename Input>
class PipelineInputNode
{
public:
    typedef Input InputType;

    virtual ~PipelineInputNode() = default;

    void addPending(Input element)
    {
        this->pending.push_back(element);
    }

    void processPending()
    {
        for (const auto &item : this->pending) {
            this->process(item);
        }

        this->pending.clear();
    }

    bool hasPending()
    {
        return !this->pending.empty();
    }

    virtual void cleanup()
    {
    }

protected:
    std::vector<Input> pending;

    virtual void process(Input input) = 0;

    template<typename ConverterInput, typename ConverterOutput>
    friend class ConverterNode;
};

template<typename Input, typename Output>
class PipelineNode: public PipelineInputNode<Input>
{
public:
    virtual ~PipelineNode() = default;

    void outputTo(std::shared_ptr<PipelineInputNode<Output>> node)
    {
        auto cast = std::static_pointer_cast<tb::PipelineInputNode<Output>>(node);
        this->adjacent.push_back(std::move(node));
    }

    template<
        typename Node,
        typename NodeInput = typename Node::InputType,
        typename std::enable_if<std::is_base_of<tb::PipelineInputNode<NodeInput>, Node>::value, bool>::type = true,
        typename std::enable_if<std::is_convertible<Output, NodeInput>::value, bool>::type = true
    >
    void outputTo(std::shared_ptr<Node> node)
    {
        this->outputTo(std::make_shared<ConverterNode<Output, NodeInput>>(node));
    }

protected:
    std::vector<std::shared_ptr<PipelineInputNode<Output>>> adjacent;

    virtual void next(Output element)
    {
        for (const auto &item : this->adjacent) {
            item->addPending(element);
        }
    }

    friend class PipelineNode;
};

template<typename Type>
class NopNode: public PipelineNode<Type, Type>
{
protected:
    void process(Type input) override
    {
        this->next(input);
    }
};

template<typename Type>
using BroadcasterNode = NopNode<Type>;

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
