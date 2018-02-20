//
// Created by moe on 07.01.18.
//

#include <DataBase/SGX/SGXErrorCodes.h>
#include "Rendering/SeqDiagBlock.h"

moe::SeqDiagBlock::SeqDiagBlock(moe::Transform2D transform, qreal width, qreal height, QPen *pen, QBrush *brush) :
        Rect(transform, width, height, pen, brush)
{
    name = "Sequence Diagram Block";
    lineOffset_ = new moe::EmptyRenderable(Transform2D(1, 0, 0, 1, 0/*((width) - 1)*/, 0));//ToDo set the x Coordinate to 0 so the blocks will be rendered as one block
    children_.push_back(lineOffset_);
}

void moe::SeqDiagBlock::draw(SceneData &data, moe::Transform2D &parentTransform)
{
    /*if(mouseOver_){
        mouseOver_->hide();
        data.scene->removeItem(mouseOver_);
        delete mouseOver_;
        mouseOver_ = nullptr;
    }*/
    hideMouseOverAfterRenderUpdate();
    Rect::draw(data, parentTransform);
}

void moe::SeqDiagBlock::addBlock(Renderable *innerBlock)
{
    lineOffset_->children_.push_back(innerBlock);
}

moe::SeqDiagBlock::~SeqDiagBlock()
{
    /*auto it = lineOffset_->children_.begin();
    while (it != lineOffset_->children_.end())
    {
        delete *it;
        it++;
    }
    lineOffset_->children_.clear();*/
    children_.removeAll(lineOffset_);
    delete lineOffset_;
}

void moe::SeqDiagBlock::initializeStats(const CallHoverInfo &callsInfos)
{
    callsInfos_.childrenCounter = callsInfos.childrenCounter;
    callsInfos_.childrenTotalRuntime = callsInfos.childrenTotalRuntime;
    callsInfos_.callName = callsInfos.callName;
    callsInfos_.callTotalTime = callsInfos.callTotalTime;
    callsInfos_.enclaveId = callsInfos.enclaveId;
    callsInfos_.enclaveBinaryName = callsInfos.enclaveBinaryName;
    callsInfos_.status = callsInfos.status;
    callsInfos_.aex = callsInfos.aex;
}

void moe::SeqDiagBlock::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    int rowCount = 0;
    qreal yOffset = 0;
    QList<qreal> elementWidths;
    mouseOver_ = new QGraphicsRectItem(this);
    topLevelItem()->setZValue(10.0);
    setZValue(11);
    mouseOver_->setBrush(Qt::lightGray);
    mouseOver_->setPos(this->boundingRect().right() + 5, event->pos().y());

    auto callName = new QGraphicsTextItem(mouseOver_);
    callName->setDefaultTextColor(Qt::black);
    callName->setHtml("Call Name : <b>" + callsInfos_.callName + "</b>");
    elementWidths.push_back(callName->boundingRect().right());

    if(callsInfos_.enclaveId != 0)
    {
        auto enclaveInfo = new QGraphicsTextItem(mouseOver_);
        enclaveInfo->setDefaultTextColor(Qt::black);
        enclaveInfo->setHtml("EnclaveID/Name : <b>" + QString::number(callsInfos_.enclaveId,'f', 0) + " / " + callsInfos_.enclaveBinaryName + "</b>");
        enclaveInfo->setY(callName->boundingRect().bottomLeft().y());
        rowCount++;
        elementWidths.push_back(enclaveInfo->boundingRect().right());
    }

    auto status = new QGraphicsTextItem(mouseOver_);
    status->setDefaultTextColor(Qt::black);
    QString statusText = callsInfos_.status ? "<font color = \"red\">" + sgxErrorToString(callsInfos_.status) : "<font color = \"green\"> success";
    status->setHtml("Status : <b> " + statusText + "</b>");
    status->setY(callName->boundingRect().bottomLeft().y() + (15*rowCount));
    rowCount++;
    elementWidths.push_back(status->boundingRect().right());


    auto callTotalTime = new QGraphicsTextItem(mouseOver_);
    callTotalTime->setDefaultTextColor(Qt::black);
    callTotalTime->setHtml("Total runtime : <b>" + checkTimeUnit(callsInfos_.callTotalTime) + " (" +QString::number(callsInfos_.callTotalTime,'f', 0) + "ns)" + "</b>");
    callTotalTime->setY(status->boundingRect().bottomLeft().y() + (15*rowCount));
    rowCount++;
    yOffset = status->boundingRect().bottomLeft().y();
    elementWidths.push_back(callTotalTime->boundingRect().right());


    if (callsInfos_.childrenCounter != 0)
    {
        auto callChildsCounter = new QGraphicsTextItem(mouseOver_);
        callChildsCounter->setDefaultTextColor(Qt::black);
        callChildsCounter->setHtml("Number Of inner Calls : <b>" + QString::number(callsInfos_.childrenCounter,'f', 0) + "</b>");
        callChildsCounter->setY(callTotalTime->boundingRect().bottomLeft().y() + (15*rowCount));
        rowCount++;
        elementWidths.push_back(callChildsCounter->boundingRect().right());

        auto childrenTotalTime = new QGraphicsTextItem(mouseOver_);
        childrenTotalTime->setDefaultTextColor(Qt::black);
        childrenTotalTime->setHtml("Total inner Calls runtime : <b>" + checkTimeUnit(callsInfos_.childrenTotalRuntime) + " (" + QString::number(callsInfos_.childrenTotalRuntime,'f', 0) + "ns)" + "</b>");
        childrenTotalTime->setY(callChildsCounter->boundingRect().bottomLeft().y() + (15*rowCount));
        rowCount++;
        yOffset = childrenTotalTime->boundingRect().bottomLeft().y();
        elementWidths.push_back(childrenTotalTime->boundingRect().right());

    }

    if(callsInfos_.aex >= 0)
    {
        auto asyncExit = new QGraphicsTextItem(mouseOver_);
        asyncExit->setDefaultTextColor(Qt::black);
        asyncExit->setHtml("AEX count : <b> " + QString::number(callsInfos_.aex,'f',0) + "</b>");
        asyncExit->setY(yOffset + (15 * rowCount));
        rowCount++;
        yOffset = asyncExit->boundingRect().bottomLeft().y();
    }

    qreal maxWidth = *std::max_element(elementWidths.begin(),elementWidths.end());
    mouseOver_->setRect(-5,-5, maxWidth + 15, yOffset + (15 * ++rowCount));
    //scene()->update();
    /*std::cerr << "we are here boooooooooyz mouse Pos : " << event->pos().y() << " mouse Over coordination is y : " << mouseOver_->pos().y() << "  x:" << mouseOver_->pos().x()<< std::endl;

    std::cerr << "this object coordinations x" << this->boundingRect().x() << std::endl;
    std::cerr << "this object coordinations y" << this->boundingRect().y() << std::endl;*/
}

