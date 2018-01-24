//
// Created by moe on 07.01.18.
//

#ifndef SCENE_SEQDIAGBLOCK_H
#define SCENE_SEQDIAGBLOCK_H


#include "DataBase/SGX/CallStats.h"
#include "Rect.h"

namespace moe {


    class SeqDiagBlock : public Rect {

    public:

        SeqDiagBlock(Transform2D transform = Transform2D(), qreal width = 0, qreal height= 0,
                     QPen* pen = new QPen(Qt::red), QBrush* brush = new QBrush(Qt::blue));

        virtual ~SeqDiagBlock();

        void addBlock(Renderable *innerBlock);

        virtual void initializeStats(const CallStats &callsInfos);

        virtual QRectF boundingRect() const override;

    protected:

        virtual void hoverEnterEvent(QGraphicsSceneHoverEvent* event) override;

        virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent* event) override;

        virtual void draw(SceneData &data, Transform2D &parentTransform) override;

        CallStats callsInfos_;

        Renderable *lineOffset_;

        QGraphicsRectItem *mouseOver_;

    private:
    };
}



#endif //SCENE_SEQDIAGBLOCK_H
