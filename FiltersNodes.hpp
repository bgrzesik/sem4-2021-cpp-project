//
// Created by Bartłomiej Grzesik on 03/06/2021.
//

#include "PipelineNode.hpp"
#include "Spotters.hpp"

#ifndef TRIGGERFILTERS_HPP
#define TRIGGERFILTERS_HPP



namespace tb
{

template<typename Type>
class FilterNode: public tb::PipelineNode<Type, Type>
{
public:
    ~FilterNode() override = default;

protected:
    void process(Type input, PipelineContext *ctx) final {
        if (filter(input)) {
            this->next(ctx, input);
        }
    }

    virtual bool filter(Type input) = 0;
};

enum class CollisionType {
    CenterInside,
    SinglePoint,
    BoundingBoxOverlap,
};

class RectangleFilter : public FilterNode<const SpottedObject *> {

public:
    RectangleFilter(cv::Rect rect, CollisionType type);

protected:
    bool filter(const SpottedObject *object) override;

private:
    cv::Rect rect;
    CollisionType type;
};

}

#endif //TRIGGERFILTERS_HPP
