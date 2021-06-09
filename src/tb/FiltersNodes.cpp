//
// Created by Bartłomiej Grzesik on 03/06/2021.
//

#include "FiltersNodes.hpp"

namespace tb
{

RectangleFilter::RectangleFilter(cv::Rect rect, CollisionType type)
    : rect(std::move(rect)), type(type)
{
}

bool RectangleFilter::filter(const SpottedObject *object)
{

    switch (this->type) {
        case CollisionType::CenterInside: {
            return object->getCenter().inside(this->rect);
        }

        case CollisionType::SinglePoint: {
            auto &points = object->getPoints();

            return std::any_of(points.begin(), points.end(),
                               [this](const cv::Point &p) {
                                   return p.inside(this->rect);
                               });
        }

        case CollisionType::BoundingBoxOverlap: {
            auto bbox = cv::boundingRect(object->getPoints());
            return !(this->rect & bbox).empty();
        }

    }
}

AreaFilter::AreaFilter(int min_area, int max_area)
    : min_area(min_area), max_area(max_area)
{
}

bool AreaFilter::filter(const SpottedObject *object)
{
    cv::Rect bbox = cv::boundingRect(object->getPoints());
    int area = bbox.area();
    return this->min_area < area && area < this->max_area;
}

}