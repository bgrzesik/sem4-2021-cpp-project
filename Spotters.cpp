//
// Created by Bartłomiej Grzesik on 01/06/2021.
//
#include <exception>

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

bool Spotter::acceptsGrayImage() const
{
    return true;
}

const std::string &QRSpottedObject::getCode() const
{
    return this->code;
}

QRSpottedObject::QRSpottedObject(std::string &&code)
    : code(code)
{
}

QRCodeSpotter::QRCodeSpotter()
    : detector()
{
}

SpottedObjectArray QRCodeSpotter::detect(const cv::Mat &image)
{
    std::vector<std::unique_ptr<SpottedObject>> objects;

    std::vector<std::string> codes;
    std::vector<cv::Point> points;

    this->detector.detectAndDecodeMulti(image, codes, points);

    while (!codes.empty()) {
        auto object = std::make_unique<QRSpottedObject>(std::move(codes.back()));
        codes.pop_back();

        for (int i = 0; i < 4; ++i) {
            object->addPoint(points.back());
            points.pop_back();
        }

        objects.emplace_back(std::move(object));
    }


    return std::move(objects);
}

bool QRCodeSpotter::acceptsGrayImage() const
{
    return true;
}

static constexpr char FaceHaarCascadeClassifier[] = "haarcascades/haarcascade_frontalface_default.xml";

FaceSpotter::FaceSpotter()
    : classifier(cv::samples::findFile(FaceHaarCascadeClassifier))
{
}

SpottedObjectArray FaceSpotter::detect(const cv::Mat &image)
{
    SpottedObjectArray objects;

    std::vector<cv::Rect> faces;
    this->classifier.detectMultiScale(image, faces);

    for (const auto &face : faces) {
        auto object = std::make_unique<SpottedObject>();

        object->addPoint(face.tl());
        object->addPoint(face.br());

        objects.emplace_back(std::move(object));
    }


    return std::move(objects);
}

bool FaceSpotter::acceptsGrayImage() const
{
    return true;
}

}