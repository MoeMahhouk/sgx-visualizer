//
// Created by moe on 13.12.17.
//

#ifndef SCENE_DATABASE_H
#define SCENE_DATABASE_H

#include <QtCore/QVector>
#include <sqlite3.h>
#include "MyThread.h"
#include "EventMap.h"
namespace moe {

    class DataBase  {

    public:
        DataBase(sqlite3* db);
//        static DataBase* Create(sqlite3* db);

    private:
        int setThreadNumbers(int argc, char **argv, char **azColName);
        static int static_thread_numbers_callback(void *data, int argc, char **argv, char **azColName);
        QVector<MyThread> threads_;
        int threadNumbers;
        //sqlite3* db_;
    };

}



#endif //SCENE_DATABASE_H
