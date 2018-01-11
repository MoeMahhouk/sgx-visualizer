//
// Created by moe on 10.01.18.
//

#ifndef SCENE_FILTER_H
#define SCENE_FILTER_H

#include <QtCore/QVector>
#include <DataBase/SGX/MyThread.h>

namespace moe {

    class Filter {

    public:

        Filter(){};

        virtual ~Filter() = default;

        virtual QVector<MyThread> execute (const QVector<MyThread> &toFilterList,const QVector<int> &chosenElements) = 0;

        virtual QString toSQLStatement() = 0;

    };
}
#endif //SCENE_FILTER_H
