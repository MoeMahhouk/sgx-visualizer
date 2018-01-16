

#include <Filtering/SGX/ThreadFilter.h>
#include "MainWindow.h"
#include "Utility/MathUtility.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent)
{
    createActions();
    createFilterDocks();
    createMenus();
    createToolbar();
    createStatusBar();
    applySettings();
    generateGraphicsView();

}

MainWindow::~MainWindow()
{
   // delete ui;
	delete sceneRootNode_;
    delete db;
   // delete filter;
}


void MainWindow::open()
{
    QString fileName = QFileDialog::getOpenFileName(this);
    if (!fileName.isEmpty())
        loadFile(fileName);
}


void MainWindow::wheelEvent(QWheelEvent *event) {

    if (event->modifiers() & Qt::ControlModifier) {
        qreal yScaleFactor = pow((double) 2, event->delta()/ 240.0);
        verticalZoom(yScaleFactor);
    } else {
        verticalScroll(moe::signum(event->delta())*40,factor_);
    }
}


void MainWindow::resetPressed()
{
    for (moe::Renderable* r: sequenceListNode_->children_)
    {
        moe::SequenceDiagram *s = static_cast<moe::SequenceDiagram*>(r);
        s->resetLineScales();
    }
    sequenceListNode_->setTransform(moe::Transform2D());
    yOffset_ = 0;
    yScale_ = 1;
    moe::ResetEvent e;
    notify(&e);
    render();
}


void MainWindow::scrollRightPressed()
{
    sequenceListNode_->setTransform(sequenceListNode_->getTransform() * moe::Transform2D(1,0,0,1,20,0));
    render();
}

void MainWindow::scrollLeftPressed()
{
    sequenceListNode_->setTransform(sequenceListNode_->getTransform() * moe::Transform2D(1,0,0,1,-20,0));
    render();
}

void MainWindow::scrollToNextEvent() {
    if(db)
    {
       // std::cerr << "data base is not null" << std::endl;
        scrollToNextEvent(db->getThreads_(),factor_);
    }
}

void MainWindow::createMenus()
{
    fileMenu_ = menuBar()->addMenu(tr("File"));
    fileMenu_->addAction(openAction_);

    viewMenu_ = menuBar()->addMenu(tr("View"));
    viewMenu_->addAction(threadFilterAction_);
    viewMenu_->addAction(eCallFilterAction_);
    viewMenu_->addAction(oCallFilterAction_);
    viewMenu_->addAction(applyDockAction_);


    menuBar()->addSeparator();
    helpMenu_ = menuBar()->addMenu(tr("Help"));

}

void MainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
}

void MainWindow::createActions()
{
    openAction_ = new QAction(tr("Open"), this);
    openAction_->setShortcuts(QKeySequence::Open);
    openAction_->setStatusTip(tr("Open an existing executable"));
    connect(openAction_,SIGNAL(triggered()), this, SLOT(open()));
}

void MainWindow::createToolbar()
{
    toolBar_ = addToolBar(tr("File"));
    toolBar_->addAction(openAction_);
}