void moe::SeqDiagBlock::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    if (mouseOver_ == nullptr)
        return;

    mouseOver_->setPos(this->boundingRect().right() + 5, event->pos().y());
}

void moe::SeqDiagBlock::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
   // if(!rect->isVisible()) {
        /*if(mouseOver_) {
            scene()->removeItem(mouseOver_);
            delete mouseOver_;
        }*/
     //   return;
    //}
    topLevelItem()->setZValue(0.0);
    setZValue(0.0);
//    mouseOver_->hide();
    //scene()->removeItem(mouseOver_);
    //delete mouseOver_;
    //mouseOver_  = nullptr; //write after free segv error
    hideMouseOverAfterRenderUpdate();
}

QRectF moe::SeqDiagBlock::boundingRect() const
{
    return Rect::boundingRect();
}

void moe::SeqDiagBlock::removeFromScene(SceneData& sceneData)
{
    //std::cerr << "ahhhhhhh we are getting removed from scene" << std::endl;
    /*if(mouseOver_)
    {
        std::cerr << "hey we are here at removeFromScene and we are removing mouseOver" << std::endl;

        mouseOver_->hide();
        sceneData.scene->removeItem(mouseOver_);
        delete mouseOver_;
        mouseOver_ = nullptr;
    }*/
    if (mouseOver_)
    {
        mouseOver_->hide();
        auto temp = mouseOver_;
        mouseOver_ = nullptr;
        delete temp;
    }
    if(isInScene)
    {
        hideInScene(sceneData);
        //sceneData.scene->removeItem(rect);
        //sceneData.scene->removeItem(this);
        //isInScene = false;
    }
    //rect->hide();
    //hide();
    for (Renderable *child : lineOffset_->children_)
    {
        child->removeFromScene(sceneData);
    }

}

void moe::SeqDiagBlock::addToScene(SceneData& sceneData)
{
    //std::cerr << "yeah we are getting readded to the scene" << std::endl;
    /*if (mouseOver_)
    {
        std::cerr << "hey we are here at addtoscene and we are removing mouseOver" << std::endl;
        mouseOver_->hide();
        sceneData.scene->removeItem(mouseOver_);
        delete mouseOver_;
        mouseOver_ = nullptr;
    }*/
    if (mouseOver_)
    {
        std::cerr << "we are here now and mouseover should be hidden "<< std::endl;
        mouseOver_->hide();
        auto temp = mouseOver_;
        mouseOver_ = nullptr;
        delete temp;
    }
    if(!isInScene)
    {
        //sceneData.scene->addItem(rect);
        //sceneData.scene->addItem(this);
        showInScene(sceneData);
        //isInScene = true;
    }
    //rect->show();
    //show();
    for (Renderable *child : lineOffset_->children_)
    {
        child->addToScene(sceneData);
    }
}

const moe::CallHoverInfo &moe::SeqDiagBlock::getCallsInfos_() const
{
    return callsInfos_;
}

void moe::SeqDiagBlock::hideMouseOverAfterRenderUpdate() {
    if(mouseOver_){
        mouseOver_->hide();
        scene()->removeItem(mouseOver_);
        auto temp = mouseOver_;
        mouseOver_ = nullptr;
        delete temp;
    }
}
