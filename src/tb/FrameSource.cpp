//
// Created by Bartłomiej Grzesik on 09/06/2021.
//

#include "FrameSource.hpp"

namespace tb
{

void FrameSource::process(cv::VideoCapture *capture, tb::PipelineContext *ctx)
{
    (*capture) >> this->frame.rgb;

    cv::cvtColor(this->frame.rgb, frame.hsv, cv::COLOR_BGR2HSV);
    cv::cvtColor(this->frame.rgb, frame.gray, cv::COLOR_BGR2GRAY);

    next(ctx, &this->frame);
}

Frame &FrameSource::getFrame()
{
    return this->frame;
}

}
