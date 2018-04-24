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

/**
 * the main window calls that implements the Qt's Mainwindow widget and extends the Notifier interface for the events exchange system
 */
class MainWindow : public QMainWindow, public moe::Notifier
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    /**
     * uncheck all items of the QListWidget
     * @param list
     */
    void unSelectAll(QListWidget *list);
    /**
     * selects all ievem of the QListWidget
     * @param list
     */
    void selectAll(QListWidget *list);

private slots:

    /**
     * triggered by pressing the open button to load the chosen trace
     */
    void open();

    /**
     * triggered by pressing the open without visuals button to load the trace without visualising it (only for analysis purposes)
     */
    void openWithoutVisuals();

    /**
     * executes the chosen filters by clicking the apply filter button
     */
    void applyFilter();

    /**
     * resets all filters back to the first state by clicking the reset filter buttuon
     */
    void resetFilter();

    /**
     * resets all rendered objects back to the first standard state
     */
    void resetPressed();

    /**
     * shifts the rendered objects to the right side by either clicking the scroll right button or alt and mouse wheel together
     */
    void scrollRightPressed();

    /**
     * shifts the rendered objects to the left side by either clicking the scroll right button or alt and mouse wheel together
     */
    void scrollLeftPressed();

    /**
     * jumps to the direct next ecall event of the rendered objects by clicking the next ecall button
     */
    void scrollToNextEvent();

    /**
     * generates the ecall/ocall statistics by pressing the statistic button
     */
    void generateCallStatistics();

    /**
     * generates the ecall/ocall static and dynamic analysis by pressing the analysis button
     */
    void generateCallStaticAnalysis();
protected:
    /**
     * handles the mouse action events including the wheel movements
     * @param event
     */
    void wheelEvent ( QWheelEvent * event ) override;

private:
    /**
     * checks if there are threads without ecall events (empty from sgx events)
     * @return
     */
    bool checkEmptyThreads();

    /**
     * transform the list of threads including its children list of ecalls and ocalls into rendered objects and places them into the sequence list node
     * @param threads
     * @param factor
     */
    void visualizeThreads(const QVector<moe::MyThread> threads, qreal factor = 1);

    /**
     * generates the menu bars of the main window
     */
    void createMenus();

    /**
     * initialise the status bar of the main window
     */
    void createStatusBar();

    /**
     * initialises the available actions, such as open trace action and others
     */
    void createActions();

    /**
     * adds an extra tool bar for extra options if needed
     */
    void createToolbar();

    /**
     * generates the filter widgets on the left side of the screen
     */
    void createFilterDocks();

    /**
     * initialises the graphics view with the scene that renders the loaded trace
     */
    void generateGraphicsView();

    /**
     * updates the scene after each action, either navigation, trace opening or filter action
     */
    void render();

    /**
     * reads the opened trace  and loads all information onto the scene.
     */
    void updateTraces();

    /**
     * is responsible for vertical scrolling (Up and down)
     * @param yOffset
     * @param factor
     */
    void verticalScroll(qreal yOffset, qreal factor = 1);

    /**
     * zooms only in one Y coordination (axes)
     * @param yScale
     * @param factor
     */
    void verticalZoom(qreal yScale, qreal factor = 1); // ToDo added factor just incase

    /**
     * iterates through the thread list and finds the next ecall event to jump to
     * if not found it jumps back to the start
     * @param threads
     * @param factor
     */
    void scrollToNextEvent(const QVector<moe::MyThread> threads, qreal factor = 1);

    /**
     * this jumps(scrolls) to a specific time/location
     * @param yOffset
     * @param factor
     */
    void scrollTo(qreal yOffset, qreal factor = 1);

    /**
     * it adds the extra options above the scene widget
     * @param toolbar
     */
    void addZoomAndScrollOptions(QToolBar* toolbar);

    /**
     * searches for the first ecall event and last ended ecall events then zooms and scrolls such all rendered objects
     * are placed between the start and the end of the measure line
     */
    void zoomAndScrollTofirstEvent();

    /**
     * applys the window size after starting this tool
     */
    void applySettings();

    /**
     * saves the window resolution after exiting this tool
     */
    void writeSettings();

    /**
     * triggered by the open button, loads the file with the chosen file name as parameter
     * @param fileName
     */
    void loadFile(const QString &fileName);

    /**
     * triggered by closing the main window
     * @param event
     */
    void closeEvent(QCloseEvent *event);

    /**
     * triggered each time the window is resized and readjusts all widgets including the scene and its contents
     * @param event
     */
    void resizeEvent(QResizeEvent *event);

    /**
     * clears the sequence list node from all the rendered objects and deletes(frees) the pointers correctly
     */
    void clearSequenceListNode();

    moe::Renderable *sceneRootNode_;
    moe::Renderable *sequenceListNode_;


    moe::MeasureLine *measureLine_ = 0;

    moe::Transform2D sceneTransformation = moe::Transform2D(1,0,0,1,0,0);

    moe::SgxDatabaseStructure* db = 0;
    // Ui::MainWindow *ui;
    qreal factor_ = 1;
    qreal yScale_ = 1; //toDo better solution for zooming
    qreal scaleLineStep;

    qreal yOffset_ = 0;
    QWidget *viewArea_;
    QToolBar *viewToolbar_;
    QGraphicsView *view_;
    QGraphicsScene *scene_;
    QMenu *fileMenu_;
    QMenu *viewMenu_;
    QMenu *helpMenu_;
    QToolBar *toolBar_;
    QDialog *analysisDialig_ = 0;
    QDialog *statisticsDialog_ = 0;

    QSet<int> chosenThreads;
    QSet<int> chosenEcalls;
    QSet<int> chosenOcalls;
    QSet<int> chosenEnclaves;
    QPair<uint64_t ,uint64_t > chosenTimeFilter;
    moe::IFilter *filter;
    void generateEncalveList();
    void generateECallList();
    void generateOCallList();
    void generateThreadList();
    void generateFilterControls();
    bool updateThreads();
    bool updateECalls();
    bool updateOCalls();
    bool updateEnclaves();
    bool updateTimeFilter();
    uint64_t getTime(const QLineEdit &inputTime) const;
    void resetThreadsEcallsOcallsEnclavesAndTimeline();

    QLineEdit *startTimeFilter;
    QLineEdit *endTimeFilter;
    QListWidget *enclavesList_;
    QListWidget *eCallList_;
    QListWidget *oCallList_;
    QListWidget *threadList_;
    QDockWidget *threadDock_;
    QDockWidget *eCallDock_;
    QDockWidget *oCallDock_;
    QDockWidget *enclaveDock_;
    QDockWidget *timeDock_;
    QAction *threadFilterAction_;
    QAction *eCallFilterAction_;
    QAction *oCallFilterAction_;
    QAction *enclaveFilterAction_;
    QAction *loadCallStatistics;
    QAction *loadCallStaticAnalysis_;
    QAction *timeFilterAction_;
    QAction *applyDockAction_;
    QWidget *filterControls_;
    QAction *openAction_;
    QAction *openWithOutVisualisationAction_;
};

#endif // MainWindow_H


