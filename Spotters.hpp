//
// Created by Bartłomiej Grzesik on 01/06/2021.
//

#include <vector>
#include <opencv2/opencv.hpp>

#ifndef SPOTTERS_HPP
#define SPOTTERS_HPP

namespace tb
{

class SpottedObject
{
public:
    SpottedObject() = default;

    virtual ~SpottedObject() = default;

    void addPoint(cv::Point point);

    cv::Point getCenter() const;

    std::vector<cv::Point> &getPoints();

    const std::vector<cv::Point> &getPoints() const;

private:
    std::vector<cv::Point> points;
};

using SpottedObjectArray = std::vector<std::unique_ptr<SpottedObject>>;

class Spotter
{
public:
    virtual ~Spotter() = default;

    virtual SpottedObjectArray detect(const cv::Mat &image) = 0;

    virtual bool acceptsGrayImage() const;
};

class QRSpottedObject: public SpottedObject
{
public:
    explicit QRSpottedObject(std::string &&code);

    const std::string &getCode() const;

private:
    std::string code;
    friend class QRCodeSpotter;
};

class QRCodeSpotter: public Spotter
{
public:
    QRCodeSpotter();

    SpottedObjectArray detect(const cv::Mat &image) override;

    bool acceptsGrayImage() const override;
private:
    cv::QRCodeDetector detector;

};

class FaceSpotter: public Spotter
{
public:
    FaceSpotter();

    SpottedObjectArray detect(const cv::Mat &image) override;

    bool acceptsGrayImage() const override;

private:
    cv::CascadeClassifier classifier;

};

}

#endif //SPOTTERS_HPP
