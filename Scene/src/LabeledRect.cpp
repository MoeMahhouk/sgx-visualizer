//
// Created by moe on 22.11.17.
//

#include "LabeledRect.h"

moe::LabeledRect::LabeledRect(moe::Transform2D transform, qreal width, qreal height, QString title, QPen *pen,
                              QBrush *brush) : Rect(transform, width, height, pen, brush){

    label_ = TitleText(Transform2D(),title);
    this->children_.push_back(&label_);
    std::cout << "labeledRect created with title : "<< title.toStdString() << std::endl;
}

const moe::TitleText &moe::LabeledRect::getLabel_() const {
    return label_;
}

void moe::LabeledRect::draw(moe::SceneData &sceneData, moe::Transform2D &parentTransform) {
    Rect::draw(sceneData, parentTransform);
}

void moe::LabeledRect::setLabelText(const QString &labelText) {
    label_.setTitle_(labelText);
}
