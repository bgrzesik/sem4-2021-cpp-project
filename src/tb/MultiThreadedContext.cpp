//
// Created by Bartłomiej Grzesik on 08/06/2021.
//

#include <iostream>
#include <boost/asio/post.hpp>
#include "MultiThreadedContext.hpp"

namespace tb
{

MultiThreadedContext::MultiThreadedContext(size_t thread_count)
    : jobs(0), pool(thread_count)
{
}

void MultiThreadedContext::start(Job *job)
{
    this->jobs = 0;
    pushJob(job);

    {
        std::unique_lock unique_lock(this->mutex);
        while (this->jobs > 0) {
            this->condvar.wait(unique_lock);
        }
    }
}

void MultiThreadedContext::pushJob(Job *job)
{
    {
        std::unique_lock unique_lock(this->mutex);
        this->jobs++;
    }

    auto lambda = [this, job]() {
        (*job)();

        {
            std::unique_lock unique_lock(this->mutex);
            this->jobs--;
            this->condvar.notify_one();
        }
    };

    boost::asio::post(this->pool, lambda);
}

}
