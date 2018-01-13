//
// Created by moe on 10.01.18.
//

#include "Filtering/SGX/ThreadFilter.h"

moe::ThreadFilter::ThreadFilter(IReciever *reciever,QVector<int> chosenElements) : IFilter(reciever, chosenElements)
{

}


QString moe::ThreadFilter::toSQLStatement()
{
    QString conditionQuery = "";
    if (!chosenElements_.isEmpty())
    {

        conditionQuery.append(" AND t.id IN ( ");
        for (int i = 0; i < chosenElements_.size() ; ++i) {
            conditionQuery.append(QString::number(chosenElements_[i]));
            conditionQuery.append(" ,");

        }
        conditionQuery.remove(conditionQuery.size()-1, 1);
        conditionQuery.append(")");
        return conditionQuery;
    } else {
        return conditionQuery;
    }
}

void moe::ThreadFilter::execute() {
    dReciever_->SetAction(TYPES::ACTION_LIST::THREADFILTER);
    dReciever_->getResult(toSQLStatement());
}
