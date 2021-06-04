//
// Created by Bartłomiej Grzesik on 03/06/2021.
//

#include "FiltersNodes.hpp"

namespace tb
{
    void FilterNode::process(const SpottedObject *input)
    {
        if (this->filter(input)) {
            this->next(input);
        }
    }
}