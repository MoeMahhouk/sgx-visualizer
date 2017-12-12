//
// Created by moe on 22.11.17.
//

#include "LabeledRect.h"

moe::LabeledRect::LabeledRect(Transform2D transform, qreal width, qreal height, QString title, QPen *pen,
                              QBrush *brush)
        : Rect(transform, width, height, pen, brush),
          label_(Transform2D(),title)
{

    name = "LabeledRect";
    this->children_.push_back(&label_);
    std::cout << "labeledRect born "<< this << std::endl;
}

moe::LabeledRect::~LabeledRect() {
    std::cout << "LabeledRect is dying " << this << std::endl;
    children_.removeAll(&label_);
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
