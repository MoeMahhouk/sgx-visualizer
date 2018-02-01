//
// Created by moe on 24.01.18.
//

#include "Rendering/SeqDiagBlockCluster.h"

moe::SeqDiagBlockCluster::SeqDiagBlockCluster(moe::Transform2D transform, qreal width, qreal height, QPen *pen,
                                              QBrush *brush) : SeqDiagBlock(transform, width, height, pen, brush)
{
    //isClustered = false;
}

/*void moe::SeqDiagBlockCluster::initializeStats(const moe::CallHoverInfo &callsInfos) {
    SeqDiagBlock::initializeStats(callsInfos);
}*/

moe::SeqDiagBlockCluster::~SeqDiagBlockCluster() {

}

void moe::SeqDiagBlockCluster::addBlock(SeqDiagBlock *innerBlock) {
    if(height_ == 0) {
        Renderable::setTransform(innerBlock->getTransform());
        setHeight(innerBlock->getHeight());
        setWidth(innerBlock->getWidth());
        innerBlock->Renderable::setTransform(Transform2D());

    } else {
        qreal yOffsetBetweenEndAndStart = innerBlock->getTransform().getY() - (getTransform().getY() + getHeight()); //start of the added block - the end of the existing cluster block
        qreal relativeStartTime = innerBlock->getTransform().getY() - getTransform().getY();
        qreal innerBlockHeight = innerBlock->getHeight();
        qreal new_height_diff = yOffsetBetweenEndAndStart + innerBlockHeight;
        innerBlock->Renderable::setTransform(Transform2D(1,0,0,1,0,relativeStartTime));
        setHeight(getHeight() + new_height_diff);
    }
    lineOffset_->children_.push_back(innerBlock);

}

bool moe::SeqDiagBlockCluster::checkClusterCriteria(moe::Renderable *innerBlock) {

    if (height_ == 0) {
        return true;
    }
    //start of the added block - the end of the existing cluster block
    qreal yOffsetBetweenEndAndStart = innerBlock->getTransform().getY() - (getTransform().getY() + getHeight());


    return yOffsetBetweenEndAndStart <= (getHeight()/lineOffset_->children_.size());

}

void moe::SeqDiagBlockCluster::draw(moe::SceneData &data, moe::Transform2D &parentTransform)
{  //ToDo test it without the if, it might also work because we remove the item later from the scene when it hast height more than 150, so the ifs are redundant here
    updateRectTranform();
    if ((rect->rect().height() / (lineOffset_->children_.size()+1)) < 15.0)
    {
        if(!isClustered)
        {

            if(!isInScene)
            {
               // std::cerr << "must reshown because it got reclustered now" << std::endl;
                //data.scene->addItem(rect);
                //data.scene->addItem(this);
                //isInScene = true;
                showInScene(data);
            }
            /*for (Renderable* child : lineOffset_->children_)
            {
                child->removeFromScene(data);
            }*/

            //std::cerr << "children should now vanish and only the cluster appear" << std::endl;

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
            //data.scene->removeItem(rect);
            //data.scene->removeItem(this);
            //isInScene = false;
        }
        if(this->isClustered)
        {
            /*if(isInScene)
            {
                hideInScene();
                data.scene->removeItem(rect);
                data.scene->removeItem(this);
                isInScene = false;
            }*/
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
    //SeqDiagBlock::hoverEnterEvent(event);
    return;
}

void moe::SeqDiagBlockCluster::hoverLeaveEvent(QGraphicsSceneHoverEvent *event) {
    //SeqDiagBlock::hoverLeaveEvent(event);
    return;
}

/*void moe::SeqDiagBlockCluster::hideRenderable() {
    for (Renderable *child : lineOffset_->children_) {
        child->hideRenderable();
    }
}

void moe::SeqDiagBlockCluster::showRenderable() {
    for (Renderable *child : lineOffset_->children_) {
        child->showRenderable();
    }
}*/

/*void moe::SeqDiagBlockCluster::initializeRenderable(moe::SceneData &sceneData, moe::Transform2D &parentTransform) {
    sceneData.scene->addItem(rect);
    sceneData.scene->addItem(this);
}*/
