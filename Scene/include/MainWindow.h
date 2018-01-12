#ifndef MainWindow_H
#define MainWindow_H

#include <QMainWindow>
#include <QtWidgets>

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

private slots:
    void open();
    void onZoomInPressed();
    void onZoomOutPressed();
    void resetPressed();
    void scrollUpPressed();
    void scrollDownPressed();
    void scrollRightPressed();
    void scrollLeftPressed();
    void scrollToNextEvent();

protected:
    void wheelEvent ( QWheelEvent * event );
private:

    moe::Renderable *sceneRootNode_;
    moe::Renderable *sequenceListNode_;
    moe::MeasureLine *measureLine_;

    //QVector<moe::SequenceDiagram*> sequenceList;
    //QVector<moe::Renderable*> renderables;
    //QVector<moe::Renderable*> lineRenderables;

    moe::Transform2D sceneTransformation = moe::Transform2D(1,0,0,1,0,0);
    //moe::Line *lineMeasureTest2;
    moe::SequenceDiagram *sequenceDiagram;
    //moe::Rect *topElement;
    //moe::Renderable *lineOfTopElement;
    //moe::Line *lineOfTopElement2;
    void visualizeThreads(const QVector<moe::MyThread> threads, qreal factor = 1);
    void createMenus();
    void createStatusBar();
    void createActions();
    void createToolbar();
    void drawScene();
    void render();
    void verticalScroll(qreal yOffset, qreal factor = 1);
    void verticalZoom(qreal yScale, qreal factor = 1); // ToDo added factor just incase (this method still needs more work)
    void scrollToNextEvent(const QVector<moe::MyThread> threads, qreal factor = 1); //ToDo implement it later as a button with Slots and Signals
    void scrollTo(qreal yOffset, qreal factor = 1);
    void zoomAndScrollTofirstEvent();
    void applySettings();
    void writeSettings();
    void loadFile(const QString &fileName);
    void closeEvent(QCloseEvent *event);
    void resizeEvent(QResizeEvent *event);


    QListView *filterListView;
    moe::SgxDatabaseStructure* db = 0;
   // Ui::MainWindow *ui;
    qreal factor_ = 1;
    qreal yScale_ = 1; //toDo better solution for zooming
    qreal yOffset_ = 0;
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
    QTabWidget* generateTabList();
    QListWidget* generateECallList();
    QListWidget* generateThreadList();
    QTabWidget *tabwidget_;
    QListWidget *eCallList_;
    QListWidget *threadList_;
    QDockWidget *threadDock_;
    QDockWidget *eCallDock_;
    /*
     * end of test
     */
    QAction *openAction_;
    QVector <moe::MyThread*> threadsFilterList;


};

#endif // MainWindow_H


