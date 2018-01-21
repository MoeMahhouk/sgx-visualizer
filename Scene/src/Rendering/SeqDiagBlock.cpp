//
// Created by moe on 07.01.18.
//

#include "Rendering/SeqDiagBlock.h"
#include <QGraphicsSceneHoverEvent>

moe::SeqDiagBlock::SeqDiagBlock(moe::Transform2D transform, qreal width, qreal height, QPen *pen, QBrush *brush) :
        Rect(transform, width, height, pen, brush)
{
    name = "Sequence Diagram Block";
    lineOffset_ = new moe::EmptyRenderable(Transform2D(1, 0, 0, 1, 0/*((width) - 1)*/, 0));//ToDo set the x Coordinate to 0 so the blocks will be rendered as one block
    children_.push_back(lineOffset_);
}

void moe::SeqDiagBlock::draw(moe::Transform2D &parentTransform)
{
    Rect::draw(parentTransform);
}

void moe::SeqDiagBlock::addBlock(moe::Renderable *innerBlock)
{
    lineOffset_->children_.push_back(innerBlock);
}

moe::SeqDiagBlock::~SeqDiagBlock()
{
    children_.removeAll(lineOffset_);
    delete lineOffset_;
}

void moe::SeqDiagBlock::initializeStats(int childrenCounter, uint64_t childrenTotalRuntime) {
    childrenCounter_ = childrenCounter;
    totalChildrenRuntime_ = childrenTotalRuntime;

}

void moe::SeqDiagBlock::hoverEnterEvent(QGraphicsSceneHoverEvent *event) {

    mouseOver_ = new QGraphicsRectItem(this);
    /*topLevelItem()->setZValue(10.0);
    setZValue(11.0);*/
    mouseOver_->setBrush(Qt::lightGray);
    mouseOver_->setPos(this->boundingRect().right() + 5, event->pos().y());
    auto callChildsCounter = new QGraphicsTextItem(mouseOver_);
    callChildsCounter->setDefaultTextColor(Qt::black);
    callChildsCounter->setHtml("Number Of Calls : <b>" + QString::number(childrenCounter_,'f', 0) + "</b>");

    auto childrenTotalTime = new QGraphicsTextItem(mouseOver_);
    childrenTotalTime->setDefaultTextColor(Qt::black);
    childrenTotalTime->setHtml("Total childrenCall runtime : <b>" + QString::number(totalChildrenRuntime_,'f', 0) + "ns" + "</b>");
    childrenTotalTime->setY(callChildsCounter->boundingRect().bottomLeft().y() + 15);

    mouseOver_->setRect(-5,-5, childrenTotalTime->boundingRect().right()+10, childrenTotalTime->boundingRect().bottomLeft().y()+50);
    std::cerr << "we are here boooooooooyz mouse Pos : " << event->pos().y() << "mouse Over coordination is y:" << mouseOver_->pos().y() << "  x:" << mouseOver_->pos().x()<< std::endl;
    std::cerr << "this object coordinations x" << this->boundingRect().x() << std::endl;
    std::cerr << "this object coordinations y" << this->boundingRect().y() << std::endl;
}

void moe::SeqDiagBlock::hoverLeaveEvent(QGraphicsSceneHoverEvent *event) {
    scene()->removeItem(mouseOver_);
    //setZValue(0.0);
    //topLevelItem()->setZValue(0.0);
    delete mouseOver_;
}

QRectF moe::SeqDiagBlock::boundingRect() const {
    return Rect::boundingRect();
}
