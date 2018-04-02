//
// Created by moe on 02.04.18.
//

#ifndef SCENE_IDIALOG_H
#define SCENE_IDIALOG_H

#include <QtWidgets/QDialog>

namespace moe {

    class IDialog : public QDialog {

    public:

        IDialog() : QDialog(){}

        IDialog(QWidget *parent = 0, Qt::WindowFlags f = 0) : QDialog(parent, f){}

    protected:

        virtual void generate() = 0;


    };

}

#endif //SCENE_IDIALOG_H
