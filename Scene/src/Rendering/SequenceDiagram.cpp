//
// Created by moe on 17.11.17.
//


#include "Rendering/SequenceDiagram.h"



moe::SequenceDiagram::SequenceDiagram(Transform2D transform, QString topBlockLabel,int sequenceLineDepth) :
        Renderable(transform), topBlock_(Transform2D(), 50, 20, topBlockLabel)
{
    name = "SequenceDiagram";
    this->children_.push_back(&topBlock_);
    offsetForLine_ = new moe::EmptyRenderable(Transform2D(1, 0, 0, 1,0,topBlock_.getHeight()));/* topBlock_.getWidth() / 2 - 1,
                                                                     topBlock_.getHeight()));*/
    topBlock_.children_.push_back(offsetForLine_);
    sequenceLine_ = new moe::Line(Transform2D(1,0,0,1,topBlock_.getWidth() / 2 - 1,0),0,sequenceLineDepth,2);
    offsetForLine_->children_.push_back(sequenceLine_);
    //std::cout << "sequence diagram created" << std::endl;
}

moe::Renderable *moe::SequenceDiagram::getSequenceLine_() const {
    return sequenceLine_;
}

qreal moe::SequenceDiagram::getLineScale() {
    return sequenceLine_->getTransform().yScale();
}

void moe::SequenceDiagram::setLineScale(qreal scale) {
    //sequenceLine_->getTransform().scale(1,scale);
    offsetForLine_->getTransform().scale(1,scale);
}

void moe::SequenceDiagram::draw(moe::SceneData &sceneData, moe::Transform2D &parentTransform) {
    return;
}


moe::Renderable *moe::SequenceDiagram::getOffsetForLine_() const {
    return offsetForLine_;
}

const moe::LabeledBlock &moe::SequenceDiagram::getTopBlock_() const {
    return topBlock_;
}

void moe::SequenceDiagram::resetLineScales() {
    /*sequenceLine_->getTransform().setYScale(1);
    sequenceLine_->getTransform().setXScale(1);*/
    offsetForLine_->getTransform().setXScale(1);
    offsetForLine_->getTransform().setYScale(1);
}

moe::SequenceDiagram::~SequenceDiagram() {
    children_.removeAll(&topBlock_);
}

void moe::SequenceDiagram::addBlock(qreal createdTime, qreal endTime) {
    sequenceLine_->children_.push_back(new moe::SeqDiagBlock(Transform2D(1,0,0,1,-2,createdTime),15,endTime));
}

void moe::SequenceDiagram::addLabeledBlock(qreal createdTime, qreal endTime, QString blockLabel) {
    sequenceLine_->children_.push_back(new moe::LabeledBlock(Transform2D(1,0,0,1,-2,createdTime),15,endTime, blockLabel));
}

void moe::SequenceDiagram::addBlock(moe::Renderable *childBlock) {
    //sequenceLine_->children_.push_back(childBlock);
    offsetForLine_->children_.push_back(childBlock);
}

void moe::SequenceDiagram::addLabeledBlock(moe::Renderable *childBlock) {
    sequenceLine_->children_.push_back(childBlock);
}

void moe::SequenceDiagram::initializeRenderable(moe::SceneData &sceneData, moe::Transform2D &parentTransform) {
    return;
}