void MainWindow::generateGraphicsView()
{
    viewArea_ = new QWidget();
    auto layout = new QVBoxLayout(viewArea_);
    viewToolbar_ = new QToolBar(viewArea_);
    addZoomAndScrollOptions(viewToolbar_);
    layout->addWidget(viewToolbar_);
    /*QPushButton* scrollUp = new QPushButton(tr("Scroll Up"), this);
    QPushButton* scrollDown = new QPushButton(tr("Scroll Down"), this);
    QPushButton* zoomIn = new QPushButton(tr("Zoom In"), this);
    QPushButton* zoomOut = new QPushButton(tr("Zoom Out") , this);
    QPushButton* reset = new QPushButton(tr("Reset"), this);
    QPushButton* scrollRight = new QPushButton(tr("Scroll Right"), this);
    QPushButton* scrollLeft = new QPushButton(tr("Scroll Left"), this);
    QPushButton* scrollToNextEventButton = new QPushButton(tr("Next Event"), this);*/

    /*reset->move(0,155);
    zoomOut->move(0,105);
    zoomIn->move(0,55);
    scrollUp->move(0,205);
    scrollDown->move(0,255);
    scrollRight->move(0,305);
    scrollLeft->move(0,355);
    scrollToNextEventButton->move(0,405);
    zoomIn->connect(zoomIn, SIGNAL(clicked()) , this, SLOT(onZoomInPressed()));
    zoomOut->connect(zoomOut, SIGNAL(clicked()), this, SLOT(onZoomOutPressed()));
    reset->connect(reset,SIGNAL(clicked()),this, SLOT(resetPressed()));
    scrollUp->connect(scrollUp,SIGNAL(clicked()), this,SLOT(scrollUpPressed()));
    scrollDown->connect(scrollDown,SIGNAL(clicked()), this,SLOT(scrollDownPressed()));
    scrollRight->connect(scrollRight,SIGNAL(clicked()), this, SLOT(scrollRightPressed()));
    scrollLeft->connect(scrollLeft,SIGNAL(clicked()), this, SLOT(scrollLeftPressed()));
    scrollToNextEventButton->connect(scrollToNextEventButton, SIGNAL(clicked()), this, SLOT(scrollToNextEvent()));*/
    scene_ = new QGraphicsScene(this);
    view_ = new QGraphicsView(scene_,this);
    view_->setRenderHint(QPainter::Antialiasing);
    view_->setGeometry(0, 0, this->rect().width()*0.8, this->rect().height()*0.8);
    view_->setFrameStyle(0);
    scene_->setSceneRect(view_->rect());
    sceneRootNode_ = new moe::EmptyRenderable();
    sequenceListNode_ = new moe::EmptyRenderable(moe::Transform2D(1,0,0,1,0,0));
    std::cout << "MainWindow 1" << std::endl;
    sceneRootNode_->children_.push_back(sequenceListNode_);

    layout->addWidget(view_);
    viewArea_->setLayout(layout);
    setCentralWidget(viewArea_);

    render();
}

void MainWindow::addZoomAndScrollOptions(QToolBar *toolbar)
{
    auto zoomLabel = new QLabel("Zoom Options: ");
    toolbar->addWidget(zoomLabel);
    QPushButton* reset = new QPushButton(tr("Reset"), toolbar);
    reset->connect(reset,SIGNAL(clicked()),this, SLOT(resetPressed()));
    toolbar->addWidget(reset);
    toolbar->addSeparator();

    auto scrollLabel = new QLabel("Scroll Options: ");
    toolbar->addWidget(scrollLabel);

    QPushButton* scrollLeft = new QPushButton(tr("Scroll Left <-"), toolbar);
    scrollLeft->connect(scrollLeft,SIGNAL(clicked()), this, SLOT(scrollLeftPressed()));
    toolbar->addWidget(scrollLeft);

    QPushButton* scrollRight = new QPushButton(tr("Scroll Right ->"), toolbar);
    scrollRight->connect(scrollRight,SIGNAL(clicked()), this, SLOT(scrollRightPressed()));
    toolbar->addWidget(scrollRight);

    QPushButton* scrollToNextEventButton = new QPushButton(tr("Next ECall"), toolbar);
    scrollToNextEventButton->connect(scrollToNextEventButton, SIGNAL(clicked()), this, SLOT(scrollToNextEvent()));
    toolbar->addWidget(scrollToNextEventButton);
    toolbar->setStyleSheet("QToolBar{spacing:5px;}");
}

void MainWindow::render()
{
    scene_->clear();
    scene_->setBackgroundBrush(Qt::lightGray);
    moe::SceneData data{scene_};
    sceneRootNode_->render(data, sceneTransformation);
    scene_->update();
    viewArea_->show();
    //view_->show(); ToDo replaced it with viewArea.show (needs testing a little bit)
}

void MainWindow::applySettings()
{
    QSettings settings("BachelorArbeit IBR", "SGX AnalysisTool");
    QSize size = settings.value("size", QSize(800, 600)).toSize();
    resize(size);
}


void MainWindow::writeSettings()
{
    QSettings settings("BachelorArbeit IBR", "SGX AnalysisTool");
    settings.setValue("size", size());
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    writeSettings();
    event->accept();
}

void MainWindow::resizeEvent(QResizeEvent* event)
{

    if (db)
    {
        measureLine_->setPixel_line_depth_(this->height() - 250);
        factor_ = (double)(this->height() - 250)/db->getProgramTotalTime();
        render();
    }
    /* buggy a little bit and needs more rework
    view_->setGeometry(0, 0, this->rect().width()*0.8, this->rect().height()*0.8);
    //view_->setFrameStyle(0);
    scene_->setSceneRect(view_->rect());
     */
    QMainWindow::resizeEvent(event);
    view_->update();
}

