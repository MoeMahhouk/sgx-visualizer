//
// Created by moe on 17.12.17.
//

#ifndef SCENE_DATABASEMANAGER_H
#define SCENE_DATABASEMANAGER_H


#include "MyThread.h"


namespace moe {

    class DataBaseManager {
    public:
        DataBaseManager(const QString& path = "newDataBaseCreated"){}

        virtual ~DataBaseManager() = default;

        const QVector<MyThread> &getThreads_() const {
            return threads_;
        }
        virtual uint64_t getProgramTotalTime() = 0;
    protected:
        virtual void close() = 0;
        virtual uint64_t getThreadStartTime(int index) = 0;
        virtual uint64_t getProgramStartTime() = 0;
        virtual int getNumberOfRows(const QString& tableName) = 0;
        QVector<MyThread> threads_;
    };
}


#endif //SCENE_DATABASEMANAGER_H
