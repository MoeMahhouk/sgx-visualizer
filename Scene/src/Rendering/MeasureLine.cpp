//
// Created by moe on 19.11.17.
//

#include <Utility/MathUtility.h>
#include "Rendering/TitleText.h"
#include "Rendering/Line.h"
#include "Rendering/MeasureLine.h"

void moe::MeasureLine::draw(SceneData &sceneData, Transform2D &parentTransform) {
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

void moe::MeasureLine::generateScales(qreal yScale, qreal yOffset)
{
    int scaleLines = measureLines_.size();
    qreal i = 0;
    int cntr = 0;
    qreal step = total_timeline_ / (scaleLines-1);
    for (MeasureScaleLine *scaleLine : measureLines_)
    {
        scaleLine->setTransform(Transform2D(1,0,0,1,0,i * ((qreal)pixel_line_depth_/total_timeline_)));
        scaleLine->setXTarget(2);
        scaleLine->setText(checkTimeUnit((yOffset/yScale) + (i/yScale)));
        i += step;
        if(cntr % 5 == 0)
        {
            gridLines_.push_back(new Line(scaleLine->getTransform(),1500,0,1,new QPen(Qt::black,2,Qt::DotLine)));
            measureLine_.children_.push_back(gridLines_.last());
        }
        cntr++;
    }
    gridLines_.push_back(new Line(measureLines_.last()->getTransform(),1500,0,1,new QPen(Qt::black,2,Qt::DotLine)));
    measureLine_.children_.push_back(gridLines_.last());

}

void moe::MeasureLine::updateScales(qreal yScale, qreal yOffset)
{
    int scaleLines = measureLines_.size();
    qreal i = 0;
    qreal step = total_timeline_ / (scaleLines-1);
    for (MeasureScaleLine *scaleLine : measureLines_)
    {
        scaleLine->setText(checkTimeUnit((yOffset/yScale) + (i/yScale)));
        i += step;
    }
}

void moe::MeasureLine::onNotify(Event* event)
{
    if (ZoomEvent *zoom = dynamic_cast<ZoomEvent*>(event))
    {
        updateScales(zoom->yScale_, -zoom->yOffset_);
    } else if (ScrollEvent *scroll = dynamic_cast<ScrollEvent*>(event))
    {
        updateScales(scroll->yScale_, -scroll->yOffset_);
    } else if (ResetEvent *reset = dynamic_cast<ResetEvent*>(event))
    {
        (void)reset;
        updateScales(1,0);
    }
}


void moe::MeasureLine::setPixel_line_depth_(int pixel_line_depth_) {
    MeasureLine::pixel_line_depth_ = pixel_line_depth_;
    measureLine_.setYTarget(pixel_line_depth_);
}

void moe::MeasureLine::initializeRenderable(moe::SceneData &sceneData, moe::Transform2D &parentTransform) {
    /*QPen pen;
    pen.setColor(Qt::black);
    pen.setStyle(Qt::DotLine);
    qreal from = 0;
    qreal to = sceneData.scene->width();
    for (int i = 0; i < std::ceil(getNumOfScaleLines()/5.0) ; ++i)
    {
        from = measureLines_[i*5]->getTransform().getY()+50;
        sceneData.scene->addLine(0, from, to, from,pen);
    }
    from = measureLines_[getNumOfScaleLines()-1]->getTransform().getY()+50;
    sceneData.scene->addLine(0,from,to,from,pen);*/
    return;
}

int moe::MeasureLine::getNumOfScaleLines() const {
    return measureLines_.size();
}

/*void moe::MeasureLine::paintGrid()
{
    QPainter painter;
    QPen pen;
    pen.setColor(Qt::black);
    pen.setStyle(Qt::DotLine);
    painter.setPen(pen);
    for (int i = 0; i < std::ceil(getNumOfScaleLines()/5.0) ; ++i)
    {

    }
}
*/
