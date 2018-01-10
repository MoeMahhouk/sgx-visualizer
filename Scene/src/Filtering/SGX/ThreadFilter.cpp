//
// Created by moe on 10.01.18.
//

#include "Filtering/SGX/ThreadFilter.h"

moe::ThreadFilter::ThreadFilter()
{

}

moe::ThreadFilter::~ThreadFilter()
{
}

QVector<moe::MyThread> moe::ThreadFilter::execute(const QVector<moe::MyThread> &toFilterList, const QVector<int> &chosenThreads)
{
    if(chosenThreads.isEmpty())
        return toFilterList;

    QVector<MyThread> filteredThreadList = QVector<MyThread>(chosenThreads.size());
    for (int i = 0; i < chosenThreads.size(); ++i)
    {
        int chosenThreadIndex = chosenThreads[i];
        filteredThreadList.push_back(toFilterList[chosenThreadIndex]);
    }
    return filteredThreadList;
}
