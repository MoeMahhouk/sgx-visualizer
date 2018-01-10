//
// Created by moe on 10.01.18.
//

#ifndef SCENE_THREADFILTERSGX_H
#define SCENE_THREADFILTERSGX_H


#include <Filtering/Filter.h>

namespace moe {

    class ThreadFilter : public Filter {

    public:

        ThreadFilter();

        virtual ~ThreadFilter();

        virtual QVector<MyThread> execute(const QVector<MyThread> &toFilterList, const QVector<int> &chosenThreads);

    };

}


#endif //SCENE_THREADFILTERSGX_H
