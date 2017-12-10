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

        SequenceDiagram(Transform2D transform, QString topBlockLabel);
        Renderable *getSequenceLine_() const;
        Renderable *getOffsetForLine_() const;
        virtual ~SequenceDiagram();
        const LabeledRect &getTopBlock_() const;
        qreal getLineScale();
        void setLineScale(qreal scale);
        void resetLineScales();
    protected:
        void addBlock(qreal createdTime, qreal endTime);
        void draw(SceneData &sceneData, Transform2D &parentTransform) override;
    private:
        LabeledRect topBlock_;
        Renderable* offsetForLine_;
        Renderable* sequenceLine_;
    };

}

#endif //SCENE_SEQUENCEDIAGRAM_H
