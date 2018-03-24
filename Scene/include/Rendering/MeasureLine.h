//
// Created by moe on 19.11.17.
//

#ifndef SCENE_MEASURELINE_H
#define SCENE_MEASURELINE_H

#include "Renderable.h"
#include "MeasureScaleLine.h"
#include "Utility/Observer.h"

namespace moe {
	/**
	 * Measure Line class generates the measure line at the left of the scene and is part of the observer pattern
	 * such that each navigation operationg is executed, it get notified
	 */
    class MeasureLine : public Renderable, public Observer {

    public:

        MeasureLine(Transform2D transform, uint64_t total_timeline,int pixel_line_depth, int scaleLines = 20);

	    virtual ~MeasureLine()
	    {
		    children_.removeAll(&measureLine_);
	    }

        Line& getMeasureLine_();

        void generateScales(qreal yScale, qreal yOffset);

		void updateScales(qreal yScale, qreal yOffset);

		void setPixel_line_depth_(int pixel_line_depth_);

		int getNumOfScaleLines()const;

	protected:

        void draw(SceneData &sceneData, Transform2D &parentTransform) override;

		void initializeRenderable(SceneData &sceneData, Transform2D &parentTransform) override;

	private:
		virtual void onNotify(Event* event) override;
        //void paintGrid();
		int pixel_line_depth_ = 0;
		uint64_t total_timeline_ = 0;
	    Line measureLine_;
        QVector<MeasureScaleLine*> measureLines_ = QVector<MeasureScaleLine*>(0);
        QVector<Line *> gridLines_;
    };

}



#endif //SCENE_MEASURELINE_H
