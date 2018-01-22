//
// Created by moe on 10.01.18.
//

#include "Filtering/SGX/ECallOCallFilter.h"

moe::ECallOCallFilter::ECallOCallFilter(IReciever *reciever, QVector<int> chosenECalls, QVector<int> chosenOCalls, QVector<int> chosenEnclaves) :
        IFilter(reciever), chosenECalls_(chosenECalls), chosenOCalls_(chosenOCalls), chosenEnclaves_(chosenEnclaves)
{

}

QString moe::ECallOCallFilter::toSQLStatement()
{
    QString conditionQuery = "";
    conditionQuery.append(" AND ( (e1.type = 14 AND e1.call_id IN ( ");
    if (!chosenECalls_.isEmpty())
    {
        for (int i = 0; i < chosenECalls_.size() ; ++i)
        {
            conditionQuery.append(QString::number(chosenECalls_[i]));
            conditionQuery.append(" ,");
        }
        conditionQuery.remove(conditionQuery.size()-1, 1);
        conditionQuery.append(")");
    } else {
        conditionQuery.append(")");
    }

    conditionQuery.append(" AND e1.eid in ( ");
    if (!chosenEnclaves_.isEmpty())
    {
        for (int i = 0; i < chosenEnclaves_.size() ; ++i) {
            conditionQuery.append(QString::number(chosenEnclaves_[i]));
            conditionQuery.append(" ,");
        }
        conditionQuery.remove(conditionQuery.size()-1, 1);
        conditionQuery.append("))");
    } else {
        conditionQuery.append("))");
    }

    conditionQuery.append(" OR ( e1.type = 16 AND e1.call_id IN ( ");

    if (!chosenOCalls_.isEmpty())
    {
        for (int i = 0; i < chosenOCalls_.size() ; ++i)
        {
            conditionQuery.append(QString::number(chosenOCalls_[i]));
            conditionQuery.append(" ,");
        }
        conditionQuery.remove(conditionQuery.size()-1, 1);
        conditionQuery.append("))");
    } else {
        conditionQuery.append("))");
    }
    conditionQuery.append(")");
    return conditionQuery;
}

void moe::ECallOCallFilter::execute()
{
    dReciever_->SetAction(TYPES::ACTION_LIST::ECALLOCALLFILTER);
    dReciever_->getResult(toSQLStatement());
}

