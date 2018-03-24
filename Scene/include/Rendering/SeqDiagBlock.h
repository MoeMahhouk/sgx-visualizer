//
// Created by moe on 07.01.18.
//

#ifndef SCENE_SEQDIAGBLOCK_H
#define SCENE_SEQDIAGBLOCK_H

#include <QGraphicsSceneHoverEvent>

#include "DataBase/SGX/CallHoverInfo.h"
#include "Rect.h"
#include "Utility/MathUtility.h"


namespace moe {

    /**
     * extends the rectangle class and adds sequence diagram blocks into the scene
     */
    class SeqDiagBlock : public Rect {

    public:

        SeqDiagBlock(Transform2D transform = Transform2D(), qreal width = 0, qreal height= 0,
                     QPen* pen = new QPen(Qt::red), QBrush* brush = new QBrush(Qt::blue));

        virtual ~SeqDiagBlock();

        /**
         * adds renderable object to the renderable children
         * @param innerBlock
         */
        void addBlock(Renderable *innerBlock);

        /**
         * stores the hover information in case of hovering events
         * @param callsInfos
         */
        virtual void initializeStats(const CallHoverInfo &callsInfos);

        virtual QRectF boundingRect() const override;

        virtual void removeFromScene(SceneData& sceneData) override;

        virtual void addToScene(SceneData& sceneData) override;


        const CallHoverInfo &getCallsInfos_() const;

    protected:
        /**
         * is triggered when hovering over the hitbox sequence diagram block
         * @param event
         */
        virtual void hoverEnterEvent(QGraphicsSceneHoverEvent* event) override;

        /**
         * is triggered when leaving the hitbox sequence diagram block
         * @param event
         */
        virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent* event) override;

        /**
          * is triggered when moving on the hitbox sequence diagram block
          * @param event
          */
        void hoverMoveEvent(QGraphicsSceneHoverEvent *event) override;

        virtual void draw(SceneData &data, Transform2D &parentTransform) override;

        /**
         * removes the hover block info artifacts from the scene
         */
        void hideMouseOverAfterRenderUpdate();

        CallHoverInfo callsInfos_;

        Renderable *lineOffset_;

        QGraphicsRectItem *mouseOver_ = 0;



    };
}



#endif //SCENE_SEQDIAGBLOCK_H
