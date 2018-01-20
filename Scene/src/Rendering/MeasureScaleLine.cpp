//
// Created by moe on 21.11.17.
//

#include "Rendering/MeasureScaleLine.h"

moe::MeasureScaleLine::MeasureScaleLine(Transform2D transform, qreal yPos, qreal xTarget, const QString &title_)
        : Renderable(transform),
          scaleLine_(Transform2D(1, 0, 0, 1, 0, yPos), xTarget, 0, 2),
          scaleText_(Transform2D(), title_, 10, 4, -10)

{
    name = "MeasureScaleLine";
    scaleLine_.children_.push_back(&scaleText_);
    this->children_.push_back(&scaleLine_);
}

void moe::MeasureScaleLine::draw(moe::SceneData &sceneData, moe::Transform2D &parentTransform) {
    return;
}

void moe::MeasureScaleLine::setXTarget(qreal xTarget) {
    scaleLine_.setXTarget_(xTarget);
}

void moe::MeasureScaleLine::setText(const QString &title) {
    scaleText_.setTitle_(title);
}

void moe::MeasureScaleLine::initializeRenderable(moe::SceneData &sceneData, moe::Transform2D &parentTransform) {
    return;
}
