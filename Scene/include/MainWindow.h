#ifndef MainWindow_H
#define MainWindow_H

#include <QMainWindow>
#include <QtWidgets>

#include "Renderable.h"
#include "Rect.h"
#include "Line.h"
#include "SequenceDiagram.h"
#include "MeasureLine.h"
#include "Notifier.h"
#include "SgxDatabaseStructure.h"

namespace Ui {
class MainWindow;
}

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
    void applySettings();
    void writeSettings();
    void loadFile(const QString &fileName);
    void closeEvent(QCloseEvent *event);
    void resizeEvent(QResizeEvent *event);

    moe::DataBaseManager* db = 0;
    Ui::MainWindow *ui;
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

    QAction *openAction_;



};

#endif // MainWindow_H


