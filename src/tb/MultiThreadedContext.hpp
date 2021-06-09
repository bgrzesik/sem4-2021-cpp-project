//
// Created by Bartłomiej Grzesik on 08/06/2021.
//
#include <mutex>
#include <condition_variable>
#include <limits>
#include <boost/asio/thread_pool.hpp>

#include "PipelineContext.hpp"
#include "PipelineNode.hpp"

#ifndef MULTITHREADEDCONTEXT_H
#define MULTITHREADEDCONTEXT_H

namespace tb
{

class MultiThreadedContext : public PipelineContext
{
public:
    MultiThreadedContext(size_t thread_count = 4);

    void start(Job *job) override;

    void pushJob(Job *job) override;

private:
    boost::asio::thread_pool pool;

    std::condition_variable condvar;
    std::mutex mutex;
    size_t jobs;

};

}


#endif //MULTITHREADEDCONTEXT_H
