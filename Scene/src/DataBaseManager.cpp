//
// Created by moe on 17.12.17.
//

#include <iostream>

#include "DataBaseManager.h"

moe::DataBaseManager::DataBaseManager(const QString &path) {
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName(path);
    if (!m_db.open())
    {
        std::cerr << "Error: connection with database fail" << std::endl;
    } else {
        std::cout << "Database: Connection Ok" << std::endl;
    }
    threads_ = QVector<MyThread>(getNumberOfRows("threads"),MyThread());
    for (int i = 0; i < threads_.length() ; ++i) {
        initilizeThreadAtIndex(i);
        initilizeECallsOfThreadAtIndex(i);
    }
}

int moe::DataBaseManager::getNumberOfRows(const QString &tableName) {
    QSqlQuery query;
    query.prepare("SELECT * FROM " + tableName);
    //query.prepare("SELECT * FROM Ecalls");
    //query.bindValue(":tablename",tableName);
    int numRows;
    if(!query.exec())
    {
        std::cerr << "Error: "<< query.lastError().text().toStdString() << std::endl;
        return -1;
    }
    if(m_db.driver()->hasFeature(QSqlDriver::QuerySize))
    {
        return query.size();
    } else {
        query.last();
        numRows = query.at() + 1;
        return numRows;
    }
}
/**
 * closes the datebase file
 */
void moe::DataBaseManager::close() {
    m_db.close();
}
/**
 * initializes the threads in the vector MyThreads
 * @param index
 */
void moe::DataBaseManager::initilizeThreadAtIndex(int index) {
    uint64_t pthread_id,start_address, start_address_normalized, start_symbol, start_time, total_time;

    std::string name, start_symbol_file_name; // TODO add start symbol name and start address normalized and start symbol

    int ecallNumbers = getEcallsNumberOfThreadAtIndex(index); //TODO get successfull ecall Numbers of the thread and not all

    start_time = getThreadTime(index);
    total_time = getTotalTime() - start_time; // TODO another query according the thread destruction event

    QSqlQuery query;
    query.prepare("SELECT pthread_id, start_address, name FROM threads WHERE id = (:id)");
    query.bindValue(":id", index);
    if(!query.exec())
    {
        std::cerr << "Error: "<< query.lastError().text().toStdString() << std::endl;
        return;
    } else {
        query.next();
        pthread_id = (uint64_t) query.value(0).toDouble();
        start_address = (uint64_t) query.value(1).toDouble();
        name = query.value(2).toString().toStdString();
        threads_[index] = MyThread(index, pthread_id, start_address, 0, 0, start_time, total_time, name, "",ecallNumbers);
    } //TODO get the ECalls and add them to the children of their parent thread (considering that Ecalls might as well have children)
}
/**
 * @return the absolute runtime of the program
 */
uint64_t moe::DataBaseManager::getTotalTime() {
    QSqlQuery query;
    query.prepare("SELECT value FROM general where key LIKE 'start_time' OR key LIKE 'end_time'");
    if(!query.exec())
    {
        std::cerr << "Error: "<< query.lastError().text().toStdString() << std::endl;
        return -1;
    } else {
        query.next();
        //std::cout << query.value(0).toInt() << std::endl;
        uint64_t start_time = query.value(0).toDouble();
        query.next();
        //std::cout << query.value(0).toInt() << std::endl;
        return query.value(0).toDouble() - start_time;
    }
}


/**
 * @param index
 * @return returns the absolute start time of the thread at given index
 */
uint64_t moe::DataBaseManager::getThreadTime(int index) {
    QSqlQuery query;
    query.prepare("SELECT time FROM events WHERE involved_thread = (:involvedThread) AND type = 3");
    query.bindValue(":involvedThread", index);
    if(!query.exec())
    {
        std::cerr << "Error: "<< query.lastError().text().toStdString() << std::endl;
        return -1;
    } else {
        query.next();
        int64_t result = query.value(0).toDouble() - getProgramStartTime();
        return result < 0 ? 0 : result; //ToDo problem got fixed but this still not hurt :-)
    }
}

/**
 * @return the program's start time relatively to the PC logged with
 */
uint64_t moe::DataBaseManager::getProgramStartTime() {
    QSqlQuery query;
    query.prepare("SELECT value FROM general where key LIKE 'start_time'");
    if(!query.exec())
    {
        std::cerr << "Error: "<< query.lastError().text().toStdString() << std::endl;
        return -1;
    } else {
        query.next();
        //std::cout << query.value(0).toInt() << std::endl;
        return query.value(0).toDouble();
    }
}
/**
 *
 * @param index
 * @return the number of Ecalls which are called from the thread at the given index
 */
