//
// Created by moe on 10.01.18.
//

#include "Filtering/SGX/ECallFilter.h"

moe::ECallFilter::ECallFilter()
{

}

moe::ECallFilter::~ECallFilter()
{

}

QVector<moe::MyThread>
moe::ECallFilter::execute(const QVector<moe::MyThread> &toFilterList, const QVector<int> &chosenECalls)
{
    if(chosenECalls.isEmpty())
        return toFilterList;

    QVector<MyThread> filteredThreadList = toFilterList;

    for (int i = 0; i < toFilterList.size() ; ++i)
    {
        for (int j = 0; j < toFilterList[i].threadEcalls_.size() ; ++j)
        {
            int currentEcallId = toFilterList[i].threadEcalls_[j]->id_;
            if (!(std::binary_search(chosenECalls.begin(),chosenECalls.end(),currentEcallId) || checkChildren(*toFilterList[i].threadEcalls_[j], chosenECalls)))
            {
                filteredThreadList[i].threadEcalls_.erase(filteredThreadList[i].threadEcalls_.begin()+j);
            }
        }
        if(filteredThreadList[i].threadEcalls_.isEmpty()) {
            filteredThreadList.erase(filteredThreadList.begin()+i);
        }
    }
}


bool moe::ECallFilter::checkChildren(moe::ECall parentEcall, const QVector<int> &chosenECalls)
{
    if (parentEcall.children_.isEmpty())
    {
        return false;
    }

    for (int i = 0; i < parentEcall.children_.size() ; ++i)
    {
        if (!parentEcall.children_[i]->children_.isEmpty())
        {
            for (int j = 0; j < parentEcall.children_[i]->children_.size() ; ++j)
            {
                if (std::binary_search(chosenECalls.begin(), chosenECalls.end(), parentEcall.children_[i]->children_[j]->id_))
                {
                    return true;
                } else if (checkChildren(parentEcall.children_[i]->children_[j]->id_, chosenECalls)) {
                    return true;
                }
            }
        }
    }
    return false;
}
