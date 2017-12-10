//
// Created by moe on 22.11.17.
//

#ifndef SCENE_LABELEDRECT_H
#define SCENE_LABELEDRECT_H

#include "Rect.h"

namespace moe {

    class LabeledRect : public Rect {
    public:
        LabeledRect(Transform2D transform = Transform2D(), qreal width=0, qreal height=0, QString title = "",
                    QPen* pen = new QPen(Qt::red), QBrush* brush  = new QBrush(Qt::blue));

        virtual ~LabeledRect();

        const TitleText &getLabel_() const;
        void setLabelText (const QString &labelText);

    protected:
        void draw(SceneData &sceneData, Transform2D &parentTransform) override;
    private:
        TitleText label_;
    };

}




#endif //SCENE_LABELEDRECT_H
