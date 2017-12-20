//
// Created by moe on 17.11.17.
//

#ifndef SCENE_SEQUENCEDIAGRAM_H
#define SCENE_SEQUENCEDIAGRAM_H

#include "Renderable.h"
#include "Rect.h"
#include "Line.h"
#include "LabeledRect.h"

namespace moe{

    class SequenceDiagram : public Renderable {
    public:

        SequenceDiagram(Transform2D transform, QString topBlockLabel, int sequenceLineDepth);
        Renderable *getSequenceLine_() const;
        Renderable *getOffsetForLine_() const;
        virtual ~SequenceDiagram();
        const LabeledRect &getTopBlock_() const;
        qreal getLineScale();
        void setLineScale(qreal scale);
        void resetLineScales();
        void addBlock(qreal createdTime, qreal endTime); //ToDo this method was protected moved to public for test issues
        void addLabeledBlock(qreal createdTime, qreal endTime, QString blockLabel); //ToDo this method was protected moved to public for test issues
        void addBlock(Renderable *childBlock);
        void addLabeledBlock(Renderable *childBlock);
    protected:
        void draw(SceneData &sceneData, Transform2D &parentTransform) override;
    private:
        LabeledRect topBlock_;
        Renderable* offsetForLine_;
        Renderable* sequenceLine_;
    };

}

#endif //SCENE_SEQUENCEDIAGRAM_H
