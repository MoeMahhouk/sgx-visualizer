//
// Created by moe on 19.11.17.
//

#include "Rendering/TitleText.h"
#include "Rendering/Line.h"
#include "Rendering/MeasureLine.h"

void moe::MeasureLine::draw(moe::SceneData &sceneData, moe::Transform2D &parentTransform) {
    return;
}

moe::MeasureLine::MeasureLine(Transform2D transform, uint64_t total_timeline, int pixel_line_depth, int scaleLines) :
        Renderable(transform), total_timeline_(total_timeline), pixel_line_depth_(pixel_line_depth),
        measureLine_ (Transform2D(),0 ,pixel_line_depth,2)
{
    name = "MeasureLine";

    measureLines_ = QVector<MeasureScaleLine*>(scaleLines);

    for (int i = 0; i < scaleLines; ++i) {
        measureLines_[i] = new MeasureScaleLine();
        measureLine_.children_.push_back(measureLines_[i]);
    }

    children_.push_back(&measureLine_);

    generateScales(1,0);
    //std::cout << "MeasureLine created with scales" << std::endl;
}

moe::Line & moe::MeasureLine::getMeasureLine_() {
    return measureLine_;
}

void moe::MeasureLine::generateScales(qreal yScale, qreal yOffset) {
    int scaleLines = measureLines_.size();
    qreal i = 0;
    qreal step = total_timeline_ / (scaleLines-1);
    for (MeasureScaleLine *scaleLine : measureLines_) {
        scaleLine->setTransform(Transform2D(1,0,0,1,0,i * ((qreal)pixel_line_depth_/total_timeline_)));
        scaleLine->setXTarget(2);
        scaleLine->setText(checkUnit((yOffset/yScale) + (i/yScale)));
        i += step;
    }

}

void moe::MeasureLine::onNotify(Event* event) {
    if (ZoomEvent *zoom = dynamic_cast<ZoomEvent*>(event)) {
            generateScales(zoom->yScale_, -zoom->yOffset_);
    } else if (ScrollEvent *scroll = dynamic_cast<ScrollEvent*>(event)) {
            generateScales(scroll->yScale_, -scroll->yOffset_);
    } else if (ResetEvent *reset = dynamic_cast<ResetEvent*>(event)) {
        //resetScales();
        generateScales(1,0);

    }
}

QString moe::MeasureLine::checkUnit(qreal scaleNumber) {
    if (scaleNumber == 0) {
        return QString::number(scaleNumber,'f',2);
    }
    if(scaleNumber >= pow(10,9)) {
        return QString::number(scaleNumber / pow(10,9),'f',2) + "s";
    } else if (scaleNumber >= pow(10,6)) {
        return QString::number(scaleNumber / pow(10, 6), 'f', 2) + "ms";
    } else if (scaleNumber >= pow(10,3)) {
        return QString::number(scaleNumber / pow(10,3),'f',2) + "us";
    } else if (scaleNumber >1) {
        return QString::number(scaleNumber,'f',2) + "ns";
    } else if (scaleNumber * pow(10,3) > 1) {
        return QString::number(scaleNumber * pow(10,3),'f',2) + "ps";
    } else if(scaleNumber * pow(10,6) > 1) {
        return QString::number(scaleNumber * pow(10,6),'f',2) + "fs";
    } else if(scaleNumber * pow(10,9) > 1) {
        return QString::number(scaleNumber * pow(10,9),'f',2) + "as";
    } else {
        return QString::number(0,'f',2);
    }
}

void moe::MeasureLine::setPixel_line_depth_(int pixel_line_depth_) {
    MeasureLine::pixel_line_depth_ = pixel_line_depth_;
    measureLine_.setYTarget(pixel_line_depth_);
}

void moe::MeasureLine::initializeRenderable(moe::SceneData &sceneData, moe::Transform2D &parentTransform) {
    return;
}
