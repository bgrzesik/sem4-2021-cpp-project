//
// Created by Bartłomiej Grzesik on 08/06/2021.
//

#include "SingleThreadedContext.hpp"

namespace tb
{

void SingleThreadedContext::start(tb::Job *job)
{
    (*job)();
}

void SingleThreadedContext::pushJob(Job *job)
{
    (*job)();
}

}