void MainWindow::loadFile(const QString& fileName)
{
    resetPressed();
    db = new moe::SgxDatabaseStructure(fileName);
    /*
     * testing window height
     */
    factor_ = (double)(this->height() - 250)/db->getProgramTotalTime();
    measureLine_ = new moe::MeasureLine(moe::Transform2D(1,0,0,1,scene_->sceneRect().x()+5,50),db->getProgramTotalTime(),this->height() - 250, 40);
    registerObserver(measureLine_);
    visualizeThreads(db->getThreads_(), factor_);
    sceneRootNode_->children_.push_back(measureLine_);
    zoomAndScrollTofirstEvent();
    generateThreadList();
    generateECallList();
    generateOCallList();
    render();
}


/**
 * visualize the threads and their transitions as sequence diagrams in the scene
 * @param threads
 * @param factor
 */
void MainWindow::visualizeThreads(const QVector<moe::MyThread> threads, qreal factor)
{
    for (int i = 0; i < threads.length() ; ++i)
    {
        moe::SequenceDiagram* thread = threads[i].toRenderable(factor);
        thread->setTransform(moe::Transform2D(1,0,0,1, scene_->sceneRect().x() + (135 * (i+1)), 30));
        sequenceListNode_->children_.push_back(thread);
    }
}

/**
 * jumps to the next event in consideration of their start time
 * @param threads
 * @param factor
 */
void MainWindow::scrollToNextEvent(const QVector<moe::MyThread> threads, qreal factor)
{
    if (threads.isEmpty())
        return;

    qreal currentTime = (yOffset_ * moe::signum(yOffset_)) / yScale_/1000;
    //uint64_t currentTime =(uint64_t) ((yOffset_ * moe::signum(yOffset_)) / yScale_);
    qreal new_yOffset = 0;
    QVector<uint64_t> nextEventStartTime;

    if (currentTime != 0)
    {
        for (int i = 0; i < threads.length() ; ++i)
        {
            for (int j = 0; j < threads[i].threadEcalls_.length() ; ++j)
            {
                if(threads[i].threadEcalls_[j]->start_time_/1000 > currentTime)
                {
                    nextEventStartTime.push_back(threads[i].threadEcalls_[j]->start_time_);
                    break;
                }
            }
        }
        if(!nextEventStartTime.isEmpty())
        {
            new_yOffset = nextEventStartTime[0];
            for (int i = 0; i < nextEventStartTime.length()  ; ++i) {
                if (new_yOffset > nextEventStartTime[i])
                {
                    new_yOffset = nextEventStartTime[i];
                }
            }
        }
    } else {
        new_yOffset = threads[0].threadEcalls_[0]->start_time_;
    }
    scrollTo(-new_yOffset * yScale_, factor);
    render();
}


/**
 * abstract function for vertical scall events
 * @param yOffset
 * @param factor
 */
void MainWindow::verticalScroll(qreal yOffset, qreal factor)
{
    sequenceListNode_->setTransform(sequenceListNode_->getTransform() * moe::Transform2D(1, 0, 0, 1, 0, yOffset));
    yOffset_ += (yOffset/factor);
    moe::ScrollEvent e = {yScale_, yOffset_};
    notify(&e);
    render();
}

/**
 * abstract function for vertical zoom events
 * @param yScale
 * @param factor
 */
void MainWindow::verticalZoom(qreal yScale, qreal factor)
{
    qreal oldYOffset = yOffset_;
    for (moe::Renderable* r: sequenceListNode_->children_)
    {
        moe::SequenceDiagram *s = static_cast<moe::SequenceDiagram*>(r);
        s->setLineScale(yScale);
    }
    yScale_ *= yScale;
    moe::ZoomEvent e = {yScale_, yOffset_};
    notify(&e);
    scrollTo(oldYOffset * yScale, factor_);
    render();
}

void MainWindow::scrollTo(qreal yOffset, qreal factor)
{
    qreal oldXCoordinate = sequenceListNode_->getTransform().getX(); // so that it wont reset the xCoordinate each time next Event is clicked
    sequenceListNode_->setTransform(moe::Transform2D(1,0,0,1,oldXCoordinate,yOffset * factor));
    yOffset_ = yOffset;     // so that, it jumps to the target location and doesnt added the targets location to the current offset
    moe::ScrollEvent e = {yScale_, yOffset_};
    notify(&e);
    //render();
}

/**
 * calculate the total program usage and scrolls to the first event and scales the whole process so that it will be
 * visualised according to the scale line
 */
