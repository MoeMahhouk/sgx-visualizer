//
// Created by moe on 10.01.18.
//

#ifndef SCENE_THREADFILTERSGX_H
#define SCENE_THREADFILTERSGX_H


#include <Filtering/Filter.h>

namespace moe {

    class ThreadFilterEvent : public Filter {

    public:

        ThreadFilterEvent();

        virtual ~ThreadFilterEvent();

        virtual QVector<MyThread> execute(const QVector<MyThread> &toFilterList, QVector<int> &chosenThreads);

        virtual QString toSQLStatement(QVector<int> &chosenElementIndex) override;

    };

}


#endif //SCENE_THREADFILTERSGX_H

