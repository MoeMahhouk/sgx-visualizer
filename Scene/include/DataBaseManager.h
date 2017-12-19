//
// Created by moe on 17.12.17.
//

#ifndef SCENE_DATABASEMANAGER_H
#define SCENE_DATABASEMANAGER_H


#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <iostream>


namespace moe{

    class DataBaseManager {

    public:

        DataBaseManager(const QString& path = "newDataBaseCreated") {
            m_db = QSqlDatabase::addDatabase("QSQLITE");
            m_db.setDatabaseName(path);
            if (!m_db.open()) {
                std::cerr << "Error: connection with database fail" << std::endl;
            } else {
                std::cout << "Database: Connection Ok" << std::endl;
            }
        }

        virtual ~DataBaseManager() {
           m_db.close();
        }

    protected:
        virtual uint64_t getThreadStartTime(int index);
        virtual uint64_t getProgramStartTime();
        virtual int getNumberOfRows(const QString& tableName);
        virtual uint64_t getProgramTotalTime();
        QSqlDatabase m_db;
    };
}


#endif //SCENE_DATABASEMANAGER_H
