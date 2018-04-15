//
// Created by moe on 02.04.18.
//

#include "CustomWidgets/SGX/StatisticsDialog.h"


moe::StatisticsDialog::StatisticsDialog(moe::SgxDatabaseStructure &db, QWidget *parent, Qt::WindowFlags f) :
        IDialog(parent,f), db_(db)
{
    generate();
}

void moe::StatisticsDialog::generate()
{
    db_.loadEcallsStats();
    db_.loadOcallsStats();
    setWindowTitle("E/OCall Statistics");
    auto statsDiagLayout = new QVBoxLayout();

    auto statsDiagTab = new QTabWidget();

    auto ecallStatsTable = loadECallStats();
    auto ocallStatsTable = loadOCallStats();

    statsDiagTab->addTab(ecallStatsTable, "ECall");
    statsDiagTab->addTab(ocallStatsTable, "OCall");
    statsDiagLayout->addWidget(statsDiagTab);
    setLayout(statsDiagLayout);
    setWindowFlags(Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint);

}

QTableWidget *moe::StatisticsDialog::loadOCallStats()
{
    const QVector<moe::CallStatistics> &oCallStatsList = db_.getOcallStatistics();
    auto table = new QTableWidget();
    table->setRowCount(oCallStatsList.size());
    table->setColumnCount(9);

    table->setSortingEnabled(false);
    table->setHorizontalHeaderLabels(QString("ID;Call Name;Count;Average;Median;Standard Deviation;99th Percentile;95th Percentile;90th Percentile").split(";"));
    for (int i = 0; i < table->rowCount(); ++i)
    {
        auto idItem = new QTableWidgetItem;
        idItem->setData(Qt::EditRole,oCallStatsList[i].callId_);
        table->setItem(i,0, idItem);
        table->setItem(i,1, new QTableWidgetItem(oCallStatsList[i].callSymbolName_));
        auto countItem = new QTableWidgetItem;
        countItem->setData(Qt::EditRole,oCallStatsList[i].count_);
        table->setItem(i,2, countItem);
        if(oCallStatsList[i].count_ != 0)
        {
            table->setItem(i,3, new moe::TimeTableWidgetItem(oCallStatsList[i].callAvg_));
            table->setItem(i,4, new moe::TimeTableWidgetItem(oCallStatsList[i].median_));
            table->setItem(i,5, new moe::TimeTableWidgetItem(oCallStatsList[i].standardDeviation_));
            table->setItem(i,6, new moe::TimeTableWidgetItem(oCallStatsList[i]._99thPercentile_));
            table->setItem(i,7, new moe::TimeTableWidgetItem(oCallStatsList[i]._95thPercentile_));
            table->setItem(i,8, new moe::TimeTableWidgetItem(oCallStatsList[i]._90thPercentile_));
        }
    }
    table->resizeColumnsToContents();
    table->resizeRowsToContents();
    int height = 0,width = 0;
    for (int j = 0; j < table->columnCount() ; ++j)
    {
        width += table->columnWidth(j)+6;
    }
    for (int k = 0; k < table->rowCount() ; ++k)
    {
        height += table->rowHeight(k)+6;
    }
    table->horizontalHeader()->setStretchLastSection(true);
    table->verticalHeader()->setStretchLastSection(true);
    table->verticalHeader()->hide();
    table->setSortingEnabled(true);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    return table;
}

QTableWidget *moe::StatisticsDialog::loadECallStats()
{
    const QVector<moe::CallStatistics> &eCallStatsList = db_.getEcallStatistics();

    auto table = new QTableWidget();
    table->setRowCount(eCallStatsList.size());
    table->setColumnCount(9);

    table->setSortingEnabled(false);
    table->setHorizontalHeaderLabels(QString("ID;Call Name;Count;Average;Median;Standard Deviation;99th Percentile;95th Percentile;90th Percentile").split(";"));
    for (int i = 0; i < table->rowCount(); ++i)
    {
        auto item = new QTableWidgetItem;
        item->setData(Qt::EditRole,eCallStatsList[i].callId_);
        table->setItem(i,0, item);
        table->setItem(i,1, new QTableWidgetItem(eCallStatsList[i].callSymbolName_));
        auto countItem = new QTableWidgetItem;
        countItem->setData(Qt::EditRole,eCallStatsList[i].count_);
        table->setItem(i,2, countItem);
        if(eCallStatsList[i].count_ != 0)
        {
            table->setItem(i,3, new moe::TimeTableWidgetItem(eCallStatsList[i].callAvg_));
            table->setItem(i,4, new moe::TimeTableWidgetItem(eCallStatsList[i].median_));
            table->setItem(i,5, new moe::TimeTableWidgetItem(eCallStatsList[i].standardDeviation_));
            table->setItem(i,6, new moe::TimeTableWidgetItem(eCallStatsList[i]._99thPercentile_));
            table->setItem(i,7, new moe::TimeTableWidgetItem(eCallStatsList[i]._95thPercentile_));
            table->setItem(i,8, new moe::TimeTableWidgetItem(eCallStatsList[i]._90thPercentile_));
        }
    }

    table->resizeColumnsToContents();
    table->resizeRowsToContents();
    int height = 0,width = 0;
    for (int j = 0; j < table->columnCount() ; ++j)
    {
        width += table->columnWidth(j)+6;
    }
    for (int k = 0; k < table->rowCount() ; ++k)
    {
        height += table->rowHeight(k)+3;
    }
    table->horizontalHeader()->setStretchLastSection(true);
    table->verticalHeader()->setStretchLastSection(true);
    table->verticalHeader()->hide();
    table->setSortingEnabled(true);
    table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    table->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    return table;
}