void MainWindow::zoomAndScrollTofirstEvent()
{
    if(db && !db->getThreads_().isEmpty())
    {
        bool threadsEmpty = true;
        int lastNotEmptyThreadIndex;
        for (int i = 0; i < db->getThreads_().size(); ++i)
        {
            if (!db->getThreads_()[i].threadEcalls_.isEmpty())
            {
                lastNotEmptyThreadIndex = i;
                threadsEmpty = false;
            }
        }
        if (threadsEmpty) {
            return;
        }
        qreal yScaleNew;
        qreal startTimeOfFirstEvent;
        for (int j = 0; j < db->getThreads_().size(); ++j)
        {
            if (!db->getThreads_()[j].threadEcalls_.isEmpty())
            {
                startTimeOfFirstEvent = db->getThreads_()[j].threadEcalls_[0]->start_time_;
                break;
            }
        }

        int lastEcallIndex = db->getThreads_()[lastNotEmptyThreadIndex].threadEcalls_.length() - 1;
        lastEcallIndex = lastEcallIndex < 0 ? 0 : lastEcallIndex;
        qreal startTimeOfLastEvent = db->getThreads_()[lastNotEmptyThreadIndex].threadEcalls_[lastEcallIndex]->start_time_;
        qreal endTimeOfLastEvent = startTimeOfLastEvent + db->getThreads_()[lastNotEmptyThreadIndex].threadEcalls_[lastEcallIndex]->total_time_;
        yScaleNew = (qreal)db->getProgramTotalTime() / (endTimeOfLastEvent - startTimeOfFirstEvent);
        verticalZoom(yScaleNew,factor_);
        scrollTo(-startTimeOfFirstEvent * yScale_, factor_);
    }
}

void MainWindow::generateOCallList()
{
    if(db)
    {
        for (int i = 0; i < db->getOCallTypeList().size() ; ++i)
        {
            QString oCallItemName = db->getOCallTypeList()[i].symbol_name_;
            QListWidgetItem *eCallItem = new QListWidgetItem(oCallItemName, oCallList_);
            eCallItem->setFlags(eCallItem->flags() | Qt::ItemIsUserCheckable);
            eCallItem->setCheckState(Qt::Checked);
            chosenOcalls.insert(i);
        }
    }
}

bool MainWindow::updateOCalls()
{
    bool isChanged = false;
    for (int i = 0; i < oCallList_->count() ; ++i)
    {
        if(oCallList_->item(i)->checkState() == Qt::Unchecked && chosenOcalls.contains(i))
        {
            chosenOcalls.remove(i);
            isChanged = true;

        } else if(oCallList_->item(i)->checkState() == Qt::Checked && !chosenOcalls.contains(i))
        {
            chosenOcalls.insert(i);
            isChanged = true;
        }
    }
    return isChanged;}


void MainWindow::generateECallList()
{
    if(db)
    {
        for (int i = 0; i < db->getECallTypeList().size() ; ++i)
        {
            QString eCallItemName = db->getECallTypeList()[i].symbol_name_;
            QListWidgetItem *eCallItem = new QListWidgetItem(eCallItemName, eCallList_);
            eCallItem->setFlags(eCallItem->flags() | Qt::ItemIsUserCheckable);
            eCallItem->setCheckState(Qt::Checked);
            chosenEcalls.insert(i);
        }
    }
}

bool MainWindow::updateECalls() {
    bool isChanged = false;
    for (int i = 0; i < eCallList_->count() ; ++i)
    {
        if(eCallList_->item(i)->checkState() == Qt::Unchecked && chosenEcalls.contains(i))
        {
            chosenEcalls.remove(i);
            isChanged = true;

        } else if(eCallList_->item(i)->checkState() == Qt::Checked && !chosenEcalls.contains(i))
        {
            chosenEcalls.insert(i);
            isChanged = true;
        }
    }
    return isChanged;
}

void MainWindow::generateThreadList()
{
    if(db)
    {
        for (int i = 0; i <db->getThreads_().size() ; ++i)
        {
            QString threadItemName = "Thread ";
            threadItemName.append(QString::number(i));
            QListWidgetItem *threadItem = new QListWidgetItem(threadItemName, threadList_);
            threadItem->setFlags(threadItem->flags() | Qt::ItemIsUserCheckable);
            threadItem->setCheckState(Qt::Checked);
            chosenThreads.insert(i);
        }
    }
}

