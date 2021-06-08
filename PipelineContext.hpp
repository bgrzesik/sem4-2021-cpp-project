//
// Created by Bartłomiej Grzesik on 03/06/2021.
//
#include <vector>
#include <memory>
#include <set>

#ifndef TRIGGERBOX_HPP
#define TRIGGERBOX_HPP

namespace tb
{

template<typename Input>
class PipelineInputNode;

class PipelineContext;

class Job
{
public:
    virtual ~Job() = default;

    virtual void operator()() = 0;
};

template<typename Input>
class PipelineJob: public Job
{
public:
    PipelineJob(PipelineContext *ctx,
                tb::PipelineInputNode<Input> *node,
                Input input)
        : ctx(ctx), node(node), input(std::move(input))
    {
    }

    void operator()() override
    {
        node->process(input, ctx);
        node->jobs_pool.free(this);
    }

private:
    PipelineContext *ctx;
    tb::PipelineInputNode<Input> *node;
    Input input;

};

class PipelineContext
{
public:
    virtual ~PipelineContext() = default;

    template<typename Node, typename Input>
    void process(Node node, Input input)
    {
        start(node->getJobFor(this, input));
        cleanup();
    }

protected:
    virtual void start(Job *job) = 0;

    virtual void pushJob(Job *job) = 0;

private:
    std::set<PipelineInputNode<void *> *> nodes;

    void cleanup();

    template<typename Input>
    void
    pushJob(std::shared_ptr<tb::PipelineInputNode<Input>> &node, Input input)
    {
        nodes.insert(reinterpret_cast<tb::PipelineInputNode<void*> *>(node.get()));

        if (!node->deferComputation()) {
            node->process(input, this);
        } else {
            Job *job = node->getJobFor(this, input);
            this->pushJob(job);
        }
    }

    template<class NodeInput>
    friend
    class PipelineInputNode;

    template<typename NodeInput, typename NodeOutput>
    friend
    class PipelineNode;
};

}

#endif //TRIGGERBOX_HPP
