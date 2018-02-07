//
// Created by moe on 24.01.18.
//

#ifndef SCENE_SEQDIAGBLOCKCLUSTER_H
#define SCENE_SEQDIAGBLOCKCLUSTER_H

#include "SeqDiagBlock.h"

namespace moe {

    class SeqDiagBlockCluster : public SeqDiagBlock{

    public:

        SeqDiagBlockCluster(Transform2D transform = Transform2D(), qreal width = 0, qreal height= 0,
                            QPen* pen = new QPen(Qt::green), QBrush* brush = new QBrush(Qt::blue));

        virtual ~SeqDiagBlockCluster();

        void addBlock(SeqDiagBlock *innerBlock);

        const int size() const;

        bool checkClusterCriteria(Renderable *innerBlock);


    protected:

        virtual void hoverEnterEvent(QGraphicsSceneHoverEvent* event) override;

        virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent* event) override;

        virtual void draw(SceneData &data, Transform2D &parentTransform) override;

        virtual void drawChildren(SceneData& sceneData) override;

    private:
        bool isClustered;
    };

}


#endif //SCENE_SEQDIAGBLOCKCLUSTER_H
