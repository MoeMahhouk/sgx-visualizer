//
// Created by moe on 07.01.18.
//

#include "Rendering/SeqDiagBlock.h"

moe::SeqDiagBlock::SeqDiagBlock(moe::Transform2D transform, qreal width, qreal height, QPen *pen, QBrush *brush) :
        Rect(transform, width, height, pen, brush)
{
    name = "Sequence Diagram Block";
    lineOffset_ = new moe::EmptyRenderable(Transform2D(1, 0, 0, 1, ((width) - 1), 0));
    children_.push_back(lineOffset_);

}

void moe::SeqDiagBlock::draw(moe::SceneData &sceneData, moe::Transform2D &parentTransform)
{
    Rect::draw(sceneData, parentTransform);
}

void moe::SeqDiagBlock::addBlock(moe::Renderable *innerBlock)
{
    lineOffset_->children_.push_back(innerBlock);
}

moe::SeqDiagBlock::~SeqDiagBlock()
{
    children_.removeAll(lineOffset_);
    delete lineOffset_;
}
