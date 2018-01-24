//
// Created by moe on 07.01.18.
//

#include <QGraphicsSceneHoverEvent>
#include "Rendering/SeqDiagBlock.h"
#include "Utility/MathUtility.h"

moe::SeqDiagBlock::SeqDiagBlock(moe::Transform2D transform, qreal width, qreal height, QPen *pen, QBrush *brush) :
        Rect(transform, width, height, pen, brush)
{
    name = "Sequence Diagram Block";
    lineOffset_ = new moe::EmptyRenderable(Transform2D(1, 0, 0, 1, 0/*((width) - 1)*/, 0));//ToDo set the x Coordinate to 0 so the blocks will be rendered as one block
    children_.push_back(lineOffset_);
}

void moe::SeqDiagBlock::draw(SceneData &data, moe::Transform2D &parentTransform)
{
    Rect::draw(data, parentTransform);
}

void moe::SeqDiagBlock::addBlock(Renderable *innerBlock)
{
    lineOffset_->children_.push_back(innerBlock);
}

moe::SeqDiagBlock::~SeqDiagBlock()
{
    children_.removeAll(lineOffset_);
    delete lineOffset_;
}

void moe::SeqDiagBlock::initializeStats(const CallStats &callsInfos) {
    callsInfos_.childrenCounter = callsInfos.childrenCounter;
    callsInfos_.childrenTotalRuntime = callsInfos.childrenTotalRuntime;
    callsInfos_.callName = callsInfos.callName;
    callsInfos_.callTotalTime = callsInfos.callTotalTime;
    callsInfos_.enclaveId = callsInfos.enclaveId;
    callsInfos_.enclaveBinaryName = callsInfos.enclaveBinaryName;

}

void moe::SeqDiagBlock::hoverEnterEvent(QGraphicsSceneHoverEvent *event) {

    mouseOver_ = new QGraphicsRectItem(this);
    topLevelItem()->setZValue(10.0);
    setZValue(11);
    mouseOver_->setBrush(Qt::lightGray);
    mouseOver_->setPos(this->boundingRect().right() + 5, event->pos().y());

    auto callName = new QGraphicsTextItem(mouseOver_);
    callName->setDefaultTextColor(Qt::black);
    callName->setHtml("Call Name : <b>" + callsInfos_.callName + "</b>");

    auto enclaveInfo = new QGraphicsTextItem(mouseOver_);
    if(callsInfos_.enclaveId != 0)
    {
        enclaveInfo->setDefaultTextColor(Qt::black);
        enclaveInfo->setHtml("EnclaveID/Name : <b>" + QString::number(callsInfos_.enclaveId,'f', 0) + " / " + callsInfos_.enclaveBinaryName + "</b>");
    } else {
        enclaveInfo->setDefaultTextColor(Qt::black);
        enclaveInfo->setHtml("EnclaveID/Name : <b> NaN / OCall </b>");
    }
    enclaveInfo->setY(callName->boundingRect().bottomLeft().y());

    auto callTotalTime = new QGraphicsTextItem(mouseOver_);
    callTotalTime->setDefaultTextColor(Qt::black);
    callTotalTime->setHtml("Total runtime : <b>" + checkTimeUnit(callsInfos_.callTotalTime) + " (" +QString::number(callsInfos_.callTotalTime,'f', 0) + "ns)" + "</b>");
    callTotalTime->setY(enclaveInfo->boundingRect().bottomLeft().y() + 15);

    auto callChildsCounter = new QGraphicsTextItem(mouseOver_);
    callChildsCounter->setDefaultTextColor(Qt::black);
    callChildsCounter->setHtml("Number Of inner Calls : <b>" + QString::number(callsInfos_.childrenCounter,'f', 0) + "</b>");
    callChildsCounter->setY(callTotalTime->boundingRect().bottomLeft().y() + 30);

    auto childrenTotalTime = new QGraphicsTextItem(mouseOver_);
    childrenTotalTime->setDefaultTextColor(Qt::black);
    childrenTotalTime->setHtml("Total inner Calls runtime : <b>" + checkTimeUnit(callsInfos_.childrenTotalRuntime) + " (" + QString::number(callsInfos_.childrenTotalRuntime,'f', 0) + "ns)" + "</b>");
    childrenTotalTime->setY(callChildsCounter->boundingRect().bottomLeft().y() + 45);

    QList<qreal> elementWidths;
    elementWidths.push_back(callName->boundingRect().right());
    elementWidths.push_back(enclaveInfo->boundingRect().right());
    elementWidths.push_back(callTotalTime->boundingRect().right());
    elementWidths.push_back(callChildsCounter->boundingRect().right());
    elementWidths.push_back(childrenTotalTime->boundingRect().right());
    qreal maxWidth = *std::max_element(elementWidths.begin(),elementWidths.end());
    mouseOver_->setRect(-5,-5, maxWidth + 15, childrenTotalTime->boundingRect().bottomLeft().y() + 75);
    scene()->update();
    /*std::cerr << "we are here boooooooooyz mouse Pos : " << event->pos().y() << " mouse Over coordination is y : " << mouseOver_->pos().y() << "  x:" << mouseOver_->pos().x()<< std::endl;

    std::cerr << "this object coordinations x" << this->boundingRect().x() << std::endl;
    std::cerr << "this object coordinations y" << this->boundingRect().y() << std::endl;*/
}

void moe::SeqDiagBlock::hoverLeaveEvent(QGraphicsSceneHoverEvent *event) {
    scene()->removeItem(mouseOver_);
    mouseOver_->hide();
    setZValue(0.0);
    topLevelItem()->setZValue(0.0);
    delete mouseOver_;
}

QRectF moe::SeqDiagBlock::boundingRect() const {
    return Rect::boundingRect();
}

void moe::SeqDiagBlock::hideRenderable() {
    //std::cerr << "ahhhhhhh we are getting removed from scene" << std::endl;
    //scene()->removeItem(rect);
    rect->hide();
    hide();
    for (Renderable *child : lineOffset_->children_) {
        child->hideRenderable();
    }

}

void moe::SeqDiagBlock::showRenderable() {
    //std::cerr << "year we are getting readded to the scene" << std::endl;
    //scene()->addItem(rect);
    rect->show();
    show();
    for (Renderable *child : lineOffset_->children_) {
        child->showRenderable();
    }
}
