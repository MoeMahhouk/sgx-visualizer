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

    rect->setRect(absoluteTransform_.getX(),
                 absoluteTransform_.getY(),
                 width_ * absoluteTransform_.xScale(),
                 height_ * absoluteTransform_.yScale());
    //std::cerr << "rect absoluteTransform y " << absoluteTransform_.getY() << std::endl;
    //std::cerr << "rect top  y " << rect->rect().topLeft().y() << std::endl;
    //std::cerr << "rect bottom right y " << rect->rect().bottomRight().y() << std::endl;
    //std::cerr << "rect y + height*scale " << rect->rect().y() + height_*absoluteTransform_.yScale() << std::endl;
    //std::cerr << "scene height " << scene()->height() << std::endl;*/
    prepareGeometryChange();
    if(rect->rect().bottomRight().y() > 0 && rect->rect().topRight().y() < scene()->height() /*&& rect->rect().height() > 5*/) {
        std::cerr << "rect is visible " << rect->isVisible() << std::endl;
        if(!rect->isVisible()) {
            sceneData.scene->addItem(rect);
            this->show();
            rect->show();
        }
        std::cerr << "rect should now be visible " << rect->isVisible() << std::endl;
    } else {
        if (rect->isVisible()) {
            sceneData.scene->removeItem(rect);
            this->hide();
            rect->hide();
        }
        std::cerr << "rect should now be not visible visible " << rect->isVisible() << std::endl;
    }


/*
    std::cerr << " rect height with get height  " << getHeight() << std::endl;
    std::cerr << " rect height manually  " << rect->rect().height() << std::endl;
    std::cerr << " rect height with multiplication  " << height_ * absoluteTransform_.yScale() << std::endl;
    /*
     * another hopless test
     */

    //sceneData.scene->addItem(rect);
    //sceneData.scene->addItem(this);

    //std::cerr << "block y cooridnate is at " << boundingRect().y() << std::endl;
    //scene()->addItem(this);
    //std::cerr << "this rect y cooridnate is at " << rect->y() << std::endl;
    //std::cerr << "this rect is visible " << this->isVisible() << std::endl;

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
    /*
     * todo test remove the rect at the start and add it later if its inside the scene
     */
    rect->hide();
    sceneData.scene->removeItem(rect);
    sceneData.scene->addItem(this);
    this->hide();
}

QRectF moe::Rect::boundingRect() const
{
    return rect->boundingRect();
}

void moe::Rect::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
}






