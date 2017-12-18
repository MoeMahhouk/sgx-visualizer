//
// Created by moe on 17.12.17.
//

#ifndef SCENE_DATABASEMANAGER_H
#define SCENE_DATABASEMANAGER_H

#include <QtCore/QString>
#include <QVariant>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QSqlDriver>
#include <QSqlError>
#include "MyThread.h"

namespace moe{

    class DataBaseManager {

    public:
        DataBaseManager(const QString& path);
        void testMethod(int index);
    private:
        int getEcallsNumberOfThreadAtIndex(int index);
        uint64_t getThreadTime(int index);
        uint64_t getProgramStartTime();
        int getNumberOfRows(const QString& tableName);
        void initilizeECallsOfThreadAtIndex(int index);
        void initilizeThreadAtIndex(int index);
        uint64_t getTotalTime();
        void close();
        QSqlDatabase m_db;
        QVector<MyThread> threads_;
    };
}


#endif //SCENE_DATABASEMANAGER_H
