//
// Created by moe on 19.11.17.
//

#include "TitleText.h"
#include "Line.h"
#include "MeasureLine.h"

void moe::MeasureLine::draw(moe::SceneData &sceneData, moe::Transform2D &parentTransform) {
    //generateScales();
    return;
}

moe::MeasureLine::MeasureLine(Transform2D transform, qreal lineDepth, int scaleLines) : Renderable(transform), height_(lineDepth),
                                                                                        measureLine_ (Transform2D(),0 ,lineDepth,2)
{
    name = "MeasureLine";

    measureLines_ = QVector<MeasureScaleLine*>(scaleLines);

    for (int i = 0; i < scaleLines; ++i) {
        measureLines_[i] = new MeasureScaleLine();
        measureLine_.children_.push_back(measureLines_[i]);
    }

    children_.push_back(&measureLine_);

    generateScales(1,0);
    std::cout << "MeasureLine created with scales" << std::endl;
}

moe::Line & moe::MeasureLine::getMeasureLine_() {
    return measureLine_;
}

void moe::MeasureLine::generateScales(qreal yScale, int yOffset) {
    int scaleLines = measureLines_.size();

    qreal i = 0;
    qreal step = height_ / (scaleLines-1);
    for (MeasureScaleLine *scaleLine : measureLines_) {
        scaleLine->setTransform(Transform2D(1,0,0,1,0,i));
        scaleLine->setXTarget(2);
        scaleLine->setText(QString::number((yOffset/yScale) + (i/yScale)));
        i += step;
    }

}

void moe::MeasureLine::resetScales() {
    measureLine_.getTransform().setYScale(1);
    measureLine_.getTransform().setXScale(1);
}

void moe::MeasureLine::onNotify(Event* event) {
    if (ZoomEvent *zoom = dynamic_cast<ZoomEvent*>(event)) {
        generateScales(zoom->r1->getTransform().yScale(), -zoom->r2->getTransform().getY());

    } else if (ScrollEvent *scroll = dynamic_cast<ScrollEvent*>(event)) {
        generateScales(scroll->r1->getTransform().yScale(), -scroll->r2->getTransform().getY());

    } else if (ResetEvent *reset = dynamic_cast<ResetEvent*>(event)) {
        resetScales();
        generateScales(reset->r1->getTransform().yScale(), -reset->r2->getTransform().getY());

    }
    /*switch (event)
    {
        case RESET:
            resetScales();
            generateScales(renderable1.getTransform().yScale(), -renderable2.getTransform().getY());
            break;
        case ZOOM:
        case SCROLL:
            generateScales(renderable1.getTransform().yScale(), -renderable2.getTransform().getY());
            break;
    }*/
}



