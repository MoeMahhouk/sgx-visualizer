//
// Created by moe on 02.04.18.
//

#ifndef SCENE_STATISTICSDIALOG_H
#define SCENE_STATISTICSDIALOG_H

#include <QtWidgets/QTableWidget>
#include <QHeaderView>
#include <QtWidgets/QVBoxLayout>
#include "Utility/TimeTableWidgetItem.h"
#include "CustomWidgets/IDialogWidget/IDialog.h"
#include "DataBase/SGX/SgxDatabaseStructure.h"

namespace moe {

    class StatisticsDialog : public IDialog{

    public:

        StatisticsDialog(SgxDatabaseStructure &db, QWidget *parent = 0, Qt::WindowFlags f = 0);
        ~StatisticsDialog() {};

    protected:

        void generate() override;

    private:

        QTableWidget *loadOCallStats();

        QTableWidget *loadECallStats();

        SgxDatabaseStructure &db_;
    };

}


#endif //SCENE_STATISTICSDIALOG_H
