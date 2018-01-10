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
#include "DataBase/DataBaseManager.h"
#include "MyThread.h"
#include "OCall.h"
#include "EventMap.h"
#include "OCallTypes.h"
#include "ECallTypes.h"

namespace moe {

    class SgxDatabaseStructure : public DataBaseManager{

    public:

        SgxDatabaseStructure(const QString& path = "newDataBaseCreated", const QString& type = "QSQLITE");

        virtual ~SgxDatabaseStructure() {
            close();
        };

        uint64_t getProgramTotalTime();

        const QVector<MyThread> &getThreads_() const;

        void close() override;

        const QVector<OCallTypes> &getOCallTypeList() const;

        const QVector<ECallTypes> &getECallTypeList() const;

    private:

        int getNumberOfRows(const QString& tableName);
        uint64_t getThreadStartTime(int index);
        uint64_t getProgramStartTime();
        uint64_t getProgramEndTime();
        uint64_t  getRelaTimeOfParent(int parentRowNumber);
        uint64_t  getThreadTotalTime(int index);
        int getEcallsNumberOfThreadAtIndex(int index);
        void initializeECallsOfThreadAtIndex(int index);
        void initializeThreadAtIndex(int index);
        void loadECallTypeList();
        void loadOCallTypeList();

        QVector<OCallTypes> oCallTypeList; // this list is not for rendering, it is for the filtering stuff
        QVector<ECallTypes> eCallTypeList; //this list is not for rendering, it is for the filtering stuff
        QVector<MyThread> threads_;
        QSqlDatabase m_db;
    };
}



#endif //SCENE_SGXDATABASESTRUCTURE_H
