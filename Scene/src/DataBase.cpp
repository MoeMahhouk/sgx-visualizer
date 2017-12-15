//
// Created by moe on 13.12.17.
//

#include "DataBase.h"

moe::DataBase::DataBase(sqlite3* db) {
    char *zErrMsg = 0;
    int rc;
    rc = sqlite3_exec(db,"Select COUNT (*) from threads",static_thread_numbers_callback,(void*)this,&zErrMsg);
    threads_ = QVector<MyThread>(threadNumbers,MyThread());
    for (int i = 0; i < threads_.length() ; ++i) {
        threads_[i] = MyThread(1,1,1,1,1,"blabla","blabla");
    }
}

int moe::DataBase::static_thread_numbers_callback(void *data, int argc, char **argv, char **azColName) {
    return static_cast<DataBase*>(data)->setThreadNumbers(argc, argv, azColName);

}

int moe::DataBase::setThreadNumbers(int argc, char **argv, char **azColName) {
   threadNumbers = atoi(argv[0]);
    return 0;
}


/*int moe::DataBase::onThreadsReceived(void *data, int argc, char **argv, char **azColName) {
    b->threads = QVector(arguments.threadnumber);
}

int moe::DataBase::onThreadData(void *data, int argc, char **argv, char **azColName) {
    databse.thread[number].create(arguments;)
}{
}

moe::DataBase* moe::DataBase::Create(sqlite3* db) {

    DataBase *b = new DataBase();
    sqlite3_exec(ask for thread number, &onThreadsReceived);
    sqlite3_exec(get thread data, &onThreadData);
}
*/




