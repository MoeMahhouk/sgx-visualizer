#ifndef LINE_H
#define LINE_H

#include "Renderable.h"
#include <QGraphicsLineItem>

namespace moe {

    class Line : public Renderable {

    public :
        Line(Transform2D transform  = Transform2D(), qreal xTarget = 0, qreal yTarget = 0, qreal penWidth = 4, QPen* pen = new QPen(Qt::black));
        //Line():Line(Transform2D()){}

        virtual ~Line()
        {
            delete pen_;
            //delete line_; //ToDo QGraphicsLineItem has no constructor
        }

        void removeFromScene(SceneData &sceneData) override;

        void addToScene(SceneData &sceneData) override;


        qreal yTarget() const;
        void setYTarget(const qreal &lineDepth);

        void setXTarget_(qreal xTarget);

    protected:
        virtual void draw(SceneData &sceneData, Transform2D &parentTransform) override;

        void initializeRenderable(SceneData &sceneData, Transform2D &parentTransform) override;

    private:
        qreal xTarget_;
        qreal yTarget_;
        QPen* pen_;
        QGraphicsLineItem *line_;
    };

}

#endif // LINE_H
