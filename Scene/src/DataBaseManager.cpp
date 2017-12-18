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
        return result < 0 ? 0 : result; //TODO ask nico or arthur why the first thread start time before the start time of the general program
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
    query.prepare("SELECT COUNT (*) FROM events WHERE involved_thread = (:involved_thread) AND type = 13" );
    query.bindValue(":involved_thread", index);
    if(!query.exec())
    {
        std::cerr << "Error: "<< query.lastError().text().toStdString() << std::endl;
        return -1;
    }
    query.next();
    return query.value(0).toInt();
    }
/**
 * temporary method to test if the querries are valid
 * @param index
 */
//TODO delete this method code at the end
void moe::DataBaseManager::testMethod(int index) {
    QSqlQuery query;
    query.prepare("SELECT pthread_id, start_address, name FROM threads WHERE id = (:id)");
    query.bindValue(":id", index);
    if(!query.exec())
    {
        std::cerr << "Error: "<< query.lastError().text().toStdString() << std::endl;
        return;
    } else {
        query.next();
        std::cerr << "Pthread_id: " << (uint64_t)query.value(0).toDouble() << "\nstart_address: " << query.value(1).toDouble() << "\nname: " << query.value(2).toString().toStdString() << std::endl;
    }
}

void moe::DataBaseManager::initilizeECallsOfThreadAtIndex(int index) {

    QSqlQuery query;
}

