//
// Created by moe on 20.02.18.
//

#ifndef SCENE_TIMETABLEWIDGETITEM_H
#define SCENE_TIMETABLEWIDGETITEM_H

#include <QtWidgets/QTableWidgetItem>
#include "Utility/MathUtility.h"
namespace moe {
    /**
     * extended QTableWidgetItem in order to sort the time entries in the tables not as strings but as real time integers
     */
    class TimeTableWidgetItem : public QTableWidgetItem {

    public:
        TimeTableWidgetItem(qreal timeVal);
        bool operator<(const QTableWidgetItem &other) const override;

    private:
        qreal timeValue_;
    };

}



#endif //SCENE_TIMETABLEWIDGETITEM_H
