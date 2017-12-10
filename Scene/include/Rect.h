#ifndef RECT_H
#define RECT_H


#include "Renderable.h"
#include "TitleText.h"
#include <QGraphicsSimpleTextItem>

namespace moe {
    class Rect :public Renderable {

    public:

        Rect(Transform2D transform = Transform2D(), qreal width = 0, qreal height= 0,
             QPen* pen = new QPen(Qt::red), QBrush* brush = new QBrush(Qt::blue));


        qreal getHeight() const;
        void setHeight(const qreal& height);

        qreal getWidth() const;
        void setWidth(const qreal& width);


    protected:
        virtual void draw(SceneData& sceneData, Transform2D &parentTransform) override;
        qreal width_, height_;
        QPen* pen_;
        QBrush* brush_;

    };
}

#endif // RECT_H
