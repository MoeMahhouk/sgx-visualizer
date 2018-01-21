//
// Created by moe on 21.11.17.
//

#ifndef SCENE_MEASURESCALELINE_H
#define SCENE_MEASURESCALELINE_H

#include "Renderable.h"
#include "Line.h"
#include "TitleText.h"

namespace moe {

    class MeasureScaleLine : public Renderable {
    public:
        MeasureScaleLine(Transform2D transform = Transform2D(), qreal yPos = 0, qreal xTarget = 0, const QString &title_ = "");

        virtual ~MeasureScaleLine()
        {
            children_.removeAll(&scaleLine_);
            scaleLine_.children_.removeAll(&scaleText_);
        }

        void setXTarget(qreal xTarget);
        void setText(const QString &title);

    protected:
        void draw(Transform2D &parentTransform) override;

    private:
        Line scaleLine_;
    protected:
        void initializeRenderable(SceneData &sceneData, Transform2D &parentTransform) override;

    private:
        TitleText scaleText_;

    };
}




#endif //SCENE_MEASURESCALELINE_H