//TODO this methode should be reworked later for the failed Ecall Creation etc...
int moe::DataBaseManager::getEcallsNumberOfThreadAtIndex(int index) {
    QSqlQuery query;
    query.prepare("SELECT COUNT (*) FROM events AS e1 JOIN events AS e2 ON e1.id = e2.call_event "
                          "WHERE e1.involved_thread = (:involved_thread) AND e1.type = 14 AND e2.return_value = 0");
    query.bindValue(":involved_thread", index);
    if(!query.exec())
    {
        std::cerr << "Error: "<< query.lastError().text().toStdString() << std::endl;
        return -1;
    }
    query.next();
    return query.value(0).toInt();
    }


void moe::DataBaseManager::initilizeECallsOfThreadAtIndex(int index) {
    QMap<int, Call*> calls;
    int id,call_id,call_event;
    uint64_t start_time, total_time;
    QSqlQuery query;
    query.prepare("SELECT e1.id,e1.type,e1.time AS start_time,e2.time AS end_time,e1.call_id,IFNULL(e1.call_event, 0) AS call_event"
                          " FROM events AS e1 JOIN events AS e2 ON e1.id = e2.call_event"
                          " WHERE e2.return_value = 0 AND e1.involved_thread = (:involved_thread) ORDER BY e1.id");
    query.bindValue(":involved_thread", index);
    if(!query.exec())
    {
        std::cerr << "Error: "<< query.lastError().text().toStdString() << std::endl;
        return;
    }
    while(query.next())
    {
        id = query.value(0).toInt();
        start_time = (uint64_t) query.value(2).toDouble() - getProgramStartTime();
        total_time = (uint64_t) query.value(3).toDouble() - query.value(2).toDouble();
        call_id = query.value(4).toInt();
        call_event = query.value(5).toInt();

        switch(query.value(1).toInt()) { //EventType
            case (int)EventMap::EnclaveECallEvent: {
                QSqlQuery tmpQuery;
                tmpQuery.prepare("SELECT eid,symbol_address,symbol_name,is_private FROM ecalls WHERE id = (:id)");
                tmpQuery.bindValue(":id",call_id);
                if(!tmpQuery.exec()) {
                    std::cerr << "Error: "<< query.lastError().text().toStdString() << std::endl;
                    return;
                }
                tmpQuery.next();
                int eid = tmpQuery.value(0).toInt();
                uint64_t symbol_address = (uint64_t)tmpQuery.value(1).toDouble();
                std::string symbol_name = tmpQuery.value(2).toString().toStdString();
                bool is_private = tmpQuery.value(3).toInt();
                ECall *eCall = new ECall(call_id,eid,symbol_address,start_time,total_time,is_private,symbol_name);
                calls[id] = eCall;

                if(call_event != 0) {
                    calls[call_event]->children_.push_back(eCall);
                } else {
                    threads_[index].threadEcalls_.push_back(eCall);
                }
                break;
            }
            case (int)EventMap::EnclaveOCallEvent : {
                QSqlQuery tmpQuery;
                tmpQuery.prepare("SELECT eid,symbol_name,symbol_file_name,symbol_address,symbol_address_normalized "
                                         " FROM ocalls WHERE id = (:id)");
                tmpQuery.bindValue(":id",call_id);
                if(!tmpQuery.exec()) {
                    std::cerr << "Error: "<< query.lastError().text().toStdString() << std::endl;
                    return;
                }
                tmpQuery.next();
                int eid = tmpQuery.value(0).toInt();
                std::string symbol_name = tmpQuery.value(1).toString().toStdString();
                std::string symbol_file_name = tmpQuery.value(2).toString().toStdString();
                uint64_t symbol_address = (uint64_t)tmpQuery.value(3).toDouble();
                uint64_t symbol_address_normalized = (uint64_t)tmpQuery.value(4).toDouble();

                OCall *oCall = new OCall(call_id,eid,symbol_address,start_time,total_time,symbol_name,
                                         symbol_address_normalized,symbol_file_name);
                calls[id] = oCall;

                if(call_event != 0) {
                    calls[call_event]->children_.push_back(oCall);
                } else { //this should never be reachable unless the database is corrupted
                    std::cerr << "OCall has no ecall id from which its triggered " << std::endl;
                }
                break;
            default://should never happen if the query runs perfectly
                break;
            }
        }
    }
}

