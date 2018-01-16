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

        IFilter(IReciever *dreciever) : dReciever_(dreciever){};

        virtual ~IFilter()
        {
            // delete dReciever_;//ToDO this is causing problems because it deletes the db and produce free error (ask nico for help politly :) )
        };

        virtual QString toSQLStatement() = 0;

        virtual void execute() = 0;

    protected:
        IReciever *dReciever_; //DataBank Reciever
    };
}
#endif //SCENE_FILTER_H
