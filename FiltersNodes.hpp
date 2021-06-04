//
// Created by Bartłomiej Grzesik on 03/06/2021.
//

#include "PipelineNode.hpp"
#include "Spotters.hpp"

#ifndef TRIGGERFILTERS_HPP
#define TRIGGERFILTERS_HPP



namespace tb
{

class FilterNode: public tb::PipelineNode<const tb::SpottedObject *, const tb::SpottedObject *>
{
public:
    ~FilterNode() override = default;

protected:
    void process(const SpottedObject *input) final;

    virtual bool filter(const SpottedObject *input) = 0;

};

}

#endif //TRIGGERFILTERS_HPP
