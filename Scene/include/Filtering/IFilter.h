//
// Created by moe on 10.01.18.
//

#ifndef SCENE_FILTER_H
#define SCENE_FILTER_H

#include <QtCore/QVector>
#include <DataBase/SGX/MyThread.h>
#include "IReciever.h"

namespace moe {

    class IFilter {

    public:

        IFilter(IReciever *dreciever, QVector<int> chosenElements) : dReciever_(dreciever), chosenElements_(chosenElements){};

        virtual ~IFilter() {
            delete dReciever_;
        };

        virtual QString toSQLStatement() = 0;

        virtual void execute() = 0;

    protected:
        QVector<int> chosenElements_;
        IReciever *dReciever_; //DataBank Reciever
    };
}
#endif //SCENE_FILTER_H
