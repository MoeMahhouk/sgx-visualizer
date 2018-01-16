//
// Created by moe on 10.01.18.
//

#ifndef SCENE_ECALLFILTER_H
#define SCENE_ECALLFILTER_H


#include <Filtering/IFilter.h>

namespace moe {

    class ECallFilter : public IFilter {

    public:
        ECallFilter(IReciever *reciever, QVector<int> chosenECalls, QVector<int> chosenOCalls);

        QString toSQLStatement() override;

        void execute() override;

    private:
        QVector<int> chosenECalls_;
        QVector<int> chosenOCalls_;
    };

}


#endif //SCENE_ECALLFILTER_H
