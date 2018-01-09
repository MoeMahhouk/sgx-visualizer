//
// Created by moe on 19.12.17.
//

#ifndef SCENE_SGXDATABASESTRUCTURE_H
#define SCENE_SGXDATABASESTRUCTURE_H

#include <QSqlDriver>
#include <QSqlError>
#include <QtCore/QString>
#include <QVariant>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <iostream>
#include "DataBaseManager.h"
#include "MyThread.h"
#include "OCall.h"
#include "EventMap.h"

namespace moe {

    class SgxDatabaseStructure : public DataBaseManager{

    public:
        SgxDatabaseStructure(const QString& path = "newDataBaseCreated", const QString& type = "QSQLITE");//ToDo maybe add renderable Scene Root node as parameter??
        virtual ~SgxDatabaseStructure() {
            close();
        };

        uint64_t getProgramTotalTime() override;
        void close() override;
    protected:
        int getNumberOfRows(const QString& tableName) override;
        uint64_t getThreadStartTime(int index) override;
        uint64_t getProgramStartTime() override;
        uint64_t getProgramEndTime() override;
    private:
        uint64_t  getRelaTimeOfParent(int parentRowNumber);
        uint64_t  getThreadTotalTime(int index);
        int getEcallsNumberOfThreadAtIndex(int index);
        void initializeECallsOfThreadAtIndex(int index);
        void initializeThreadAtIndex(int index);
        QSqlDatabase m_db;
    };
}



#endif //SCENE_SGXDATABASESTRUCTURE_H
