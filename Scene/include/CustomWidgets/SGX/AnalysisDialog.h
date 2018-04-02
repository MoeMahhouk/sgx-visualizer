//
// Created by moe on 02.04.18.
//

#ifndef SCENE_ANALYSISDIALOG_H
#define SCENE_ANALYSISDIALOG_H

#include <QtWidgets/QTableWidget>
#include <QHeaderView>
#include "CustomWidgets/IDialogWidget/IDialog.h"
#include "DataBase/SGX/SgxDatabaseStructure.h"

namespace moe {

    class AnalysisDialog : public IDialog {

    public:

        AnalysisDialog(SgxDatabaseStructure &db, QWidget *parent = 0, Qt::WindowFlags f = 0);

        ~AnalysisDialog(){};


    protected:

        void generate() override;

    private:
        QTableWidget *loadCallStaticTable(const QVector<CallStaticAnalysis> &callStaticAnalysisList);

        QTableWidget *loadECallDynamicTable(const QMap<int,ECallDynamicAnalysis> &ecallDynamicAnalysisMap);

        QTableWidget *loadOCallDynamicTable(const QMap<int,OCallDynamicAnalysis> &ocallDynamicAnalysisMap);


        SgxDatabaseStructure &db_;
    };

}


#endif //SCENE_ANALYSISDIALOG_H
