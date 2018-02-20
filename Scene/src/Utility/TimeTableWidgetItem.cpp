//
// Created by moe on 20.02.18.
//

#include "Utility/TimeTableWidgetItem.h"


moe::TimeTableWidgetItem::TimeTableWidgetItem(qreal timeVal) :
        QTableWidgetItem(moe::checkTimeUnit(timeVal,0) + " (" + QString::number(timeVal,'f',0) + " ns)"), timeValue_(timeVal)
{

}

bool moe::TimeTableWidgetItem::operator<(const QTableWidgetItem &other) const
{
    TimeTableWidgetItem tmp = static_cast<const TimeTableWidgetItem&>(other);
    return timeValue_ < tmp.timeValue_;
}
