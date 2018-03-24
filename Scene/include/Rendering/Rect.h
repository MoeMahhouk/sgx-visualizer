#ifndef RECT_H
#define RECT_H


#include "Renderable.h"
#include "TitleText.h"
#include <QGraphicsSimpleTextItem>

namespace moe {

    /**
     * draws rectangles in the scene
     */
    class Rect :public Renderable, public QGraphicsItem {

    public:

        Rect(Transform2D transform = Transform2D(), qreal width = 0, qreal height= 0,
             QPen* pen = new QPen(Qt::red), QBrush* brush = new QBrush(Qt::blue));

        virtual ~Rect()
        {
            delete rect;
            delete pen_;
            delete brush_;
        };


        qreal getHeight() const;
        void setHeight(const qreal& height);

        qreal getWidth() const;
        void setWidth(const qreal& width);

        /**
         * returns the hitbox of the rectangle
         * @return
         */
        virtual QRectF boundingRect() const override;
        void paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
                   QWidget* widget) override;

    protected:

        virtual void draw(SceneData &sceneData, Transform2D &parentTransform) override;

        virtual void initializeRenderable(SceneData &sceneData, Transform2D &parentTransform) override;

        /**
         * updates the transformation matrix relatively to the parent transformation matrix
         */
        void updateRectTranform();

        /**
         * checks if the rectangle inside the visible part of the scene or not
         * @param sceneData
         */
        void checkInSceneBorders(SceneData &sceneData);

        /**
         * hides the rendered object in the scene (does not print it)
         * @param sceneData
         */
        void hideInScene(SceneData &sceneData);

        /**
         * redraws the hidden object back into the scene
         * @param sceneData
         */
        void showInScene(SceneData &sceneData);

        qreal width_, height_;
        // Renderable* offsetForLine_;
        QPen* pen_;
        QBrush* brush_;
        QGraphicsRectItem *rect;
        bool isInScene;
    };
}

#endif // RECT_H
