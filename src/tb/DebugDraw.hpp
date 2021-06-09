//
// Created by Bartłomiej Grzesik on 09/06/2021.
//

#include "Notifier.hpp"
#include "Spotters.hpp"
#include "FrameSource.hpp"

#ifndef DEBUGDRAW_HPP
#define DEBUGDRAW_HPP

namespace tb
{

class DebugDraw: public Notifier<const SpottedObject *>
{
public:
    explicit DebugDraw(Frame *frame);
    explicit DebugDraw(FrameSource &frame_source);
    explicit DebugDraw(std::shared_ptr<FrameSource> &frame_source);

    void notify(const SpottedObject *object) override;

protected:
    Frame *frame;

};

}

#endif //DEBUGDRAW_HPP
