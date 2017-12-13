//
// Created by moe on 19.11.17.
//

#ifndef SCENE_MEASURELINE_H
#define SCENE_MEASURELINE_H

#include "Renderable.h"
#include "MeasureScaleLine.h"
#include "Observer.h"

namespace moe{

    class MeasureLine : public Renderable, public Observer {
    public:
        MeasureLine(Transform2D transform, qreal lineDepth, int scaleLines = 10);
	    virtual ~MeasureLine()
	    {
		    children_.removeAll(&measureLine_);
	    }

        Line& getMeasureLine_();

        void generateScales(qreal yScale, int yOffset);
        void resetScales();
    protected:
        void draw(SceneData &sceneData, Transform2D &parentTransform) override;

    private:
		QString checkUnit(qreal scaleNumber);
        virtual void onNotify(Event* event) override ;
        Line measureLine_;
        int height_ = 0;
        QVector<MeasureScaleLine*> measureLines_ = QVector<MeasureScaleLine*>(0);
    };

}



#endif //SCENE_MEASURELINE_H
