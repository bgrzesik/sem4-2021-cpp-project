//
// Created by Bartłomiej Grzesik on 01/06/2021.
//
#include <exception>
#include <utility>

#include "Spotters.hpp"

namespace tb
{

cv::Point2d SpottedObject::getCenter() const
{
    if (this->points.empty()) {
        throw std::length_error("empty object");
    }

    auto moments = cv::moments(this->points);
    return cv::Point2d(moments.m10 / moments.m00, moments.m01 / moments.m00);
}

std::vector<cv::Point> &SpottedObject::getPoints()
{
    return points;
}
const std::vector<cv::Point> &SpottedObject::getPoints() const
{
    return points;
}

const std::string &QRSpottedObject::getCode() const
{
    return this->code;
}

QRSpottedObject::QRSpottedObject(std::string &&code)
    : code(std::move(code))
{
}

QRCodeSpotter::QRCodeSpotter()
    : detector()
{
    this->codes.reserve(reserveCodeCount);
    this->points.reserve(reserveCodeCount * 4);
}

void QRCodeSpotter::process(const Frame *frame)
{
    this->detector.detectAndDecodeMulti(frame->gray, this->codes, this->points);

    for (int i = 0; i < this->codes.size(); ++i) {
        QRSpottedObject *mem = this->object_pool.malloc();
        auto *object = new(mem) QRSpottedObject(std::move(this->codes[i]));

        for (int j = 0; j < 4; ++j) {
            object->getPoints().emplace_back(this->points[i * 4 + j]);
        }

        this->next(object);
    }

    this->codes.clear();
    this->points.clear();
}

static constexpr char FaceHaarCascadeClassifier[] = "haarcascades/haarcascade_frontalface_default.xml";

FaceSpotter::FaceSpotter()
    : classifier(cv::samples::findFile(FaceHaarCascadeClassifier))
{
}

void FaceSpotter::process(const Frame *frame)
{
    std::vector<cv::Rect> faces;
    this->classifier.detectMultiScale(frame->gray, faces);

    for (const auto &face : faces) {
        auto object = this->object_pool.construct();

        object->getPoints().emplace_back(face.x, face.y);
        object->getPoints().emplace_back(face.x + face.width, face.y);
        object->getPoints().emplace_back(face.x + face.width, face.y + face.width);
        object->getPoints().emplace_back(face.x, face.y + face.width);

        this->next(object);
    }
}

ColorSpotter::ColorSpotter(cv::Scalar hsv_min, cv::Scalar hsv_max, int min_area, cv::Size gauss_kernel, int threshold)
    : hsv_min(std::move(hsv_min)), hsv_max(std::move(hsv_max)), min_area(min_area),
      gauss_kernel(std::move(gauss_kernel)), threshold(threshold)
{
}

void ColorSpotter::process(const Frame *frame)
{
    cv::inRange(frame->hsv, this->hsv_min, this->hsv_max, this->mask);
    cv::GaussianBlur(this->mask, this->mask, this->gauss_kernel, 0);
    cv::threshold(this->mask, this->mask, this->threshold, 255, cv::THRESH_BINARY);

    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;

    cv::findContours(mask,
                     contours,
                     hierarchy,
                     cv::RETR_TREE,
                     cv::CHAIN_APPROX_SIMPLE);

    for (auto &contour : contours) {
        if (cv::contourArea(contour) <= this->min_area) {
            continue;
        }

        auto object = this->object_pool.construct();
        object->getPoints() = std::move(contour);
        this->next(object);
    }
}

}