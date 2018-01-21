//
// Created by moe on 17.11.17.
//

#ifndef SCENE_BLOCKTITLE_H
#define SCENE_BLOCKTITLE_H

#include <QGraphicsSimpleTextItem>
#include "Renderable.h"

namespace moe {
    class TitleText : public Renderable {

    public:
        TitleText(Transform2D transform = Transform2D(),
                  const QString &title_ = "",
                  int fontSize = 10,
                  int xOffset = 2,
                  int yOffset = -16
                 );

        void setTitle_(const QString &title);
        const QString &getTitle_() const;

    protected:
        virtual void draw(Transform2D &parentTransform) override;

        virtual void initializeRenderable(SceneData &sceneData, Transform2D &parentTransform) override;

    private:
        QString title_;
        int fontSize_, xOffset_, yOffset_;
        QGraphicsSimpleTextItem *text_;
    };

}


#endif //SCENE_BLOCKTITLE_H
