//
// Created by moe on 10.01.18.
//

#ifndef SCENE_ECALLFILTER_H
#define SCENE_ECALLFILTER_H


#include <Filtering/IFilter.h>

namespace moe {
    /**
     * Ecall ocall filter class
     */
    class ECallOCallFilter : public IFilter {

    public:
        ECallOCallFilter(IReciever *reciever, QVector<int> chosenECalls, QVector<int> chosenOCalls,
                         QVector<int> chosenEnclaves, QPair<uint64_t ,uint64_t> chosenTimeline) ;

        QString toSQLStatement() override;

        void execute() override;

    private:
        QVector<int> chosenECalls_;
        QVector<int> chosenOCalls_;
        QVector<int> chosenEnclaves_;
        QPair<uint64_t ,uint64_t > chosenTimeline_;
    };

}


#endif //SCENE_ECALLFILTER_H
