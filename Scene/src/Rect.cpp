#include "Rect.h"


// constructor that sends the coordinations to the super class renderable where it saves them in transform relative variable


moe::Rect::Rect(Transform2D transform, qreal width, qreal height, QPen* pen, QBrush* brush) : Renderable(transform)
{
    name = "Rect";
    width_ = width;
    height_ = height;
    pen_ = pen;
    brush_ = brush;
}




void moe::Rect::draw(SceneData& sceneData, Transform2D &parentTransform)
{
    //add and render the items in the scene according to their relative position with transformation methods

    sceneData.scene->addRect(
            absoluteTransform_.getX(),
            absoluteTransform_.getY(),
            width_ * absoluteTransform_.xScale(),
            height_ * absoluteTransform_.yScale(),
            *pen_, *brush_);
}


qreal moe::Rect::getHeight() const
{
    return height_;
}

void moe::Rect::setHeight(const qreal& height)
{
    height_ = height;
}

qreal moe::Rect::getWidth() const
{
    return width_;
}

void moe::Rect::setWidth(const qreal& width)
{
    width_ = width;
}

