#ifndef RECT_H
#define RECT_H


#include "Renderable.h"
#include "TitleText.h"
#include <QGraphicsSimpleTextItem>

namespace moe {
    class Rect :public Renderable, public QGraphicsItem {

    public:

        Rect(Transform2D transform = Transform2D(), qreal width = 0, qreal height= 0,
             QPen* pen = new QPen(Qt::red), QBrush* brush = new QBrush(Qt::blue));

        virtual ~Rect()
        {
            delete pen_;
            delete brush_;
        };


        qreal getHeight() const;
        void setHeight(const qreal& height);

        qreal getWidth() const;
        void setWidth(const qreal& width);

        //void addBlock(Renderable *innerBlock);

        QRectF boundingRect() const override;
        void paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
                   QWidget* widget) override;


    protected:

        virtual void draw(Transform2D &parentTransform) override;
        virtual void initializeRenderable(SceneData &sceneData, Transform2D &parentTransform) override;

        qreal width_, height_;
        // Renderable* offsetForLine_;
        QPen* pen_;
        QBrush* brush_;
        QGraphicsRectItem *rect;

    };
}

#endif // RECT_H
