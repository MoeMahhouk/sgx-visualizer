//
// Created by moe on 07.01.18.
//

#ifndef SCENE_MATHUTILITY_H
#define SCENE_MATHUTILITY_H

#include <math.h>
#include <type_traits>
#include <QString>
#include <QVector>

/**
 * this is a gathering of global math functions
 */
namespace moe {


    struct AbstractTimeUnit {
        qreal absTime_ = 0;
        QString absTimeWithUnit_ = "";
        QString timeWithUnit_ = "";
    };

    /**
     * returns the signum of the given number
     * @tparam T
     * @param x
     * @param is_signed
     * @return
     */
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

    /**
     * checks if the given number is int or not
     * @tparam T
     * @param x
     * @return
     */
    template <typename T> inline constexpr
    bool isInteger(T x)
    {
        return floorf(x) == x;
    }

    /**
     * computes the median of the sorted list
     * @param sortedTotalTimeList
     * @return
     */
    inline double median(QVector<uint64_t > sortedTotalTimeList)
    {
        int listSize = sortedTotalTimeList.size();
        return listSize % 2 == 0 ? (sortedTotalTimeList[(listSize / 2) - 1] + sortedTotalTimeList[listSize/2]) / 2.0 : sortedTotalTimeList[(listSize-1) / 2];
    }

    /**
     * calculates the variant of the given list and average
     * @param sortedTotaltimeList
     * @param average
     * @return
     */
    template <typename T> inline double varianz(QVector<T> sortedTotaltimeList, qreal average)
    {
        int listSize = sortedTotaltimeList.length();
        double varianz = 0;
        for (uint64_t totalTimeElement : sortedTotaltimeList)
        {
            varianz += pow((double)totalTimeElement - average, 2);
        }
        return (varianz / (double)listSize);
    }

    /**
     * calculates the standard deviation of the given list using the given average
     * @param sortedTotaltimeList
     * @param average
     * @return
     */
    template <typename T> inline double standardDeviation(QVector<T> sortedTotaltimeList, qreal average)
    {
        return sqrt(varianz(sortedTotaltimeList,average));
    }

    /**
     * computes the desired percentile of the given list
     * @param sortedTotaltimeList
     * @param percent
     * @return
     */
    inline double percentile(QVector<uint64_t> sortedTotaltimeList, qreal percent)
    {
        int listSize = sortedTotaltimeList.size();
        double result = listSize * percent;
        return isInteger(result) ? (sortedTotaltimeList[result - 1] + sortedTotaltimeList[result]) / 2 : sortedTotaltimeList[ceilf(result) - 1];
    }

    /**
     * convert the time from ns into the nearest time unit
     * @param time
     * @param decimal
     * @return
     */
    inline QString checkTimeUnit(qreal time,int decimal = 2)
    {
        if (time == 0) {
            return QString::number(time,'f',decimal);
        }
        if(time >= pow(10,9)) {
            return QString::number(time / pow(10,9),'f',decimal) + " s";
        } else if (time >= pow(10,6)) {
            return QString::number(time / pow(10, 6), 'f', decimal) + " ms";
        } else if (time >= pow(10,3)) {
            return QString::number(time / pow(10,3),'f',decimal) + " µs";
        } else if (time >1) {
            return QString::number(time,'f',decimal) + " ns";
        } else if (time * pow(10,3) > 1) {
            return QString::number(time * pow(10,3),'f',decimal) + " ps";
        } else if(time * pow(10,6) > 1) {
            return QString::number(time * pow(10,6),'f',decimal) + " fs";
        } else if(time * pow(10,9) > 1) {
            return QString::number(time * pow(10,9),'f',decimal) + " as";
        } else {
            return QString::number(0,'f',decimal);
        }
    }
    /*
     * ToDo this is for later implementation of sorting in statistics tables
     */
    inline AbstractTimeUnit checkAbsTimeUnit(qreal time, int decimal = 2)
    {
        AbstractTimeUnit result;
        result.absTime_ = QString::number(time,'f',decimal).toDouble();
        result.absTimeWithUnit_ = QString::number(time,'f',decimal) + " ns";
        if (time == 0) {
            result.timeWithUnit_ = QString::number(time,'f',decimal);
            return result;
        }
        if(time >= pow(10,9)) {
            result.timeWithUnit_ = QString::number(time / pow(10,9),'f',decimal) + " s";
            return result;
        } else if (time >= pow(10,6)) {
            result.timeWithUnit_ = QString::number(time / pow(10, 6), 'f', decimal) + " ms";
            return result;
        } else if (time >= pow(10,3)) {
            result.timeWithUnit_ = QString::number(time / pow(10,3),'f',decimal) + " µs";
            return result;
        } else if (time >1) {
            result.timeWithUnit_ = QString::number(time,'f',decimal) + " ns";
            return result;
        } else if (time * pow(10,3) > 1) {
            result.timeWithUnit_ = QString::number(time * pow(10,3),'f',decimal) + " ps";
            return result;
        } else if(time * pow(10,6) > 1) {
            result.timeWithUnit_ = QString::number(time * pow(10,6),'f',decimal) + " fs";
            return result;
        } else if(time * pow(10,9) > 1) {
            result.timeWithUnit_ = QString::number(time * pow(10,9),'f',decimal) + " as";
            return result;
        } else {
            result.timeWithUnit_ = QString::number(0,'f',decimal);
            return result;
        }
    }

}

#endif //SCENE_MATHUTILITY_H
