//
// Created by moe on 24.01.18.
//

#include "Rendering/SeqDiagBlockCluster.h"

moe::SeqDiagBlockCluster::SeqDiagBlockCluster(moe::Transform2D transform, qreal width, qreal height, QPen *pen,
                                              QBrush *brush) : SeqDiagBlock(transform, width, height, pen, brush)
{}

moe::SeqDiagBlockCluster::~SeqDiagBlockCluster() {

}

void moe::SeqDiagBlockCluster::addBlock(SeqDiagBlock *innerBlock) {
    auto innerBlockInfo = innerBlock->getCallsInfos_();
    if(height_ == 0) {
        Renderable::setTransform(innerBlock->getTransform());
        setHeight(innerBlock->getHeight());
        setWidth(innerBlock->getWidth());
        innerBlock->Renderable::setTransform(Transform2D());
        callsInfos_.enclaveId = innerBlockInfo.enclaveId;
        callsInfos_.enclaveBinaryName = innerBlockInfo.enclaveBinaryName;
        callsInfos_.callName = "Cluster of E\\OCalls";
    } else {
        qreal yOffsetBetweenEndAndStart = innerBlock->getTransform().getY() - (getTransform().getY() + getHeight()); //start of the added block - the end of the existing cluster block
        qreal relativeStartTime = innerBlock->getTransform().getY() - getTransform().getY();
        qreal innerBlockHeight = innerBlock->getHeight();
        qreal new_height_diff = yOffsetBetweenEndAndStart + innerBlockHeight;
        innerBlock->Renderable::setTransform(Transform2D(1,0,0,1,0,relativeStartTime));
        setHeight(getHeight() + new_height_diff);
    }
    lineOffset_->children_.push_back(innerBlock);
    callsInfos_.childrenCounter += innerBlockInfo.childrenCounter;
    callsInfos_.callTotalTime += innerBlockInfo.callTotalTime;
    callsInfos_.childrenTotalRuntime += innerBlockInfo.childrenTotalRuntime;

}

bool moe::SeqDiagBlockCluster::checkClusterCriteria(moe::Renderable *innerBlock) {

    if (height_ == 0 /*|| lineOffset_->children_.size() < 2*/) {
        return true;
    }
    //start of the added block - the end of the existing cluster block
    qreal yOffsetBetweenEndAndStart = innerBlock->getTransform().getY() - (getTransform().getY() + getHeight());


    return yOffsetBetweenEndAndStart <= (getHeight()/lineOffset_->children_.size());

}

void moe::SeqDiagBlockCluster::draw(moe::SceneData &data, moe::Transform2D &parentTransform)
{  //ToDo test it without the if, it might also work because we remove the item later from the scene when it hast height more than 150, so the ifs are redundant here
    hideMouseOverAfterRenderUpdate();
    updateRectTranform();
    if ((rect->rect().height() / (this->size()+1)) < 15.0)
    {
        if(!isClustered)
        {

            if(!isInScene)
            {
                showInScene(data);
            }
            isClustered = true;
        }

        for (Renderable* child : lineOffset_->children_)
        {
            child->removeFromScene(data);
        }
        checkInSceneBorders(data);
    } else {

        if(isInScene)
        {
            hideInScene(data);
        }
        if(this->isClustered)
        {
            isClustered = false;
            for (Renderable* child : lineOffset_->children_)
            {
                child->addToScene(data);
            }
        }
    }
    drawChildren(data);

}

void moe::SeqDiagBlockCluster::drawChildren(moe::SceneData &sceneData) {
    if (!this->isClustered)
    {
        SeqDiagBlock::drawChildren(sceneData);
    }
}

void moe::SeqDiagBlockCluster::hoverEnterEvent(QGraphicsSceneHoverEvent *event) {
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
    callName->setHtml("Name : <b>" + callsInfos_.callName + "</b>");
    elementWidths.push_back(callName->boundingRect().right());

    if(callsInfos_.enclaveId != 0)
    {
        auto enclaveInfo = new QGraphicsTextItem(mouseOver_);
        enclaveInfo->setDefaultTextColor(Qt::black);
        enclaveInfo->setHtml("EnclaveID/Name : <b>" + QString::number(callsInfos_.enclaveId,'f', 0) + " / " + callsInfos_.enclaveBinaryName + "</b>");
        enclaveInfo->setY(callName->boundingRect().bottomLeft().y());
        rowCount++;
        elementWidths.push_back(enclaveInfo->boundingRect().right());
    } /*else {
        enclaveInfo->setDefaultTextColor(Qt::black);
        enclaveInfo->setHtml("EnclaveID/Name : <b> NaN / OCall Cluster </b>");
    }*/

    auto clusterTotalTime = new QGraphicsTextItem(mouseOver_);
    clusterTotalTime->setDefaultTextColor(Qt::black);
    clusterTotalTime->setHtml("Cluster's Total runtime : <b>" + checkTimeUnit(callsInfos_.callTotalTime) + " (" +QString::number(callsInfos_.callTotalTime,'f', 0) + "ns)" + "</b>");
    clusterTotalTime->setY(callName->boundingRect().bottomLeft().y() + (15*rowCount));
    rowCount++;
    elementWidths.push_back(clusterTotalTime->boundingRect().right());


    auto directChildrenNumber = new QGraphicsTextItem(mouseOver_);
    directChildrenNumber->setDefaultTextColor(Qt::black);
    directChildrenNumber->setHtml("Number direct clustered Calls : <b>" + QString::number(lineOffset_->children_.size(),'f', 0) + "</b>");
    directChildrenNumber->setY(clusterTotalTime->boundingRect().bottomLeft().y() + (15*rowCount));
    rowCount++;
    yOffset = clusterTotalTime->boundingRect().bottomLeft().y();
    elementWidths.push_back(directChildrenNumber->boundingRect().right());


    if(callsInfos_.childrenTotalRuntime != 0)
    {
        auto childrenTotalTime = new QGraphicsTextItem(mouseOver_);
        childrenTotalTime->setDefaultTextColor(Qt::black);
        childrenTotalTime->setHtml("Total inner Calls runtime : <b>" + checkTimeUnit(callsInfos_.childrenTotalRuntime) + " (" + QString::number(callsInfos_.childrenTotalRuntime,'f', 0) + "ns)" + "</b>");
        childrenTotalTime->setY(directChildrenNumber->boundingRect().bottomLeft().y() + (15*rowCount));
        rowCount++;
        elementWidths.push_back(childrenTotalTime->boundingRect().right());

        auto innerChildrenNumber = new QGraphicsTextItem(mouseOver_);
        innerChildrenNumber->setDefaultTextColor(Qt::black);
        innerChildrenNumber->setHtml("Number inner clustered Calls : <b>" + QString::number(callsInfos_.childrenCounter,'f', 0) + "</b>");
        innerChildrenNumber->setY(childrenTotalTime->boundingRect().bottomLeft().y() + (15*rowCount));
        rowCount++;
        yOffset = innerChildrenNumber->boundingRect().bottomLeft().y();
        elementWidths.push_back(innerChildrenNumber->boundingRect().right());
    }

    qreal maxWidth = *std::max_element(elementWidths.begin(),elementWidths.end());
    mouseOver_->setRect(-5,-5, maxWidth + 15, yOffset + (15 * ++rowCount));
    //return;
}

void moe::SeqDiagBlockCluster::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    SeqDiagBlock::hoverLeaveEvent(event);
    //return;
}


const int moe::SeqDiagBlockCluster::size() const
{
    return lineOffset_->children_.size();
}


