//
// Created by Bartłomiej Grzesik on 03/06/2021.
//

#include "PipelineContext.hpp"
#include "PipelineNode.hpp"

namespace tb
{

void PipelineContext::cleanup()
{
    for (const auto &item : this->nodes) {
        item->cleanup();
    }
}

}