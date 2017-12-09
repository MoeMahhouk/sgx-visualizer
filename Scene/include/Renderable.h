#ifndef RENDERABLE_H
#define RENDERABLE_H

#include <iostream>
#include <QtWidgets/QGraphicsScene>
#include <QtWidgets/QGraphicsView>
#include "Transform2D.h"

namespace moe {
    struct SceneData {
        QGraphicsScene* scene;
    };

    class Renderable {

    public:

        QVector<Renderable*> children_;

        Renderable(Transform2D transform = Transform2D()) {
            relativeTransform_ = transform;
        }

        Transform2D& getTransform() {
            return relativeTransform_;
        }

        void setTransform(const Transform2D &transform) {
            relativeTransform_ = transform;
        }


        void render(SceneData& sceneData, Transform2D &parentTransform)
        {
            absoluteTransform_ =  relativeTransform_ * parentTransform;

            draw(sceneData, parentTransform);

            for(Renderable* child: children_)
                child->render(sceneData, absoluteTransform_);
        }
    protected:
        virtual void draw(SceneData& sceneData, Transform2D &parentTransform) = 0;
        Transform2D relativeTransform_;
        Transform2D absoluteTransform_;
    };

    class EmptyRenderable : public Renderable {
    protected:
        virtual void draw(SceneData& sceneData, Transform2D &parentTransform) {}
    public:

        EmptyRenderable(Transform2D transform = Transform2D()) : Renderable(transform) {}

    };

}

#endif // RENDERABLE_H
