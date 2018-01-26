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

    rect = sceneData.scene->addRect(
            absoluteTransform_.getX(),
            absoluteTransform_.getY(),
            width_ * absoluteTransform_.xScale(),
            height_ * absoluteTransform_.yScale(),
            *pen_, *brush_);

    /*rect = new QGraphicsRectItem(absoluteTransform_.getX(),
                                 absoluteTransform_.getY(),
                                 width_ * absoluteTransform_.xScale(),
                                 height_ * absoluteTransform_.yScale());
    rect->setBrush(*brush_);
    rect->setPen(*pen_);*/
    /*
     * todo test remove the rect at the start and add it later if its inside the scene
     */
    //rect->hide();
    //sceneData.scene->removeItem(rect);
    //sceneData.scene->addItem(rect);
    sceneData.scene->addItem(this);
    showInScene();
    //this->hide();
    isInScene = true;
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
    if(rect->rect().bottomRight().y() > 0 && rect->rect().topRight().y() < sceneData.scene->height() /*&& rect->rect().height() > 5*/) {
       // std::cerr << " is this rect visible ? " << rect->isVisible() << std::endl;
        if(!isInScene) {
            sceneData.scene->addItem(rect);
            sceneData.scene->addItem(this);
            showInScene();
            isInScene = true;
            //this->show();
            //rect->show();
            //std::cerr << "rect should now be visible " << rect->isVisible() << std::endl;
        }
    } else {
        if (isInScene) {
            hideInScene();
            sceneData.scene->removeItem(rect);
            sceneData.scene->removeItem(this);
            isInScene = false;
            //this->hide();
            //rect->hide();
           // std::cerr << "rect should now be not visible visible " << rect->isVisible() << std::endl;
        }
    }

}

void moe::Rect::hideInScene() {
    rect->hide();
    this->hide();
}

void moe::Rect::showInScene() {
    rect->show();
    this->show();
}






