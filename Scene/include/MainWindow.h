#ifndef MainWindow_H
#define MainWindow_H

#include <QMainWindow>
#include <QtWidgets>

#include "Renderable.h"
#include "Rect.h"
#include "Line.h"
#include "SequenceDiagram.h"
#include "MeasureLine.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
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
private:

    //moe::Renderable *sceneRootNodeOffset_;
    moe::Renderable *sceneRootNode_;
    moe::Renderable *sequenceListNode_;
    moe::MeasureLine *measureLine_;

    //QVector<moe::SequenceDiagram*> sequenceList;
    //QVector<moe::Renderable*> renderables;
    //QVector<moe::Renderable*> lineRenderables;

    moe::Transform2D sceneTransformation = moe::Transform2D();
    //moe::Line *lineMeasureTest2;
    moe::SequenceDiagram *sequenceDiagram;
    //moe::Rect *topElement;
    //moe::Renderable *lineOfTopElement;
    //moe::Line *lineOfTopElement2;

    void createMenus();
    void createStatusBar();
    void createActions();
    void createToolbar();
    void drawScene();
    void render();
    void applySettings();
    void writeSettings();
    void loadFile(const QString &fileName);
    void closeEvent(QCloseEvent *event);
    void resizeEvent(QResizeEvent *event);

    Ui::MainWindow *ui;

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


