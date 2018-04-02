//
// Created by moe on 02.04.18.
//

#include <QtWidgets/QVBoxLayout>
#include "CustomWidgets/SGX/AnalysisDialog.h"

moe::AnalysisDialog::AnalysisDialog(moe::SgxDatabaseStructure &db, QWidget *parent, Qt::WindowFlags f) :
IDialog(parent,f), db_(db)
{
    generate();
}

void moe::AnalysisDialog::generate()
{
    db_.loadEcallAnalysis();
    db_.loadOcallAnalysis();
    setWindowTitle("Call Static Analysis");

    auto dialogLayout = new QVBoxLayout();

    auto analysisDialogTab = new QTabWidget();
    auto staticAnalysisDialogTab = new QTabWidget();
    auto dynamicAnalysisDialogTab = new QTabWidget();

    auto eCallStaticAnalysisTable = loadCallStaticTable(db_.getEcallStaticAnalysis());
    auto oCallStaticAnalysisTable = loadCallStaticTable(db_.getOcallStaticAnalysis());
    auto eCallDynamicAnalysisTable = loadECallDynamicTable(db_.getEcallDynamicAnalysis());
    auto oCallDynamicAnalysisTable = loadOCallDynamicTable(db_.getOcallDynamicAnalysis());

    analysisDialogTab->addTab(staticAnalysisDialogTab,"Static");
    analysisDialogTab->addTab(dynamicAnalysisDialogTab,"Dynamic");
    staticAnalysisDialogTab->addTab(eCallStaticAnalysisTable,"ECall");
    staticAnalysisDialogTab->addTab(oCallStaticAnalysisTable,"OCall");
    dynamicAnalysisDialogTab->addTab(eCallDynamicAnalysisTable, "ECall");
    dynamicAnalysisDialogTab->addTab(oCallDynamicAnalysisTable, "OCall");
    dialogLayout->addWidget(analysisDialogTab);
    setLayout(dialogLayout);
    setWindowFlags(Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint);

}

QTableWidget *moe::AnalysisDialog::loadCallStaticTable(const QVector<CallStaticAnalysis> &callStaticAnalysisList)
{
    auto callStaticAnalysisTable = new QTableWidget();
    callStaticAnalysisTable->setRowCount(callStaticAnalysisList.size());
    callStaticAnalysisTable->setColumnCount(2);
    callStaticAnalysisTable->setHorizontalHeaderLabels(QString("Call Name;Analysis").split(";"));
    for (int i = 0; i < callStaticAnalysisList.size() ; ++i)
    {
        callStaticAnalysisTable->setItem(i,0,new QTableWidgetItem(callStaticAnalysisList[i].callName_));
        auto textItem = new QTableWidgetItem(callStaticAnalysisList[i].analysisText_);
        !callStaticAnalysisList[i].warning ? textItem->setBackground(QBrush(Qt::green)) : textItem->setForeground(QBrush(Qt::red));
        callStaticAnalysisTable->setItem(i,1,textItem);
    }
    callStaticAnalysisTable->resizeRowsToContents();
    //callStaticAnalysisTable->resizeColumnsToContents();
    callStaticAnalysisTable->horizontalHeader()->setStretchLastSection(true);
    callStaticAnalysisTable->verticalHeader()->setStretchLastSection(true);
    callStaticAnalysisTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    callStaticAnalysisTable->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    return callStaticAnalysisTable;
}


QTableWidget *
moe::AnalysisDialog::loadECallDynamicTable(const QMap<int, ECallDynamicAnalysis> &ecallDynamicAnalysisMap)
{
    auto callDynamicAnalysisTable = new QTableWidget();
    callDynamicAnalysisTable->setRowCount(ecallDynamicAnalysisMap.size());
    callDynamicAnalysisTable->setColumnCount(3);
    callDynamicAnalysisTable->setHorizontalHeaderLabels(QString("Id;Call Name;Analysis").split(";"));
    QMap<int,ECallDynamicAnalysis>::const_iterator k;
    int tblCntr = 0;
    for (k = ecallDynamicAnalysisMap.begin();  k != ecallDynamicAnalysisMap.end() ; k++)
    {
        auto item = new QTableWidgetItem;
        item->setData(Qt::EditRole,k.value().callId_);
        callDynamicAnalysisTable->setItem(tblCntr, 0, item);
        callDynamicAnalysisTable->setItem(tblCntr, 1, new QTableWidgetItem(k.value().callName_));
        auto textItem = new QTableWidgetItem(k.value().analysisText_);
        !k.value().warning ? textItem->setBackground(QBrush(Qt::green)) : textItem->setForeground(QBrush(Qt::red));
        callDynamicAnalysisTable->setItem(tblCntr, 2, textItem);
        tblCntr++;
    }
    callDynamicAnalysisTable->resizeColumnsToContents();
    callDynamicAnalysisTable->resizeRowsToContents();
    callDynamicAnalysisTable->horizontalHeader()->setStretchLastSection(true);
    callDynamicAnalysisTable->verticalHeader()->setStretchLastSection(true);
    callDynamicAnalysisTable->verticalHeader()->hide();
    callDynamicAnalysisTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    callDynamicAnalysisTable->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    return callDynamicAnalysisTable;
}

QTableWidget *
moe::AnalysisDialog::loadOCallDynamicTable(const QMap<int, moe::OCallDynamicAnalysis> &ocallDynamicAnalysisMap)
{
    auto callDynamicAnalysisTable = new QTableWidget();
    callDynamicAnalysisTable->setRowCount(ocallDynamicAnalysisMap.size());
    callDynamicAnalysisTable->setColumnCount(3);
    callDynamicAnalysisTable->setHorizontalHeaderLabels(QString("Id;Call Name;Analysis").split(";"));
    QMap<int,OCallDynamicAnalysis>::const_iterator k;
    int tblCntr = 0;
    for (k = ocallDynamicAnalysisMap.begin();  k != ocallDynamicAnalysisMap.end() ; k++)
    {
        auto item = new QTableWidgetItem;
        item->setData(Qt::EditRole,k.value().callId_);
        callDynamicAnalysisTable->setItem(tblCntr, 0, item);
        callDynamicAnalysisTable->setItem(tblCntr, 1, new QTableWidgetItem(k.value().callName_));
        auto textItem = new QTableWidgetItem(k.value().analysisText_);
        !k.value().warning ? textItem->setBackground(QBrush(Qt::green)) : textItem->setForeground(QBrush(Qt::red));
        callDynamicAnalysisTable->setItem(tblCntr, 2, textItem);
        tblCntr++;
    }
    callDynamicAnalysisTable->resizeColumnsToContents();
    callDynamicAnalysisTable->resizeRowsToContents();
    callDynamicAnalysisTable->horizontalHeader()->setStretchLastSection(true);
    callDynamicAnalysisTable->verticalHeader()->setStretchLastSection(true);
    callDynamicAnalysisTable->verticalHeader()->hide();
    callDynamicAnalysisTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    callDynamicAnalysisTable->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    return callDynamicAnalysisTable;
}
