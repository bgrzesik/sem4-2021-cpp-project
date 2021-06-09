//
// Created by Bartłomiej Grzesik on 09/06/2021.
//

#include "DebugDraw.hpp"
#include "Constants.hpp"


namespace tb
{

DebugDraw::DebugDraw(Frame *frame)
    : frame(frame)
{
}

DebugDraw::DebugDraw(FrameSource &frame_source)
    : frame(&frame_source.getFrame())
{
}

DebugDraw::DebugDraw(std::shared_ptr<FrameSource> &frame_source)
    : frame(&frame_source->getFrame())
{
}

void DebugDraw::notify(const tb::SpottedObject *object)
{
    const cv::Rect box(100, 100, 200, 200);

    const auto &obj_points = object->getPoints();
    cv::Rect rect = cv::boundingRect(obj_points);

    auto center = object->getCenter();

    if (center.inside(box)) {
        cv::circle(frame->rgb, center, 7, Colors::red, 10);
        cv::rectangle(frame->rgb, rect, Colors::red, 2);
    } else {
        cv::circle(frame->rgb, center, 7, Colors::green, 10);
        cv::rectangle(frame->rgb, rect, Colors::green, 2);
    }

    if (auto *qr_obj = dynamic_cast<const QRSpottedObject *>(object)) {
        cv::putText(frame->rgb, qr_obj->getCode(), center, cv::FONT_HERSHEY_SIMPLEX, 1.0, Colors::magenta, 3);
    }
}

}