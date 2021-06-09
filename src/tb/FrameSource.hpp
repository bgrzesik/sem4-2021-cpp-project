//
// Created by Bartłomiej Grzesik on 09/06/2021.
//

#include <opencv2/opencv.hpp>
#include "tb/Spotters.hpp"

#ifndef FRAMESOURCE_HPP
#define FRAMESOURCE_HPP


namespace tb
{

class FrameSource : public PipelineNode<cv::VideoCapture *, const Frame *>
{
public:
    FrameSource() = default;

    Frame &getFrame();

protected:
    void process(cv::VideoCapture *input, PipelineContext *ctx) override;

private:
    Frame frame;

};

}

#endif //FRAMESOURCE_HPP
