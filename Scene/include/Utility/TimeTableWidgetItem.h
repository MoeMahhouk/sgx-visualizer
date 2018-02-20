//
// Created by moe on 20.02.18.
//

#ifndef SCENE_TIMETABLEWIDGETITEM_H
#define SCENE_TIMETABLEWIDGETITEM_H

#include <QtWidgets/QTableWidgetItem>
#include "Utility/MathUtility.h"
namespace moe {
    class TimeTableWidgetItem : public QTableWidgetItem {

    public:
        TimeTableWidgetItem(qreal timeVal);
        bool operator<(const QTableWidgetItem &other) const override;

    private:
        qreal timeValue_;
    };

}



#endif //SCENE_TIMETABLEWIDGETITEM_H
