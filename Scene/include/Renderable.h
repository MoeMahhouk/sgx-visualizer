#ifndef RENDERABLE_H
#define RENDERABLE_H

#include <iostream>
#include <QtWidgets/QGraphicsScene>
#include <QtWidgets/QGraphicsView>
#include <typeindex>
#include "Transform2D.h"

namespace moe {
    struct SceneData {
        QGraphicsScene* scene;
        std::string indent = "";

        SceneData(QGraphicsScene* scene) : scene(scene) {}
    };

    class Renderable {

    public:
        std::string name = "Renderable";

        QVector<Renderable*> children_;

        Renderable(Transform2D transform = Transform2D()) {
            relativeTransform_ = transform;
        }
        //does not allow copy constructor
        Renderable ( const Renderable & ) = delete;

        virtual ~Renderable() {
            this->children_.clear();
        }

        Transform2D& getTransform() {
            return relativeTransform_;
        }

        const Transform2D& getTransform() const{
            return relativeTransform_;
        }

        void setTransform(const Transform2D &transform) {
            relativeTransform_ = transform;
        }


        void render(SceneData& sceneData, Transform2D &parentTransform)
        {
            absoluteTransform_ =  relativeTransform_ * parentTransform;

            draw(sceneData, parentTransform);

            std::string myIndent = sceneData.indent;
            sceneData.indent += "\t";

            std::cout << myIndent << name << " " << this << std::endl;

            for(Renderable* child: children_) {
                std::cout << myIndent << "child: " << child->name << " " << child << std::endl;
                child->render(sceneData, absoluteTransform_);
            }
            sceneData.indent = myIndent;

            std::cout << myIndent << "done" << std::endl;
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

        EmptyRenderable(Transform2D transform = Transform2D()) : Renderable(transform) {
            name = "Empty";
        }

    };

}

#endif // RENDERABLE_H
