//
// Created by moe on 24.01.18.
//

#include "Rendering/SeqDiagBlockCluster.h"

moe::SeqDiagBlockCluster::SeqDiagBlockCluster(moe::Transform2D transform, qreal width, qreal height, QPen *pen,
                                              QBrush *brush) : SeqDiagBlock(transform, width, height, pen, brush)
{
    isClustered = false;
}

/*void moe::SeqDiagBlockCluster::initializeStats(const moe::CallStats &callsInfos) {
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
    //rect->setRect(rect->boundingRect().united(innerBlock->boundingRect()));
    //height_ = rect->boundingRect().height();
    //width_ = rect->boundingRect().width();
    //std::cerr << "clusters new size is " << getTransform().getY() << " and its height is " << getHeight() << std::endl;
    lineOffset_->children_.push_back(innerBlock);

}

bool moe::SeqDiagBlockCluster::checkClusterCriteria(moe::Renderable *innerBlock) {

    if (height_ == 0) {
        return true;
    }
    qreal yOffsetBetweenEndAndStart = innerBlock->getTransform().getY() - (getTransform().getY() + getHeight()); //start of the added block - the end of the existing cluster block

    return yOffsetBetweenEndAndStart <= (getHeight()/lineOffset_->children_.size());

}

void moe::SeqDiagBlockCluster::draw(moe::SceneData &data, moe::Transform2D &parentTransform)
{  //ToDo test it without the if, it might also work because we remove the item later from the scene when it hast height more than 150, so the ifs are redundant here
    if (rect->rect().height() < 250)  //ToDo only for test , if the cluster height is more than 150 pixel, it should draw the children eventually with their clusters
    {
        if(!isClustered)
        {
            //scene()->addItem(this);
            for (Renderable* child : lineOffset_->children_) {
                //static_cast<SeqDiagBlock*>(child)->hideRenderable();
                child->hideRenderable();
            }
            scene()->update();
            scene()->addItem(rect);
            isClustered = true;
            //std::cerr << "reclusted and readded rect to scene " << std::endl;

        }

        SeqDiagBlock::draw(data, parentTransform);

        //std::cerr << "cluster still not big enough rectheight of it is " <<  rect->rect().height() << std::endl;
    } else {
        //i think it causes the problem in the main draw method because there it asks for the bottomleft and we already removed rect from the scene so, segfault appears, this should fix the problem for now
        rect->setRect(absoluteTransform_.getX(),
                      absoluteTransform_.getY(),
                      width_ * absoluteTransform_.xScale(),
                      height_ * absoluteTransform_.yScale());
        if(isClustered) {
            scene()->removeItem(rect);
            for (Renderable* child : lineOffset_->children_) {
                //static_cast<SeqDiagBlock*>(child)->showRenderable();
                child->showRenderable();
            }
            //scene()->removeItem(this);
            scene()->update();
            isClustered = false;
            //std::cerr << "not clustered anymore and removed from scene " << std::endl;
        }
        //prepareGeometryChange();
    }

}

void moe::SeqDiagBlockCluster::drawChildren(moe::SceneData &sceneData) {
    if (/*rect->rect().height() > 150 &&*/ !isClustered)
    {
        //isClustered = false;
        //sceneData.scene->removeItem(rect);
        //std::cerr << "rect pointer must be still here " << rect << std::endl;
        //sceneData.scene->removeItem(this);
        //std::cerr << "cluster is big and children must now be printed instead of the cluster" << std::endl;
        Renderable::drawChildren(sceneData);
    } /*else if (/*!isClustered && rect->rect().height() < 150) {
        std::cerr << " readding the cluster back to the scene " << std::endl;
        isClustered = true;
        sceneData.scene->addItem(rect);
        sceneData.scene->addItem(this);
    } else {
       // Renderable::drawChildren(sceneData);
    }*/
}

void moe::SeqDiagBlockCluster::hoverEnterEvent(QGraphicsSceneHoverEvent *event) {
    //SeqDiagBlock::hoverEnterEvent(event);
    return;
}

void moe::SeqDiagBlockCluster::hoverLeaveEvent(QGraphicsSceneHoverEvent *event) {
    //SeqDiagBlock::hoverLeaveEvent(event);
    return;
}

/*void moe::SeqDiagBlockCluster::initializeRenderable(moe::SceneData &sceneData, moe::Transform2D &parentTransform) {
    sceneData.scene->addItem(rect);
    sceneData.scene->addItem(this);
}*/
