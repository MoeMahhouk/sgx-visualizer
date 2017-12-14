//
// Created by moe on 13.12.17.
//

#ifndef SCENE_DATABASE_H
#define SCENE_DATABASE_H

#include <QtCore/QVector>
#include "MyThread.h"
#include "Ecall.h"
#include "Ocall.h"
#include "EventMap.h"

namespace moe {

    class DataBase {
        QVector<MyThread*> threads;

    };

}



#endif //SCENE_DATABASE_H
