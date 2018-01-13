//
// Created by moe on 10.01.18.
//

#ifndef SCENE_THREADFILTERSGX_H
#define SCENE_THREADFILTERSGX_H


#include <Filtering/IFilter.h>

namespace moe {

    class ThreadFilter : public IFilter {

    public:

        ThreadFilter(IReciever *reciever, QVector<int> chosenElements);

        void execute() override;

        virtual QString toSQLStatement() override;

    };

}


#endif //SCENE_THREADFILTERSGX_H

