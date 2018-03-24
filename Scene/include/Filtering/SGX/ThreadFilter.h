//
// Created by moe on 10.01.18.
//

#ifndef SCENE_THREADFILTERSGX_H
#define SCENE_THREADFILTERSGX_H


#include <Filtering/IFilter.h>

namespace moe {
    /**
     * thread filter class
     */
    class ThreadFilter : public IFilter {

    public:

        ThreadFilter(IReciever *reciever, QVector<int> chosenThreads);

        void execute() override;

        virtual QString toSQLStatement() override;

    private:
        QVector<int> chosenThreads_;

    };

}


#endif //SCENE_THREADFILTERSGX_H

