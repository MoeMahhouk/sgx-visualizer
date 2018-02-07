#include "Rendering/Rect.h"

moe::Rect::Rect(Transform2D transform, qreal width, qreal height, QPen* pen, QBrush* brush) : Renderable(transform)
{
    name = "Rect";
    width_ = width;
    height_ = height;
    pen_ = pen;
    brush_ = brush;
    setAcceptHoverEvents(true);
    setFlags(QGraphicsItem::ItemSendsScenePositionChanges | QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemSendsGeometryChanges);

}

void moe::Rect::draw(SceneData &sceneData, Transform2D &parentTransform)
{
    updateRectTranform();
    checkInSceneBorders(sceneData);
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

void moe::Rect::initializeRenderable(moe::SceneData &sceneData, moe::Transform2D &parentTransform)
{

    rect = new QGraphicsRectItem(absoluteTransform_.getX(),
                                 absoluteTransform_.getY(),
                                 width_ * absoluteTransform_.xScale(),
                                 height_ * absoluteTransform_.yScale());
    rect->setBrush(*brush_);
    rect->setPen(*pen_);
    showInScene(sceneData);
}

QRectF moe::Rect::boundingRect() const
{
    return rect->boundingRect();
}

void moe::Rect::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
}

void moe::Rect::updateRectTranform() {
    rect->setRect(absoluteTransform_.getX(),
                  absoluteTransform_.getY(),
                  width_ * absoluteTransform_.xScale(),
                  height_ * absoluteTransform_.yScale());
    prepareGeometryChange();
}

void moe::Rect::checkInSceneBorders(SceneData &sceneData) {
    if(rect->rect().bottomRight().y() > - 50 && rect->rect().topRight().y() < sceneData.scene->height() + 50 /*&& rect->rect().height() > 5*/) {
        if(!isInScene)
        {
            showInScene(sceneData);
        }
    } else {
        if (isInScene)
        {
            hideInScene(sceneData);
        }
    }
}

void moe::Rect::hideInScene(SceneData &sceneData) {
    rect->hide();
    this->hide();
    sceneData.scene->removeItem(rect);
    sceneData.scene->removeItem(this);
    isInScene = false;
}

void moe::Rect::showInScene(SceneData &sceneData) {
    sceneData.scene->addItem(rect);
    sceneData.scene->addItem(this);
    rect->show();
    this->show();
    isInScene = true;
}






