//
// Created by moe on 17.11.17.
//

#include "SequenceDiagram.h"



moe::SequenceDiagram::SequenceDiagram(Transform2D transform, QString topBlockLabel) : Renderable(transform)
{
    topBlock_ = LabeledRect(Transform2D(), 15, 20, topBlockLabel);
    std::cout << "topblock label is : " << topBlock_.getLabel_().getTitle_().toStdString() << std::endl;
    /*topBlock_.setWidth(15);
    topBlock_.setHeight(20);
    topBlock_.setLabelText(topBlockLabel);*/
    this->children_.push_back(&topBlock_);
    offsetForLine_ = new moe::EmptyRenderable(Transform2D(1, 0, 0, 1, topBlock_.getWidth() / 2 - 1,
                                                                     topBlock_.getHeight()));
    topBlock_.children_.push_back(offsetForLine_);
    sequenceLine_ = new moe::Line(Transform2D(),0,400,2);
    offsetForLine_->children_.push_back(sequenceLine_);
    sequenceLine_->children_.push_back(new moe::LabeledRect(Transform2D(1,0,0,1,-2,25),15,20,"test2"));
    sequenceLine_->children_.push_back(new moe::Rect(Transform2D(1,0,0,1,-2,50),15,20));

    std::cout << "sequence diagram created" << std::endl;
}

moe::Renderable *moe::SequenceDiagram::getSequenceLine_() const {
    return sequenceLine_;
}

void moe::SequenceDiagram::draw(moe::SceneData &sceneData, moe::Transform2D &parentTransform) {
    return;
}


moe::Renderable *moe::SequenceDiagram::getOffsetForLine_() const {
    return offsetForLine_;
}

const moe::LabeledRect &moe::SequenceDiagram::getTopBlock_() const {
    return topBlock_;
}

/*moe::LabeledRect *moe::SequenceDiagram::getTopBlock_() const {
    return topBlock_;
}*/


