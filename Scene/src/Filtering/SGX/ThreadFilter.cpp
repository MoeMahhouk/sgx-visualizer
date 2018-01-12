//
// Created by moe on 10.01.18.
//

#include "Filtering/SGX/ThreadFilter.h"

moe::ThreadFilterEvent::ThreadFilterEvent()
{

}

moe::ThreadFilterEvent::~ThreadFilterEvent()
{
}

QVector<moe::MyThread> moe::ThreadFilterEvent::execute(const QVector<moe::MyThread> &toFilterList, QVector<int> &chosenThreads)
{
    if(chosenThreads.isEmpty())
        return toFilterList;

    QVector<MyThread> filteredThreadList;
    for (int i = 0; i < chosenThreads.size(); ++i)
    {
        int chosenThreadIndex = chosenThreads[i];
        if(chosenThreadIndex >= toFilterList.size() || chosenThreadIndex < 0){
            std::cerr << "neeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeein" << std::endl;
        }
        MyThread copy = toFilterList[chosenThreadIndex];
        filteredThreadList.push_back(copy);
    }
    return filteredThreadList;
}

QString moe::ThreadFilterEvent::toSQLStatement(QVector<int> &chosenElementIndex) {
    if (chosenElementIndex.isEmpty())
    {
        return "SELECT t.id, t.pthread_id, t.start_address, t.name, IFNULL(t.start_symbol,0),"
                " IFNULL(t.start_symbol_file_name, \"\"),"
                " IFNULL(t.start_address_normalized,0), e1.time AS start_time"
                " FROM threads AS t JOIN events AS e1  ON t.id = e1.involved_thread "
                "WHERE e1.type = 3 ORDER BY t.id";
    } else {
        return "";
    }
}
