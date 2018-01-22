//
// Created by moe on 17.11.17.
//

#include "Rendering/TitleText.h"

void moe::TitleText::draw(SceneData &sceneData, Transform2D &parentTransform) {
   /* sceneData.scene->addSimpleText(title_,QFont("Times", fontSize_,QFont::ExtraBold))->setPos(
            absoluteTransform_.getX() + xOffset_,
            absoluteTransform_.getY() + yOffset_);*/
    text_->setPos(
            absoluteTransform_.getX() + xOffset_,
            absoluteTransform_.getY() + yOffset_);
    text_->setText(title_);

    /*
     * hopless try
     */
    sceneData.scene->addItem(text_);
}

moe::TitleText::TitleText(moe::Transform2D transform, const QString &title_, int fontSize, int xOffset, int yOffset) :
        Renderable(transform), title_(title_), fontSize_(fontSize), xOffset_(xOffset), yOffset_(yOffset)
{
    name = "TitleText";

}

void moe::TitleText::setTitle_(const QString &title) {
    title_ = title;
}

const QString &moe::TitleText::getTitle_() const {
    return title_;
}

void moe::TitleText::initializeRenderable(moe::SceneData &sceneData, moe::Transform2D &parentTransform) {
    text_ = sceneData.scene->addSimpleText(title_,QFont("Times", fontSize_,QFont::ExtraBold));
}
