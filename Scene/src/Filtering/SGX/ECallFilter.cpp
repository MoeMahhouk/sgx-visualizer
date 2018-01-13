//
// Created by moe on 10.01.18.
//

#include "Filtering/SGX/ECallFilter.h"

moe::ECallFilter::ECallFilter(IReciever *reciever, QVector<int> chosenElements) : IFilter(reciever, chosenElements)
{

}

QString moe::ECallFilter::toSQLStatement() {
    QString conditionQuery = "";
    if (!chosenElements_.isEmpty())
    {

        conditionQuery.append(" AND e1.call_id IN ( ");
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

void moe::ECallFilter::execute() {
    dReciever_->SetAction(TYPES::ACTION_LIST::ECALLFILTER);
    dReciever_->getResult(toSQLStatement());
}

