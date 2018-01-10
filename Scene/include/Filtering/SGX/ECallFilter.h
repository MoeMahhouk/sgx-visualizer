//
// Created by moe on 10.01.18.
//

#ifndef SCENE_ECALLFILTER_H
#define SCENE_ECALLFILTER_H


#include <Filtering/Filter.h>

namespace moe {

    class ECallFilter : public Filter {

    public:

        ECallFilter();

        virtual ~ECallFilter();

        virtual QVector<MyThread> execute(const QVector<MyThread> &toFilterList, const QVector<int> &chosenECalls);

    private:

        bool checkChildren(ECall parentEcall, const QVector<int> &chosenECalls);
    };
}


#endif //SCENE_ECALLFILTER_H
