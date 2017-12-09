//
// Created by moe on 19.11.17.
//

#ifndef SCENE_MEASURELINE_H
#define SCENE_MEASURELINE_H

#include "Renderable.h"
#include "MeasureScaleLine.h"

namespace moe{

    class MeasureLine : public Renderable {
    public:
        MeasureLine(Transform2D transform, qreal lineDepth, int scaleLines = 10);

        Line& getMeasureLine_();

        void generateScales(qreal yScale, int yOffset);
    protected:
        void draw(SceneData &sceneData, Transform2D &parentTransform) override;

    private:
        Line measureLine_;
        int height_ = 0;
        QVector<MeasureScaleLine> measureLines_ = QVector<MeasureScaleLine>(0);
    };

}



#endif //SCENE_MEASURELINE_H
