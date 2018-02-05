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


    class SeqDiagBlock : public Rect {

    public:

        SeqDiagBlock(Transform2D transform = Transform2D(), qreal width = 0, qreal height= 0,
                     QPen* pen = new QPen(Qt::red), QBrush* brush = new QBrush(Qt::blue));

        virtual ~SeqDiagBlock();

        void addBlock(Renderable *innerBlock);

        virtual void initializeStats(const CallHoverInfo &callsInfos);

        virtual QRectF boundingRect() const override;

        virtual void removeFromScene(SceneData& sceneData) override;

        virtual void addToScene(SceneData& sceneData) override;


        const CallHoverInfo &getCallsInfos_() const;

    protected:

        virtual void hoverEnterEvent(QGraphicsSceneHoverEvent* event) override;

        virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent* event) override;

        virtual void draw(SceneData &data, Transform2D &parentTransform) override;

        void hideMouseOverAfterRenderUpdate();

        CallHoverInfo callsInfos_;

        Renderable *lineOffset_;

        QGraphicsRectItem *mouseOver_ = 0;



    };
}



#endif //SCENE_SEQDIAGBLOCK_H
