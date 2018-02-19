//
// Created by moe on 19.02.18.
//

#include "Utility/TimeStamp.h"
#include "Utility/MathUtility.h"

moe::TimeStamp::TimeStamp(QString timeVal) : QVariant(timeVal) {
    timeVal_ = timeVal.toDouble();
}

QString moe::TimeStamp::toString() const {
    return checkTimeUnit(timeVal_,0) + + " (" + QString::number(timeVal_,'f',0) + " ns)";
}

bool moe::TimeStamp::operator<(const TimeStamp &v) const {
    return timeVal_ < v.timeVal_;
}

bool moe::TimeStamp::operator>(const moe::TimeStamp &v) const {
    return timeVal_ > v.timeVal_;
}

bool moe::TimeStamp::operator!=(const moe::TimeStamp &v) const {
    return timeVal_ != v.timeVal_;
}

bool moe::TimeStamp::operator==(const moe::TimeStamp &v) const {
    return timeVal_ == v.timeVal_;
}


