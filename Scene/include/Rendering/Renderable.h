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

        Renderable(Transform2D transform = Transform2D())
        {
            relativeTransform_ = transform;
        }
        //does not allow copy constructor
        Renderable ( const Renderable & ) = delete;

        virtual ~Renderable()
        {
	        auto it = children_.begin();
	        while (it != children_.end())
	        {
		        delete *it;
		        it++;
	        }
            this->children_.clear();
        }

        Transform2D& getTransform() {
            return relativeTransform_;
        }

        const Transform2D& getTransform() const{
            return relativeTransform_;
        }

        const Transform2D &getAbsoluteTransform_() const {
            return absoluteTransform_;
        }

        void setTransform(const Transform2D &transform) {
            relativeTransform_ = transform;
        }

        void initialize(SceneData& sceneData, Transform2D &parentTransform)
        {
            absoluteTransform_ =  relativeTransform_ * parentTransform;

            initializeRenderable(sceneData, parentTransform);

            //std::string myIndent = sceneData.indent;
            //sceneData.indent += "\t";

            //std::cout << myIndent << name << " " << this << std::endl;

            for(Renderable* child: children_) {
                // std::cout << myIndent << "child: " << child->name << " " << child << std::endl;
                child->initialize(sceneData, absoluteTransform_);
            }
            //sceneData.indent = myIndent;
        }

        void render(SceneData& sceneData, Transform2D &parentTransform)
        {
            absoluteTransform_ =  relativeTransform_ * parentTransform;

            draw(sceneData, parentTransform);
            //std::cout << myIndent << name << " " << this << std::endl;
            drawChildren(sceneData);
            //std::cout << myIndent << "done" << std::endl;
        }
        virtual void removeFromScene(SceneData &sceneData) {}
        virtual void addToScene(SceneData &sceneData) {}

    protected:

        virtual void initializeRenderable(SceneData& sceneData, Transform2D &parentTransform) = 0;

        virtual void draw(SceneData &sceneData, Transform2D &parentTransform) = 0;
        virtual void drawChildren(SceneData& sceneData) {
            for(Renderable* child: children_) {
                // std::cout << myIndent << "child: " << child->name << " " << child << std::endl;
                child->render(sceneData, absoluteTransform_);
            }
        }

        Transform2D relativeTransform_;
        Transform2D absoluteTransform_;

    };

    class EmptyRenderable : public Renderable {

    protected:

        virtual void initializeRenderable(SceneData &sceneData, Transform2D &parentTransform) {}

        virtual void draw(SceneData &sceneData, Transform2D &parentTransform) {}

    public:

        EmptyRenderable(Transform2D transform = Transform2D()) : Renderable(transform) {
            name = "Empty";
        }

	    virtual ~EmptyRenderable() = default;

    };

}

#endif // RENDERABLE_H
