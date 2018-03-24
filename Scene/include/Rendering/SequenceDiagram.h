//
// Created by moe on 17.11.17.
//

#ifndef SCENE_SEQUENCEDIAGRAM_H
#define SCENE_SEQUENCEDIAGRAM_H

#include "Renderable.h"
#include "SeqDiagBlock.h"
#include "Line.h"
#include "LabeledBlock.h"

namespace moe{
    /**
     * Sequence diagram class that initialises a sequence diagram with a line into the scene
     */
    class SequenceDiagram : public Renderable {
    public:

        SequenceDiagram(Transform2D transform, QString topBlockLabel, qreal sequenceLineDepth);
        Renderable *getSequenceLine_() const;
        Renderable *getOffsetForLine_() const;
        virtual ~SequenceDiagram();
        const LabeledBlock &getTopBlock_() const;
        qreal getLineScale();
        void setLineScale(qreal scale);
        void resetLineScales();
        void addBlock(qreal createdTime, qreal endTime); //ToDo this method was protected moved to public for test issues
        void addLabeledBlock(qreal createdTime, qreal endTime, QString blockLabel); //ToDo this method was protected moved to public for test issues
        void addBlock(Renderable *childBlock);
        void addLabeledBlock(Renderable *childBlock);

    protected:
        void draw(SceneData &sceneData, Transform2D &parentTransform) override;
        void initializeRenderable(SceneData &sceneData, Transform2D &parentTransform) override;

    private:
        LabeledBlock topBlock_;
        Renderable* offsetForLine_;
        Renderable* sequenceLine_;
        //Renderable* blocksOffset_; //ToDo test for adding blocks without offset (use this later as global offset for all children blocks) this allows later the switch to other view options :)
    };

}

#endif //SCENE_SEQUENCEDIAGRAM_H
