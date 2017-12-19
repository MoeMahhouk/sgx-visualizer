//
// Created by moe on 19.12.17.
//

#ifndef SCENE_SGXDATABASESTRUCTURE_H
#define SCENE_SGXDATABASESTRUCTURE_H

#include <QSqlDriver>
#include <QSqlError>
#include <QtCore/QString>
#include <QVariant>
#include "DataBaseManager.h"
#include "MyThread.h"
#include "OCall.h"
#include "EventMap.h"
namespace moe {

    class SgxDatabaseStructure : public DataBaseManager{

    public:
        SgxDatabaseStructure(const QString& path = "newDataBaseCreated");//ToDo maybe add renderable Scene Root node as parameter??
        virtual ~SgxDatabaseStructure() = default;
        int getEcallsNumberOfThreadAtIndex(int index);
    protected:
         uint64_t getThreadStartTime(int index) override;
         uint64_t getProgramStartTime() override;
         int getNumberOfRows(const QString& tableName) override;
         uint64_t getProgramTotalTime() override;
         //void close() override ;
    private:
        void initializeECallsOfThreadAtIndex(int index);
        void initializeThreadAtIndex(int index);
        QVector<MyThread> threads_;
    };
}



#endif //SCENE_SGXDATABASESTRUCTURE_H
