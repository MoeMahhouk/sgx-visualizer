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
    /**
     * The database class for sgx traces and it implements the IReciever interface for the filter commands (Command pattern)
     */
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

        /**
         * generates the ecall statistics
         */
        void loadEcallsStats();

        /**
         * generates the ocall statistics
         */
        void loadOcallsStats();

        /**
         * generates the static analysis for ecalls
         */
        void loadEcallAnalysis();

        /**
         * generates the static analysis for ocalls
         */
        void loadOcallAnalysis();

        /**
         * generates the dynamic analysis for ecalls
         */
        void loadECallDynamicAnalysis();

        /**
         * generates the dynamic analysis for ocalls
         */
        void loadOCallDynamicAnalysis();

        /**
         * checks if the statistics already generated
         * @return
         */
        bool statsGenerated();

        /**
         * checks if the analysis already generated
         * @return
         */
        bool staticAnalysisGenerated();

        /**
         * executes the given query
         * @param query
         * @return true if execution was successfull or false if it query was invalid
         */
        bool execAndCheckQuery(QSqlQuery query);

        const QVector<CallStatistics> &getEcallStatistics() const;

        const QVector<CallStatistics> &getOcallStatistics() const;

        const QVector<CallStaticAnalysis> &getEcallStaticAnalysis() const;

        const QVector<CallStaticAnalysis> &getOcallStaticAnalysis() const;

        const QMap<int, ECallDynamicAnalysis> &getEcallDynamicAnalysis() const;

        const QMap<int, OCallDynamicAnalysis> &getOcallDynamicAnalysis() const;

    private:
        /**
         * executes a query to find out how many entries in the given table name
         * @param tableName
         * @return how many rows with that table name
         */
        int getNumberOfRows(const QString& tableName);
        uint64_t getThreadStartTime(int index);
        uint64_t  getThreadTotalTime(int index);
        int getEcallsNumberOfThreadAtIndex(int index);

        /**
         * executes the recursive query to extract all the information needed and stores them in their proper data structure
         * in case of filter command the condition query parameter will exist and be handled to fetch only the filtered informations
         * @param conditionQuery
         */
        void initializeECallsAndOCalls(QString conditionQuery = nullptr);

        /**
         * extracts all threads informations and stores them in the list of threads
         * @param conditionQuery
         */
        void initializeThreads(QString conditionQuery = nullptr);

        /**
         * loads and stores the distinct ecalls of the opened trace
         */
        void loadECallTypeList();

        /**
         * loads and stores the different ocalls of the opened trace
         */
        void loadOCallTypeList();

        /**
         * loads and stores the existing enclaves of the opened trace
         */
        void loadExistingEnclaves();

        /**
         * searches for the thread index in the threadslist that has the threadId given as parameter
         * @param threadId
         * @return
         */
        int searchThreadIndex(int threadId);

        /**
         * it extract the occurrence time of all aex events happened inside of the given ecallid and stores them in the ecalls stats data
         * @param ecallId
         * @param ecallStartTime
         */
        void loadAexTimes(int ecallId, uint64_t ecallStartTime);

        /**
         * checks if a specific ocall event already stored inside the ocall statistic list or not
         * @param id
         * @return
         */
        bool isInOCallList(int id);

        /**
         * checks if a specific ecall event already stored inside the ecall statistic list or not
         * @param id
         * @return
         */
        bool isInECallList(int id);

        /**
         * iterates through all loaded threads
         * @return a string with the threads id in a query form
         */
        QString getInvolvedThreads();

        QMap<int, OCallDynamicAnalysis> ocallDynamicAnalysis; //ToDo rewrite this later to take tupels as key for id and eid
        QMap<int, ECallDynamicAnalysis> ecallDynamicAnalysis; //ToDo rewrite this later to take tupels as key for id and eid
        QMap<int, bool> directPublicEcalls;
        QVector<CallStatistics> ecallStatistics;
        QVector<CallStatistics> ocallStatistics;
        QVector<CallStaticAnalysis> ecallStaticAnalysis;
        QVector<CallStaticAnalysis> ocallStaticAnalysis;
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
