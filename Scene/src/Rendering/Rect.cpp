#include "Rendering/Rect.h"

moe::Rect::Rect(Transform2D transform, qreal width, qreal height, QPen* pen, QBrush* brush) : Renderable(transform)
{
    name = "Rect";
    //offsetForLine_ = new moe::EmptyRenderable(Transform2D(1, 0, 0, 1, ((width) - 1), 0));
    //children_.push_back(offsetForLine_);
    width_ = width;
    height_ = height;
    pen_ = pen;
    brush_ = brush;
    setAcceptHoverEvents(true);
    setFlags(QGraphicsItem::ItemSendsScenePositionChanges | QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemSendsGeometryChanges);
}

void moe::Rect::draw(SceneData &sceneData, Transform2D &parentTransform)
{
    //add and render the items in the scene according to their relative position with transformation methods
     /* sceneData.scene->addRect(
            absoluteTransform_.getX(),
            absoluteTransform_.getY(),
            width_ * absoluteTransform_.xScale(),
            height_ * absoluteTransform_.yScale(),
            *pen_, *brush_);*/
    rect->setRect(absoluteTransform_.getX(),
                 absoluteTransform_.getY(),
                 width_ * absoluteTransform_.xScale(),
                 height_ * absoluteTransform_.yScale());
    /*std::cerr << "rect top right y " << rect->rect().topRight().y() << std::endl;
    std::cerr << "rect bottom right y " << rect->rect().bottomRight().y() << std::endl;
    std::cerr << "scene height " << scene()->height() << std::endl;*/
    prepareGeometryChange();
    if(rect->rect().bottomRight().y() > 0 && rect->rect().topRight().y() < scene()->height()) {
        show();
        rect->show();
    } else {
        hide();
        rect->hide();

    }
    /*
     * another hopless test
     */

    //sceneData.scene->addItem(rect);
    //sceneData.scene->addItem(this);

    //std::cerr << "block y cooridnate is at " << boundingRect().y() << std::endl;
    //scene()->addItem(this);
    //std::cerr << "this rect y cooridnate is at " << rect->y() << std::endl;
    //std::cerr << "this rect is visible " << this->isVisible() << std::endl;

    /*
     * ToDo this doesnt solve the clustering problem but might be also a relative solution to it

    if (rect->rect().height() < 5) {
        rect->hide();
    } else {
        rect->show();
    }
    */
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
    rect = sceneData.scene->addRect(
            absoluteTransform_.getX(),
            absoluteTransform_.getY(),
            width_ * absoluteTransform_.xScale(),
            height_ * absoluteTransform_.yScale(),
            *pen_, *brush_);
    sceneData.scene->addItem(this);
}

QRectF moe::Rect::boundingRect() const {

    return rect->boundingRect();
}

void moe::Rect::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
}



