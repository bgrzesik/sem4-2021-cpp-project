//
// Created by Bartłomiej Grzesik on 01/06/2021.
//
#include <exception>
#include <utility>

#include "Spotters.hpp"

namespace tb
{

void SpottedObject::addPoint(cv::Point point)
{
    this->points.emplace_back(std::move(point));
}

cv::Point SpottedObject::getCenter() const
{
    if (this->points.empty()) {
        throw std::length_error("empty object");
    }

    cv::Point center(0, 0);

    for (const auto &item : this->points) {
        center += item;
    }

    return center / (double) this->points.size();
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

bool QRCodeSpotter::acceptsGrayImage() const
{
    return true;
}
void QRCodeSpotter::process(const cv::Mat *image)
{
    this->detector.detectAndDecodeMulti(*image, this->codes, this->points);

    for (int i = 0; i < this->codes.size(); ++i) {
        QRSpottedObject *mem = this->object_pool.malloc();
        auto *object = new (mem) QRSpottedObject(std::move(this->codes[i]));

        for (int j = 0; j < 4; ++j) {
            object->addPoint(this->points[i * 4 + j]);
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

void FaceSpotter::process(const cv::Mat *image)
{
    std::vector<cv::Rect> faces;
    this->classifier.detectMultiScale(*image, faces);

    for (const auto &face : faces) {
        auto object = this->object_pool.construct();

        object->addPoint(face.tl());
        object->addPoint(face.br());

        this->next(object);
    }
}

bool FaceSpotter::acceptsGrayImage() const
{
    return true;
}

}