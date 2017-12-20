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
                                                                                        measureLine_ (Transform2D(),0 ,500/*lineDepth*/,2)
//TODO i added 500 only for test purpose, fix later by adding factor parameter or another solution
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

void moe::MeasureLine::generateScales(qreal yScale, qreal yOffset) {
    int scaleLines = measureLines_.size();

    qreal i = 0;
    qreal step = height_ / (scaleLines-1);
    for (MeasureScaleLine *scaleLine : measureLines_) {
        scaleLine->setTransform(Transform2D(1,0,0,1,0,i * (500/height_))); //TODO this is also for test purpose
        scaleLine->setXTarget(2);
        //scaleLine->setText(QString::number((yOffset/yScale) + (i/yScale),'e',2));
        scaleLine->setText(checkUnit((yOffset/yScale) + (i/yScale)));
        i += step;
    }

}

void moe::MeasureLine::resetScales() {
    measureLine_.getTransform().setYScale(1);
    measureLine_.getTransform().setXScale(1);
}

void moe::MeasureLine::onNotify(Event* event) {
    if (ZoomEvent *zoom = dynamic_cast<ZoomEvent*>(event)) {
            //generateScales(zoom->r1->getTransform().yScale(), -zoom->r2->getTransform().getY());
            generateScales(zoom->yScale_, -zoom->yOffset_);
    } else if (ScrollEvent *scroll = dynamic_cast<ScrollEvent*>(event)) {
            //generateScales(scroll->r1->getTransform().yScale(), -scroll->r2->getTransform().getY());
            generateScales(scroll->yScale_, -scroll->yOffset_);
    } else if (ResetEvent *reset = dynamic_cast<ResetEvent*>(event)) {
        resetScales();
        //generateScales(reset->r1->getTransform().yScale(), -reset->r2->getTransform().getY()); //toDo old solution with 2 renderable event variables
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



