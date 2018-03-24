//
// Created by moe on 21.11.17.
//

#ifndef SCENE_MEASURESCALELINE_H
#define SCENE_MEASURESCALELINE_H

#include "Renderable.h"
#include "Line.h"
#include "TitleText.h"

namespace moe {
    /**
     * Measure Scale Line class generates the mini scale lines placed on the main measure line on the scene
     */
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

        void draw(SceneData &sceneData, Transform2D &parentTransform) override;

        void initializeRenderable(SceneData &sceneData, Transform2D &parentTransform) override;

    private:

        Line scaleLine_;

        TitleText scaleText_;

    };
}




#endif //SCENE_MEASURESCALELINE_H
