//
// Created by Bartłomiej Grzesik on 08/06/2021.
//
#include "PipelineNode.hpp"
#include "PipelineContext.hpp"

#ifndef SINGLETHREADEDCONTEXT_HPP
#define SINGLETHREADEDCONTEXT_HPP


namespace tb
{

class SingleThreadedContext : public PipelineContext
{

public:
    SingleThreadedContext() = default;

    void start(Job *job) override;

    void pushJob(Job *job) override;

};


}


#endif //SINGLETHREADEDCONTEXT_HPP
