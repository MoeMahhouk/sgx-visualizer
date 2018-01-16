//
// Created by moe on 10.01.18.
//

#include "Filtering/SGX/ECallFilter.h"

moe::ECallFilter::ECallFilter(IReciever *reciever, QVector<int> chosenECalls, QVector<int> chosenOCalls) :
        IFilter(reciever), chosenECalls_(chosenECalls), chosenOCalls_(chosenOCalls)
{

}

QString moe::ECallFilter::toSQLStatement() {
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
        conditionQuery.append("))");
    } else {
        conditionQuery.append("))");
    }
    conditionQuery.append(" OR ( e1.type = 16 AND e1.call_id IN ( ");
    if (!chosenOCalls_.isEmpty()) {
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

void moe::ECallFilter::execute() {
    dReciever_->SetAction(TYPES::ACTION_LIST::ECALLFILTER);
    dReciever_->getResult(toSQLStatement());
}

