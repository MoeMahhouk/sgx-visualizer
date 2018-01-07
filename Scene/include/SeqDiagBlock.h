//
// Created by moe on 07.01.18.
//

#ifndef SCENE_SEQDIAGBLOCK_H
#define SCENE_SEQDIAGBLOCK_H

#include "Rect.h"

namespace moe {

    class SeqDiagBlock : public Rect {

    public:
        SeqDiagBlock(Transform2D transform = Transform2D(), qreal width = 0, qreal height= 0,
                     QPen* pen = new QPen(Qt::red), QBrush* brush = new QBrush(Qt::blue));

        virtual ~SeqDiagBlock();

        void addBlock(Renderable *innerBlock);

    protected:
        void draw(SceneData &sceneData, Transform2D &parentTransform) override;

    private:
        Renderable *lineOffset_;
    };
}



#endif //SCENE_SEQDIAGBLOCK_H
