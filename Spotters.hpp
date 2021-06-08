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

struct Frame
{
    cv::Mat rgb;
    cv::Mat hsv;
    cv::Mat gray;
};

class SpottedObject
{
public:
    SpottedObject() = default;

    virtual ~SpottedObject() = default;

    [[nodiscard]] cv::Point2d getCenter() const;

    std::vector<cv::Point> &getPoints();

    [[nodiscard]] const std::vector<cv::Point> &getPoints() const;

private:
    std::vector<cv::Point> points;
};

//class BaseSpotter : public PipelineNode<const cv::Mat *, const SpottedObject *>
template<class Object = SpottedObject>
class BaseSpotter: public PipelineNode<const Frame *, const Object *>
//class BaseSpotter : public PipelineNode<const cv::Mat *, const Object *>
{
public:
    ~BaseSpotter() override = default;

    void cleanup() override
    {
        for (const auto &item : spotted_objects) {
            auto *cast_obj = (Object *) item;
            this->object_pool.free(cast_obj);
        }

        spotted_objects.clear();
    }

    bool deferComputation() override
    {
        return true;
    }

protected:
    boost::object_pool<Object> object_pool;
    std::vector<const SpottedObject *> spotted_objects;

    void next(PipelineContext *ctx, const Object *object) override
    {
        PipelineNode<const Frame *, const Object *>::next(ctx, object);
        spotted_objects.push_back(object);
    }
};

class QRSpottedObject: public SpottedObject
{
public:
    explicit QRSpottedObject(std::string &&code);

    [[nodiscard]] const std::string &getCode() const;

private:
    std::string code;
};

using Spotter = BaseSpotter<SpottedObject>;

class QRCodeSpotter: public BaseSpotter<QRSpottedObject>
{
public:
    static constexpr int reserveCodeCount = 4;

    QRCodeSpotter();

protected:
    void process(const Frame *frame, PipelineContext *ctx) override;

private:

    cv::QRCodeDetector detector;

    std::vector<std::string> codes;
    std::vector<cv::Point> points;

};

class FaceSpotter: public BaseSpotter<SpottedObject>
{
public:
    FaceSpotter();

protected:
    void process(const Frame *frame, PipelineContext *ctx) override;


private:
    cv::CascadeClassifier classifier;

};

class ColorSpotter: public BaseSpotter<SpottedObject>
{
public:
    ColorSpotter(cv::Scalar hsv_min,
                 cv::Scalar hsv_max,
                 int min_area = 2000,
                 cv::Size gauss_kernel = cv::Size(31, 31),
                 int threshold = 60);

protected:
    void process(const Frame *frame, PipelineContext *ctx) override;

private:
    cv::Mat mask;
    cv::Scalar hsv_min;
    cv::Scalar hsv_max;
    int min_area;
    cv::Size gauss_kernel;
    int threshold;

};

}

#endif //SPOTTERS_HPP
