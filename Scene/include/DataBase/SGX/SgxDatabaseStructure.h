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
#include <Utility/Observer.h>
#include <Filtering/IReciever.h>
#include "DataBase/DataBaseManager.h"
#include "MyThread.h"
#include "OCall.h"
#include "EventMap.h"
#include "OCallTypes.h"
#include "ECallTypes.h"
#include "CallStatistics.h"
#include "CallStaticAnalysis.h"
#include "CallDynamicAnalysis.h"

namespace moe {

    class SgxDatabaseStructure : public DataBaseManager, public IReciever{

    public:

        SgxDatabaseStructure(const QString& path = "newDataBaseCreated", const QString& type = "QSQLITE");

        virtual ~SgxDatabaseStructure()
        {
            close();
        };

        uint64_t getProgramTotalTime();

        const QVector<MyThread> &getThreads_() const;

        void close() override;

        const QVector<OCallTypes> &getOCallTypeList() const;

        const QVector<ECallTypes> &getECallTypeList() const;

        const QMap<int, QString> &getEnclavesMap() const;

        void SetAction(TYPES::ACTION_LIST action) override;

        void getResult(QString conditionQuery) override;

        uint64_t getProgramStartTime();

        uint64_t getProgramEndTime();

        void loadEcallsStats();

        void loadOcallsStats();

        void loadEcallAnalysis();

        void loadOcallAnalysis();

        void loadECallDynamicAnalysis();

        void loadOCallDynamicAnalysis();

        bool statsGenerated();

        bool staticAnalysisGenerated();

        bool execAndCheckQuery(QSqlQuery query);

        const QVector<CallStatistics> &getEcallStatistics() const;

        const QVector<CallStatistics> &getOcallStatistics() const;

        const QVector<ECallStaticAnalysis> &getEcallStaticAnalysis() const;

        const QVector<OCallStaticAnalysis> &getOcallStaticAnalysis() const;

        const QMap<int, ECallDynamicAnalysis> &getEcallDynamicAnalysis() const;

        const QMap<int, OCallDynamicAnalysis> &getOcallDynamicAnalysis() const;

    private:
        int getNumberOfRows(const QString& tableName);
        uint64_t getThreadStartTime(int index);
        uint64_t  getThreadTotalTime(int index);
        int getEcallsNumberOfThreadAtIndex(int index);
        void initializeECallsAndOCalls(QString conditionQuery = nullptr);
        void initializeThreads(QString conditionQuery = nullptr);
        void loadECallTypeList();
        void loadOCallTypeList();
        void loadExistingEnclaves();
        int searchThreadIndex(int threadId);
        void loadAexTimes(int ecallId, uint64_t ecallStartTime);
        bool isInOCallList(int id);
        QString getInvolvedThreads();

        QMap<int, OCallDynamicAnalysis> ocallDynamicAnalysis; //ToDo rewrite this later to take tupels as key for id and eid
        QMap<int, ECallDynamicAnalysis> ecallDynamicAnalysis; //ToDo rewrite this later to take tupels as key for id and eid
        QMap<int, bool> directPublicEcalls;
        QVector<CallStatistics> ecallStatistics;
        QVector<CallStatistics> ocallStatistics;
        QVector<ECallStaticAnalysis> ecallStaticAnalysis;
        QVector<OCallStaticAnalysis> ocallStaticAnalysis;
        QSet<int> availableEcalls;
        QSet<int> availableOcalls;
        QMap<int, QString> enclavesList; //stores the pair eid and enclave name
        QHash<int, CallHoverInfo> callHoverInfoMap;
        TYPES::ACTION_LIST currentAction;
        QVector<OCallTypes> oCallTypeList; // this list is not for rendering, it is for the filtering stuff
        QVector<ECallTypes> eCallTypeList; //this list is not for rendering, it is for the filtering stuff
        QVector<MyThread> threads_;
        QSqlDatabase m_db;

    };
}



#endif //SCENE_SGXDATABASESTRUCTURE_H
