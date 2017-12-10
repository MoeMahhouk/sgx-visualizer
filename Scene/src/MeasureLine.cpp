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

    /*qreal scale = 1;
    int yoffset = 0;
    */
    qreal i = 0;
    qreal step = height_ / (scaleLines-1);
    for (MeasureScaleLine *scaleLine : measureLines_) {
        scaleLine->setTransform(Transform2D(1,0,0,1,0,i));
        scaleLine->setXTarget(2);
        scaleLine->setText(QString::number((yOffset/yScale) + (i/yScale)));
        i += step;
    }


    /*
    for (int i = 0; i <= measureLine_->yTarget() ; i+=10) {
        Line *line = new Line(Transform2D(1,0,0,1,0,i),2,0,2);
        line->children_.push_back(new moe::TitleText(Transform2D(),QString::number(i),10,4,-10));
        measureLine_->children_.push_back(line);
    }

    int j = 0;
    if(measureLine_->getTransform().yScale() >= 60 && !isMiniScaled) {
        for (Renderable *l : measureLine_->children_) {

            for (int i = 1; i < 10; i++) {
                Line *addedLine = new Line(Transform2D(1, 0, 0, 1, 0, i), 2, 0, 2);
                addedLine->children_.push_back(
                        new moe::TitleText(Transform2D(), QString::number(j + i), 10, 4, -10));
                l->children_.push_back(addedLine);
            }
        }
        isMiniScaled = true;
    }*/
}

void moe::MeasureLine::resetScales() {
    measureLine_.getTransform().setYScale(1);
    measureLine_.getTransform().setXScale(1);
}



