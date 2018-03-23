#ifndef RENDERABLE_H
#define RENDERABLE_H

#include <iostream>
#include <QtWidgets/QGraphicsScene>
#include <QtWidgets/QGraphicsView>
#include <typeindex>
#include "Transform2D.h"

namespace moe {
    /**
     * data structure contains the graphics view framwork scenes information
     */
    struct SceneData {
        QGraphicsScene* scene;
        std::string indent = "";

        SceneData(QGraphicsScene* scene) : scene(scene) {}
    };

    /**
     * abstract class that represents the scene graph used for the rendering process
     */
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

        /**
         * initialises the rendered object and adds them into the scene
         * it iterates through all children and intitialises them recursively too
         * @param sceneData
         * @param parentTransform
         */
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

        /**
         * updates the transformation matrix of each rendered object relatively to its parent and renders it on the scene
         * @param sceneData
         * @param parentTransform
         */
        void render(SceneData& sceneData, Transform2D &parentTransform)
        {
            absoluteTransform_ =  relativeTransform_ * parentTransform;

            draw(sceneData, parentTransform);
            //std::cout << myIndent << name << " " << this << std::endl;
            drawChildren(sceneData);
            //std::cout << myIndent << "done" << std::endl;
        }

        /**
         * remove the rendered object from the scene without deleting its pointer
         * @param sceneData
         */
        virtual void removeFromScene(SceneData &sceneData)
        {
	        (void)sceneData;
        }

        /**
         * adds a removed rendering object into the scene again
         * @param sceneData
         */
        virtual void addToScene(SceneData &sceneData)
        {
	        (void)sceneData;
        }

    protected:
        /**
         * Initialises the renderable relatively to its parent transformation matrix
         * @param sceneData
         * @param parentTransform
         */
        virtual void initializeRenderable(SceneData& sceneData, Transform2D &parentTransform) = 0;

        /**
         * updates and draws the renderables relatively to their parents transformation matrix
         * @param sceneData
         * @param parentTransform
         */
        virtual void draw(SceneData &sceneData, Transform2D &parentTransform) = 0;

        /**
         * iterates through the children of the renderable and applies the render method on them
         * @param sceneData
         */
        virtual void drawChildren(SceneData& sceneData) {
            for(Renderable* child: children_) {
                // std::cout << myIndent << "child: " << child->name << " " << child << std::endl;
                child->render(sceneData, absoluteTransform_);
            }
        }

        Transform2D relativeTransform_;
        Transform2D absoluteTransform_;

    };
    /**
     * generates empty without shape rendering object that work as place holders
     */
    class EmptyRenderable : public Renderable {

    protected:

        virtual void initializeRenderable(SceneData &sceneData, Transform2D &parentTransform)
        {
            (void)sceneData;
            (void)parentTransform;
        }

        virtual void draw(SceneData &sceneData, Transform2D &parentTransform)
        {
            (void)sceneData;
            (void)parentTransform;
        }

    public:

        EmptyRenderable(Transform2D transform = Transform2D()) : Renderable(transform) {
            name = "Empty";
        }

	    virtual ~EmptyRenderable() = default;

    };

}

#endif // RENDERABLE_H
