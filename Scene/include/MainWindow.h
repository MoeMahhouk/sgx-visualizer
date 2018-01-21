#ifndef MainWindow_H
#define MainWindow_H

#include <QMainWindow>
#include <QtWidgets>
#include <Filtering/IFilter.h>
#include <Filtering/SGX/ECallOCallFilter.h>
#include <Filtering/SGX/ThreadFilter.h>

#include "Rendering/Renderable.h"
#include "Rendering/Rect.h"
#include "Rendering/Line.h"
#include "Rendering/SequenceDiagram.h"
#include "Rendering/MeasureLine.h"
#include "Utility/Notifier.h"
#include "DataBase/SGX/SgxDatabaseStructure.h"

//ToDo found out that this is added automatically while programming by Qt platform to point at the automatically generated class from the drag and drop view
/*namespace Ui {
class MainWindow;
}*/

class MainWindow : public QMainWindow, public moe::Notifier
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:

    void unSelectAll(QListWidget *list);
    void selectAll(QListWidget *list);

private slots:
    void open();
    void applyFilter();
    void resetFilter();
    void resetPressed();

    void scrollRightPressed();
    void scrollLeftPressed();
    void scrollToNextEvent();

protected:
    void wheelEvent ( QWheelEvent * event ) override;
private:

    moe::Renderable *sceneRootNode_;
    moe::Renderable *sequenceListNode_;
    moe::MeasureLine *measureLine_ = 0;


    moe::Transform2D sceneTransformation = moe::Transform2D(1,0,0,1,0,0);

    void visualizeThreads(const QVector<moe::MyThread> threads, qreal factor = 1);
    void createMenus();
    void createStatusBar();
    void createActions();
    void createToolbar();
    void createFilterDocks();
    void generateGraphicsView();
    void render();
    void updateTraces();
    void verticalScroll(qreal yOffset, qreal factor = 1);
    void verticalZoom(qreal yScale, qreal factor = 1); // ToDo added factor just incase (this method still needs more work)
    void scrollToNextEvent(const QVector<moe::MyThread> threads, qreal factor = 1); //ToDo implement it later as a button with Slots and Signals
    void scrollTo(qreal yOffset, qreal factor = 1);
    void addZoomAndScrollOptions(QToolBar* toolbar);
    void zoomAndScrollTofirstEvent();
    void applySettings();
    void writeSettings();
    void loadFile(const QString &fileName);
    void closeEvent(QCloseEvent *event);
    void resizeEvent(QResizeEvent *event);
    void clearSequenceListNode();

    QListView *filterListView;
    moe::SgxDatabaseStructure* db = 0;
   // Ui::MainWindow *ui;
    qreal factor_ = 1;
    qreal yScale_ = 1; //toDo better solution for zooming
    qreal yOffset_ = 0;

    QWidget *viewArea_;
    QToolBar *viewToolbar_;
    QGraphicsView *view_;
    QGraphicsScene *scene_;
    QMenu *fileMenu_;
    QMenu *viewMenu_;
    QMenu *helpMenu_;
    QMenuBar *menuBar_;
    QToolBar *toolBar_;
    QStatusBar *statusBar_;


    /*
     * test stuff
     */
    QSet<int> chosenThreads;
    QSet<int> chosenEcalls;
    QSet<int> chosenOcalls;
    moe::IFilter *filter;
    QTabWidget* generateTabList();
    void generateECallList();
    void generateOCallList();
    void generateThreadList();
    void generateFilterControls();
    bool updateThreads();
    bool updateECalls();
    bool updateOCalls();
    void resetThreadsEcallsAndOcalls();
    void clearQList();

    QTabWidget *tabwidget_;
    QListWidget *eCallList_;
    QListWidget *oCallList_;
    QListWidget *threadList_;
    QDockWidget *threadDock_;
    QDockWidget *eCallDock_;
    QDockWidget *oCallDock_;
    QAction *threadFilterAction_;
    QAction *eCallFilterAction_;
    QAction *oCallFilterAction_;
    QAction *applyDockAction_;
    QWidget *filterControls_;

    /*
     * end of test
     */
    QAction *openAction_;

};

#endif // MainWindow_H


