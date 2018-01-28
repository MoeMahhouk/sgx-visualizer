//
// Created by moe on 07.01.18.
//

#ifndef SCENE_MATHUTILITY_H
#define SCENE_MATHUTILITY_H

#include <math.h>
#include <type_traits>
#include <QString>
#include <QVector>

namespace moe {
    template <typename T> inline constexpr
    int signum(T x, std::false_type is_signed)
    {
        return T(0) < x;
    }

    template <typename T> inline constexpr
    int signum(T x, std::true_type is_signed)
    {
        return (T(0) < x) - (x < T(0));
    }

    template <typename T> inline constexpr
    int signum(T x)
    {
        return signum(x, std::is_signed<T>());
    }

    inline double median(QVector<uint64_t > sortedTotalTimeList)
    {
        int listSize = sortedTotalTimeList.size();
        return listSize % 2 == 0 ? (sortedTotalTimeList[(listSize / 2) - 1] + sortedTotalTimeList[listSize/2]) / 2.0 : sortedTotalTimeList[(listSize-1) / 2];
    }

    inline double varianz(QVector<uint64_t> sortedTotaltimeList, qreal average)
    {
        int listSize = sortedTotaltimeList.length();
        double varianz = 0;
        for (uint64_t totalTimeElement : sortedTotaltimeList)
        {
            varianz += pow(totalTimeElement - average, 2);
        }
        return varianz / listSize;
    }

    inline double standardDeviation(QVector<uint64_t> sortedTotaltimeList, qreal average)
    {
        return sqrt(varianz(sortedTotaltimeList,average));
    }

    inline QString checkTimeUnit(qreal time)
    {
        if (time == 0) {
            return QString::number(time,'f',2);
        }
        if(time >= pow(10,9)) {
            return QString::number(time / pow(10,9),'f',2) + "s";
        } else if (time >= pow(10,6)) {
            return QString::number(time / pow(10, 6), 'f', 2) + "ms";
        } else if (time >= pow(10,3)) {
            return QString::number(time / pow(10,3),'f',2) + "µs";
        } else if (time >1) {
            return QString::number(time,'f',2) + "ns";
        } else if (time * pow(10,3) > 1) {
            return QString::number(time * pow(10,3),'f',2) + "ps";
        } else if(time * pow(10,6) > 1) {
            return QString::number(time * pow(10,6),'f',2) + "fs";
        } else if(time * pow(10,9) > 1) {
            return QString::number(time * pow(10,9),'f',2) + "as";
        } else {
            return QString::number(0,'f',2);
        }
    }
}

#endif //SCENE_MATHUTILITY_H
