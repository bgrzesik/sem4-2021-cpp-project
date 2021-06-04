//
// Created by Bartłomiej Grzesik on 01/06/2021.
//

#include <vector>
#include <opencv2/opencv.hpp>
#include <boost/pool/object_pool.hpp>
#include "PipelineNode.hpp"

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

//class BaseSpotter : public PipelineNode<const cv::Mat *, const SpottedObject *>
template<class Object = SpottedObject>
class BaseSpotter : public PipelineNode<const cv::Mat *, const Object *>
//class BaseSpotter : public PipelineNode<const cv::Mat *, const Object *>
{
public:
    ~BaseSpotter() override = default;

    virtual bool acceptsGrayImage() const
    {
        return false;
    }

    void cleanup() override
    {
        for (const auto &item : spotted_objects) {
            auto *cast_obj = (Object *) item;
            this->object_pool.free(cast_obj);
        }

        spotted_objects.clear();
    }

protected:
    boost::object_pool<Object> object_pool;
    std::vector<const SpottedObject *> spotted_objects;

    void next(const Object *object) override
    {
        PipelineNode<const cv::Mat *, const Object *>::next(object);
        spotted_objects.push_back(object);
    }
};

class QRSpottedObject: public SpottedObject
{
public:
    explicit QRSpottedObject(std::string &&code);

    const std::string &getCode() const;

private:
    std::string code;
};

using Spotter = BaseSpotter<SpottedObject>;

class QRCodeSpotter: public BaseSpotter<QRSpottedObject>
{
public:
    static constexpr int reserveCodeCount = 4;

    QRCodeSpotter();

    bool acceptsGrayImage() const override;

protected:
    void process(const cv::Mat *image) override;

private:

    cv::QRCodeDetector detector;

    std::vector<std::string> codes;
    std::vector<cv::Point> points;

};

class FaceSpotter: public BaseSpotter<SpottedObject>
{
public:
    FaceSpotter();

    bool acceptsGrayImage() const override;

protected:
    void process(const cv::Mat *image) override;


private:
    cv::CascadeClassifier classifier;

};

}

#endif //SPOTTERS_HPP