bool MainWindow::updateThreads()
{
    bool isChanged = false;
    for (int i = 0; i < threadList_->count() ; ++i)
    {
        /*if the thread got unchecked and still not removed from the chosen thread set,
         * then it get removed from the set and method returns at the end so that the filter can be applied on the
         * chosen threads and if the thread was rechecked then it should be readded and then filter will be applied :)
         */
        if(threadList_->item(i)->checkState() == Qt::Unchecked && chosenThreads.contains(i))
        {
            chosenThreads.remove(i);
            isChanged = true;

        } else if(threadList_->item(i)->checkState() == Qt::Checked && !chosenThreads.contains(i))
        {
            chosenThreads.insert(i);
            isChanged = true;
        }
    }
    return isChanged;
}

void MainWindow::createFilterDocks()
{
    threadDock_ = new QDockWidget(tr("Thread Filter"), this);
    threadDock_->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    threadList_ = new QListWidget();
    threadDock_->setWidget(threadList_);
    addDockWidget(Qt::LeftDockWidgetArea, threadDock_);
    threadFilterAction_ = threadDock_->toggleViewAction();
    connect(threadFilterAction_, SIGNAL(toggled(bool)), threadDock_, SLOT(setVisible(bool)));

    eCallDock_ = new QDockWidget(tr("ECall Filter"), this);
    eCallDock_->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    eCallList_ = new QListWidget();
    eCallDock_->setWidget(eCallList_);
    addDockWidget(Qt::LeftDockWidgetArea, eCallDock_);
    eCallFilterAction_ = eCallDock_->toggleViewAction();
    connect(eCallFilterAction_, SIGNAL(toggled(bool)), eCallDock_, SLOT(setVisible(bool)));


    oCallDock_ = new QDockWidget(tr("OCall Filter"), this);
    oCallDock_->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    oCallList_ = new QListWidget();
    oCallDock_->setWidget(oCallList_);
    addDockWidget(Qt::LeftDockWidgetArea,oCallDock_);
    oCallFilterAction_ = oCallDock_->toggleViewAction();
    connect(oCallFilterAction_, SIGNAL(toggled(bool)), oCallDock_, SLOT(setVisible(bool)));

    QDockWidget *applyDock = new QDockWidget(tr("Apply Filter"), this);
    generateFilterControls();
    applyDock->setWidget(filterControls_);
    applyDock->setMaximumHeight(70);
    addDockWidget(Qt::LeftDockWidgetArea, applyDock);
    applyDockAction_ = applyDock->toggleViewAction();
    connect(applyDockAction_, SIGNAL(toggled(bool)), applyDock, SLOT(setVisible(bool)));

}

void MainWindow::generateFilterControls()
{
    filterControls_ = new QWidget(this);
    auto layout = new QHBoxLayout();
    auto filterButton = new QPushButton(tr("Apply"), filterControls_);
    connect(filterButton, SIGNAL(released()), this, SLOT(applyFilter()));

    auto resetFilterButton = new QPushButton(tr("Reset"), filterControls_);
    connect(resetFilterButton, SIGNAL(clicked()), this, SLOT(resetFilter()));
    layout->addWidget(resetFilterButton);
    layout->addWidget(filterButton);
    filterControls_->setLayout(layout);
}

void MainWindow::applyFilter()
{
    bool updateScene = false;
    if(updateThreads())
    {
        updateScene = true;
        filter = new moe::ThreadFilter(db,chosenThreads.toList().toVector());
        filter->execute();
        delete filter;
    }

    if(updateECalls() || updateOCalls())
    {
        updateScene = true;
        filter = new moe::ECallFilter(db, chosenEcalls.toList().toVector(), chosenOcalls.toList().toVector());
        filter->execute();
        delete filter;
    }

    if(updateScene)
    {
        auto it = sequenceListNode_->children_.begin();
        while (it != sequenceListNode_->children_.end())
        {
            delete *it;
            it++;
        }
        sequenceListNode_->children_.clear();
        resetPressed();
        visualizeThreads(db->getThreads_(),factor_);
        zoomAndScrollTofirstEvent();
        render();
    }
}

void MainWindow::resetFilter()
{
    resetThreadsEcallsAndOcalls();
    applyFilter();
}

void MainWindow::resetThreadsEcallsAndOcalls()
{
    for (int i = 0; i < threadList_->count() ; ++i)
    {
        threadList_->item(i)->setCheckState(Qt::Checked);
    }
    for (int j = 0; j < eCallList_->count() ; ++j)
    {
        eCallList_->item(j)->setCheckState(Qt::Checked);
    }
}

