//
// Created by moe on 10.01.18.
//

#include "Filtering/SGX/ThreadFilter.h"

moe::ThreadFilter::ThreadFilter(IReciever *reciever,QVector<int> chosenThreads) : IFilter(reciever), chosenThreads_(chosenThreads)
{

}


QString moe::ThreadFilter::toSQLStatement()
{
    QString conditionQuery = "";
    conditionQuery.append(" AND t.id IN ( ");
    if (!chosenThreads_.isEmpty())
    {

        for (int i = 0; i < chosenThreads_.size() ; ++i) {
            conditionQuery.append(QString::number(chosenThreads_[i]));
            conditionQuery.append(" ,");

        }
        conditionQuery.remove(conditionQuery.size()-1, 1);
        conditionQuery.append(")");
        return conditionQuery;
    } else {
        conditionQuery.append(")");
        return conditionQuery;
    }
}

void moe::ThreadFilter::execute() {
    dReciever_->SetAction(TYPES::ACTION_LIST::THREADFILTER);
    dReciever_->getResult(toSQLStatement());
}
