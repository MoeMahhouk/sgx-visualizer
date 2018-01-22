//
// Created by moe on 22.11.17.
//

#include "Rendering/LabeledBlock.h"

moe::LabeledBlock::LabeledBlock(Transform2D transform, qreal width, qreal height, QString title, QPen *pen,
                              QBrush *brush)
        : SeqDiagBlock(transform, width, height, pen, brush), label_(Transform2D(),title)
{
    setAcceptHoverEvents(false);
    name = "LabeledBlock";
    this->children_.push_back(&label_);
    //std::cout << "labeledRect born "<< this << std::endl;
}

moe::LabeledBlock::~LabeledBlock() {
    //std::cout << "LabeledBlock is dying " << this << std::endl;
    children_.removeAll(&label_);
}

const moe::TitleText &moe::LabeledBlock::getLabel_() const {
    return label_;
}

void moe::LabeledBlock::draw(SceneData &data,moe::Transform2D &parentTransform) {
    Rect::draw(data, parentTransform);
}

void moe::LabeledBlock::setLabelText(const QString &labelText) {
    label_.setTitle_(labelText);
}
