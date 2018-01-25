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

        //virtual void initializeStats(const CallStats &callsInfos) override ;

        void addBlock(SeqDiagBlock *innerBlock);

        bool checkClusterCriteria(Renderable * innerBlock);

       /* virtual void hideRenderable() override ;

        virtual void showRenderable() override;*/

    protected:

        virtual void hoverEnterEvent(QGraphicsSceneHoverEvent* event) override;

        virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent* event) override;

        //virtual void initializeRenderable(SceneData &sceneData, Transform2D &parentTransform) override;

        virtual void draw(SceneData &data, Transform2D &parentTransform) override;

        virtual void drawChildren(SceneData& sceneData) override;

    private:
        bool isClustered;
    };

}


#endif //SCENE_SEQDIAGBLOCKCLUSTER_H
