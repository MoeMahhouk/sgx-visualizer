//
// Created by moe on 19.02.18.
//

#ifndef SCENE_TIMESTAMP_H
#define SCENE_TIMESTAMP_H


#include <QtCore/QVariant>

namespace moe {
    class TimeStamp : public QVariant{
    public:

        TimeStamp(QString timeVal);
        QString toString() const;

        bool operator<(const TimeStamp &v) const;
        bool operator>(const TimeStamp &v) const;
        bool operator!=(const TimeStamp &v) const;
        bool operator==(const TimeStamp &v) const;

    private:
        double timeVal_;
    };
}



#endif //SCENE_TIMESTAMP_H
