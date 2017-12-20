#include "Rect.h"

moe::Rect::Rect(Transform2D transform, qreal width, qreal height, QPen* pen, QBrush* brush) : Renderable(transform)
{
    name = "Rect";
    offsetForLine_ = new moe::EmptyRenderable(Transform2D(1, 0, 0, 1, ((width) - 1), 0));
    children_.push_back(offsetForLine_);
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

void moe::Rect::addBlock(moe::Renderable *innerBlock) {
    offsetForLine_->children_.push_back(innerBlock);
}

