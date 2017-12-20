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
        MeasureLine(Transform2D transform, uint64_t total_timeline,int pixel_line_depth, int scaleLines = 20);
	    virtual ~MeasureLine()
	    {
		    children_.removeAll(&measureLine_);
	    }

        Line& getMeasureLine_();

        void generateScales(qreal yScale, qreal yOffset);
        void resetScales();
    protected:
        void draw(SceneData &sceneData, Transform2D &parentTransform) override;

    private:
		QString checkUnit(qreal scaleNumber);
        virtual void onNotify(Event* event) override;
        Line measureLine_;
		int pixel_line_depth_ = 0;
        uint64_t total_timeline_ = 0; //ToDo i changed this from int to uint64_t
        QVector<MeasureScaleLine*> measureLines_ = QVector<MeasureScaleLine*>(0);
    };

}



#endif //SCENE_MEASURELINE_H
