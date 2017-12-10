#include <QtGui/QVector2D>
#include "Line.h"

moe::Line::Line(Transform2D transform,qreal xTarget,qreal yTarget, qreal penWidth, QPen* pen) :
        Renderable(transform),xTarget_(xTarget) ,yTarget_(yTarget), pen_(pen)
{
    name = "Line";
    //std::cout << "line created" << std::endl;
    pen_->setWidth(penWidth);
}

void moe::Line::draw(SceneData& sceneData, Transform2D &parentTransform)
{
    sceneData.scene->addLine(
            absoluteTransform_.getX(),
            absoluteTransform_.getY(),
            absoluteTransform_.getX() + xTarget_ * absoluteTransform_.xScale(),
            absoluteTransform_.getY() + yTarget_ * absoluteTransform_.yScale(),
                   *pen_);
}

qreal moe::Line::yTarget() const
{
    return yTarget_;
}

void moe::Line::setYTarget(const qreal &height)
{
    yTarget_ = height;
}

void moe::Line::setXTarget_(qreal xTarget) {
    xTarget_ = xTarget;
}
