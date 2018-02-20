


#include <DataBase/SGX/SGXErrorCodes.h>
#include <Utility/TimeStamp.h>
#include "MainWindow.h"
#include "Utility/MathUtility.h"

//#define QCUSTOMPLOT_USE_LIBRARY
//#include "lib/qcustomplot.h"


MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent)
{
    createActions();
    createFilterDocks();
    createMenus();
    //createToolbar();
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
    {
        loadFile(fileName);
    }
}


void MainWindow::wheelEvent(QWheelEvent *event)
{//ToDo still buggy, does not recognize the real 0,0 coordination of the scene :/

    statusBar()->showMessage("Scale : "  + QString::number(yScale_,'f', 2));
    const qreal MEASURELINE_OFFSET = 50;
    QPointF mouseScenePos = view_->mapToScene(event->pos()) - (view_->mapFromScene(QPointF(0,MEASURELINE_OFFSET))*0.8) ;

    if(!scene_->sceneRect().contains(event->pos()) && db)
    {
        if (event->modifiers() & Qt::ControlModifier)
        {
            qreal yScaleFactor = pow((double) 2, event->delta()/ 240.0);
            qreal mouseSceneYBeforeZoom = mouseScenePos.y();
            qreal mouseSceneYAfterZoom = mouseScenePos.y() * yScaleFactor;
            qreal yOffsetTailDiff = ((this->height()*0.75) + ((yOffset_ * factor_)/yScale_)); // gives the tail of the objects in pixel coordination
            qreal oldYOffset = yOffset_;
            const qreal limit = (qreal)scaleLineStep/(qreal)(yScale_*yScaleFactor);
            if(limit/pow(10,9) > 1 || limit < 300 || mouseScenePos.y()/yScale_ > yOffsetTailDiff){
                return;
            }
            verticalZoom(yScaleFactor);
            scrollTo(oldYOffset * yScaleFactor, factor_);
            verticalScroll(mouseSceneYBeforeZoom - mouseSceneYAfterZoom, factor_);
        } else {
            verticalScroll(moe::signum(event->delta())*40,factor_);
        }
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

void MainWindow::scrollToNextEvent()
{
    if(db)
    {
        scrollToNextEvent(db->getThreads_(),factor_);
    }
}

void MainWindow::createMenus()
{
    fileMenu_ = menuBar()->addMenu(tr("File"));
    fileMenu_->addAction(openAction_);
    auto load = new QMenu("Load",fileMenu_);
    fileMenu_->addMenu(load);

    load->addAction(loadCallStatistics);
    load->addAction(loadCallStaticAnalysis_);


    viewMenu_ = menuBar()->addMenu(tr("View"));
    viewMenu_->addAction(timeFilterAction_);
    viewMenu_->addAction(threadFilterAction_);
    viewMenu_->addAction(eCallFilterAction_);
    viewMenu_->addAction(oCallFilterAction_);
    viewMenu_->addAction(enclaveFilterAction_);
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

    loadCallStatistics = new QAction(tr("Statistics"),this);
    loadCallStatistics->setStatusTip(tr("Generate E/OCalls Statistics for the loaded trace"));
    connect(loadCallStatistics, SIGNAL(triggered()), this, SLOT(generateCallStatistics()));
    loadCallStatistics->setDisabled(true);

    loadCallStaticAnalysis_ = new QAction(tr("Analysis"), this);
    loadCallStaticAnalysis_->setStatusTip(tr("Generate the Static/Dynamic Analysis For E/OCalls"));
    connect(loadCallStaticAnalysis_, SIGNAL(triggered()), this, SLOT(generateCallStaticAnalysis()));
    loadCallStaticAnalysis_->setDisabled(true);
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
    scene_ = new QGraphicsScene(this);
    view_ = new QGraphicsView(scene_,this);
    view_->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    //view_->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    view_->setMouseTracking(true);
    view_->setInteractive(true);
    view_->setFrameStyle(0);
    scene_->setSceneRect(view_->rect());
    sceneRootNode_ = new moe::EmptyRenderable();
    sequenceListNode_ = new moe::EmptyRenderable();
    std::cout << "MainWindow 1" << std::endl;
    sceneRootNode_->children_.push_back(sequenceListNode_);
    layout->addWidget(view_);
    viewArea_->setLayout(layout);
    viewArea_->show();
    setCentralWidget(viewArea_);
    //ToDo testing the initialize update methods
    //render();
}

void MainWindow::addZoomAndScrollOptions(QToolBar *toolbar)
{
    auto zoomLabel = new QLabel("Zoom Options: ");
    toolbar->addWidget(zoomLabel);
    auto* reset = new QPushButton(tr("Reset"), toolbar);
    reset->connect(reset,SIGNAL(clicked()),this, SLOT(resetPressed()));
    toolbar->addWidget(reset);
    toolbar->addSeparator();


    auto scrollLabel = new QLabel("Scroll Options: ");
    toolbar->addWidget(scrollLabel);

    auto* scrollLeft = new QPushButton(tr("Scroll Left <-"), toolbar);
    scrollLeft->connect(scrollLeft,SIGNAL(clicked()), this, SLOT(scrollLeftPressed()));
    scrollLeft->setShortcut(QKeySequence::MoveToPreviousChar);
    toolbar->addWidget(scrollLeft);

    auto* scrollRight = new QPushButton(tr("Scroll Right ->"), toolbar);
    scrollRight->connect(scrollRight,SIGNAL(clicked()), this, SLOT(scrollRightPressed()));
    scrollRight->setShortcut(QKeySequence::MoveToNextChar);
    toolbar->addWidget(scrollRight);

    auto* scrollToNextEventButton = new QPushButton(tr("Next ECall"), toolbar);
    scrollToNextEventButton->connect(scrollToNextEventButton, SIGNAL(clicked()), this, SLOT(scrollToNextEvent()));
    scrollToNextEventButton->setShortcut(QKeySequence::FindNext);
    scrollToNextEventButton->setStatusTip("Scroll to next ECall");
    toolbar->addWidget(scrollToNextEventButton);
    toolbar->setStyleSheet("QToolBar{spacing:5px;}");
    toolbar->addSeparator();

    toolbar->addAction(loadCallStatistics);
    toolbar->addSeparator();

    toolbar->addAction(loadCallStaticAnalysis_);
}

void MainWindow::render()
{
    //toDo testing the new update initialize method
    //scene_->clear();
    /*for(auto i: scene_->items()) {
        scene_->removeItem(i);
    }*/
    scene_->setBackgroundBrush(Qt::white);
    moe::SceneData data{scene_};
    sceneRootNode_->render(data,sceneTransformation);
    //update();
    scene_->update();
    view_->update();
    //viewArea_->show();
    //view_->show(); //ToDo replaced it with viewArea.show (needs testing a little bit)
}

void MainWindow::applySettings()
{
    QSettings settings("BachelorArbeit IBR", "SGX AnalysisTool");
    QSize size = settings.value("size", QSize(800, 600)).toSize();
    std::cerr << "size is " << size.width() << " "<< size.height() << std::endl;
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
    QMainWindow::resizeEvent(event);
    viewArea_->setGeometry(0, 0, this->rect().width()*0.8 - 50 , this->rect().height()*0.8);
    view_->setGeometry(0, 0, this->rect().width()*0.8 - 50, this->rect().height()*0.8);
    view_->setFrameStyle(0);
    scene_->setSceneRect(view_->rect());
    setCentralWidget(viewArea_);
    /*
     * a little bit buggy and should be refined later when writing the view seperate class :/
     * ToDo sometimes qt resize causes some shit scrolling issues -_-
     */
    if (db)
    {
        qreal oldYoffset = yOffset_;
        qreal oldYscale = yScale_;
        qreal oldXoffset = sequenceListNode_->getTransform().getX();
        resetPressed();
        if(measureLine_)
        {
            sceneRootNode_->children_.removeAll(measureLine_);
            unRegisterObersver(measureLine_);
            delete measureLine_;
        }
        measureLine_ = new moe::MeasureLine(moe::Transform2D(1,0,0,1,scene_->sceneRect().x()+5,50),db->getProgramTotalTime(),this->height() * 0.75, 40);
        registerObserver(measureLine_);
        sceneRootNode_->children_.push_back(measureLine_);
        //measureLine_->setPixel_line_depth_(this->height()*0.75);
        factor_ = (this->height()*0.75)/db->getProgramTotalTime();
        clearSequenceListNode();
        scene_->clear();
        visualizeThreads(db->getThreads_(),factor_);
        moe::SceneData data{scene_};
        sceneRootNode_->initialize(data, sceneTransformation);
        sequenceListNode_->setTransform(moe::Transform2D(1,0,0,1,oldXoffset,0));
        verticalZoom(oldYscale,factor_);
        scrollTo(oldYoffset,factor_);
        render();
    }
    //view_->update();
}

void MainWindow::loadFile(const QString& fileName)
{
    if(fileName.contains(".db")) {
        if (db) {
            delete db;
        }
        db = new moe::SgxDatabaseStructure(fileName);
        loadCallStatistics->setEnabled(true);
        loadCallStaticAnalysis_->setEnabled(true);
        //QThread thread1(this) ;
        //thread1.
        //std::thread t1(&MainWindow::updateTraces,this);
        //t1.join();
        updateTraces();
        setWindowTitle(fileName);
    }
}


/**
 * visualize the threads and their transitions as sequence diagrams in the scene
 * @param threads
 * @param factor
 */
void MainWindow::visualizeThreads(const QVector<moe::MyThread> threads, qreal factor)
{
    //ToDo ask nico if the other threads should still out of the list or not
    int counter = 0;
    for (int i = 0; i < threads.length() ; ++i)
    {
        if(!threads[i].threadEcalls_.isEmpty()) {
            moe::SequenceDiagram* thread = threads[i].toRenderable(factor);
            thread->setTransform(moe::Transform2D(1,0,0,1, scene_->sceneRect().x() + (135 * (counter + 1)), 30));
            sequenceListNode_->children_.push_back(thread);
            counter++;
        }
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
        yOffset_ = 1;
        scrollToNextEvent();
        return;
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
    qreal pixelScroll=0;
    if (yOffset_ + (yOffset/factor) >= 0)
    {
        qreal oldXCoordinate = sequenceListNode_->getTransform().getX();
        sequenceListNode_->setTransform(moe::Transform2D(1,0,0,1,oldXCoordinate,0));
        yOffset_ = 0;
    } else if (moe::signum(yOffset_)*(yOffset_ + (yOffset/factor)) <= (db->getProgramTotalTime() * yScale_)) {
        pixelScroll = yOffset;
        sequenceListNode_->setTransform(sequenceListNode_->getTransform() * moe::Transform2D(1, 0, 0, 1, 0, pixelScroll));
        yOffset_ += (yOffset/factor);
    } else {
       return;
    }
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
	(void)factor;
    //qreal oldYOffset = yOffset_;
    for (moe::Renderable* r: sequenceListNode_->children_)
    {
        moe::SequenceDiagram *s = static_cast<moe::SequenceDiagram*>(r);
        s->setLineScale(yScale);
    }
    yScale_ *= yScale;
    moe::ZoomEvent e = {yScale_, yOffset_};
    notify(&e);
    //scrollTo(oldYOffset * yScale, factor_);
    //render();
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
        bool threadsChildrenEmpty = true;
        qreal yScaleNew;
        qreal startTimeOfFirstEvent;
        for (int j = 0; j < db->getThreads_().size(); ++j)
        {
            if (!db->getThreads_()[j].threadEcalls_.isEmpty())
            {
                startTimeOfFirstEvent = db->getThreads_()[j].threadEcalls_[0]->start_time_;
                threadsChildrenEmpty = false;
                break;
            }
        }
        if (threadsChildrenEmpty)
        {
            return;
        }

        int lastEcallIndex;
        qreal startTimeOfLastEvent;
        qreal endTimeOfLastEvent;
        qreal endTimeOfLongestEcall = 0;
        for (int i = 0; i < db->getThreads_().size(); ++i)
        {
            if (!db->getThreads_()[i].threadEcalls_.isEmpty())
            {
                lastEcallIndex = db->getThreads_()[i].threadEcalls_.length() - 1;
                lastEcallIndex = lastEcallIndex < 0 ? 0 : lastEcallIndex;
                startTimeOfLastEvent = db->getThreads_()[i].threadEcalls_[lastEcallIndex]->start_time_;
                endTimeOfLastEvent = startTimeOfLastEvent + db->getThreads_()[i].threadEcalls_[lastEcallIndex]->total_time_;
                if (endTimeOfLongestEcall < endTimeOfLastEvent)
                {
                    endTimeOfLongestEcall = endTimeOfLastEvent;
                }
            }
        }
        yScaleNew = (qreal)db->getProgramTotalTime() / (endTimeOfLongestEcall - startTimeOfFirstEvent);
        verticalZoom(yScaleNew,factor_);
        scrollTo(-startTimeOfFirstEvent * yScale_, factor_);
    }
}

void MainWindow::generateOCallList()
{
    oCallList_->clear();
    if(db)
    {
        for (int i = 0; i < db->getOCallTypeList().size() ; ++i)
        {
            QString oCallItemName = db->getOCallTypeList()[i].symbol_name_;
            auto *eCallItem = new QListWidgetItem(oCallItemName, oCallList_);
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
    eCallList_->clear();
    if(db)
    {
        for (int i = 0; i < db->getECallTypeList().size() ; ++i)
        {
            QString eCallItemName = db->getECallTypeList()[i].symbol_name_;
            auto *eCallItem = new QListWidgetItem(eCallItemName, eCallList_);
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
    threadList_->clear();
    if(db)
    {
        for (int i = 0; i <db->getThreads_().size() ; ++i)
        {
            QString threadItemName = "Thread ";
            threadItemName.append(QString::number(i));
            auto *threadItem = new QListWidgetItem(threadItemName, threadList_);
            threadItem->setFlags(threadItem->flags() | Qt::ItemIsUserCheckable);
            threadItem->setCheckState(db->getThreads_()[i].threadEcalls_.isEmpty() ? Qt::Unchecked :Qt::Checked); //ToDo here we checked out the empty threads
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

void MainWindow::generateEncalveList()
{
    enclavesList_->clear();
    if(db)
    {
        for (auto e : db->getEnclavesMap().keys()) {
            QString enclaveName = db->getEnclavesMap().value(e);
            auto *enclaveItem = new QListWidgetItem(enclaveName, enclavesList_);
            enclaveItem->setFlags(enclaveItem->flags() | Qt::ItemIsUserCheckable);
            enclaveItem->setCheckState(Qt::Checked);
            chosenEnclaves.insert(e);
        }
    }
}

bool MainWindow::updateEnclaves()
{
    bool isChanged = false;
    int counter = 0; //because the QMap has only the enclaves that really were used in db and not all kinds of enclaves that exists
    for (auto e : db->getEnclavesMap().keys()) {
        if(enclavesList_->item(counter)->checkState() == Qt::Unchecked && chosenEnclaves.contains(e))
        {
            chosenEnclaves.remove(e);
            isChanged = true;

        } else if(enclavesList_->item(counter)->checkState() == Qt::Checked && !chosenEnclaves.contains(e))
        {
            chosenEnclaves.insert(e);
            isChanged = true;
        }
        counter++;
    }
    return isChanged;
}

uint64_t MainWindow::getTime(const QLineEdit &inputTime) const
{
    if (!inputTime.text().isEmpty())
    {
        return inputTime.text().toULong();
    } else {
        return 0;
    }
}

bool MainWindow::updateTimeFilter()
{
    bool isChanged = false;
    if(db)
    {
        uint64_t  programStartTime = db->getProgramStartTime();
        uint64_t start_time = getTime(*startTimeFilter);
        //start_time != 0 ? chosenTimeFilter.first = start_time + db->getProgramStartTime() : chosenTimeFilter.first = db->getProgramStartTime();
        chosenTimeFilter.first = start_time + programStartTime;
        uint64_t end_time = getTime(*endTimeFilter);
        end_time != 0 ? chosenTimeFilter.second = end_time + db->getProgramStartTime() : chosenTimeFilter.second = db->getProgramEndTime();
        //chosenTimeFilter.second = end_time + programStartTime;
        if (start_time || end_time)
        {
            isChanged = true;
            startTimeFilter->clear();
            endTimeFilter->clear();
        }
    }
    return isChanged;
}


void MainWindow::createFilterDocks()
{
    timeDock_ = new QDockWidget(tr("Time Filter"),this);
    auto *timeDockWidget = new QWidget();
    timeDock_->setMinimumWidth(395);
    timeDock_->setMaximumHeight(100);
    timeDock_->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    auto timeFilterLayout = new QHBoxLayout();
    //QValidator *timeValidator = new Qvalidator
    startTimeFilter = new QLineEdit(timeDock_);
    startTimeFilter->setPlaceholderText(tr("From ECall Starttime (ns)"));
    endTimeFilter = new QLineEdit(timeDock_);
    endTimeFilter->setPlaceholderText(tr("To ECall Starttime (ns)"));
    timeFilterLayout->addWidget(startTimeFilter);
    timeFilterLayout->addWidget(endTimeFilter);
    timeDockWidget->setLayout(timeFilterLayout);
    timeDock_->setWidget(timeDockWidget);
    addDockWidget(Qt::LeftDockWidgetArea, timeDock_);
    timeFilterAction_ = timeDock_->toggleViewAction();
    connect(timeFilterAction_, SIGNAL(toggled(bool)), timeDock_ , SLOT(setVisible(bool)));



    //thread dock which will have double layout later
    threadDock_ = new QDockWidget(tr("Thread Filter"), this);
    threadDock_->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    threadList_ = new QListWidget();
    //multiThreadDockWidget has 2 layouts, the first one is the main one vertical layout and has another widget which has the horizontal layout for the buttons
    auto *multiThreadDockWidget = new QWidget();
    auto multiThreadWidgetLayout = new QVBoxLayout();
    multiThreadWidgetLayout->addWidget(threadList_);
    //multiThreadButtonWidget has the horizontal layout and the 2 push buttons
    auto *multiThreadButtonWidget = new QWidget();
    auto *multiThreadButtonLayout = new QHBoxLayout();
    auto selectAllThread = new QPushButton(tr("Select All"), this);
    connect(selectAllThread, &QPushButton::pressed, [this]() { selectAll(threadList_);});
    auto unSelectAllThread = new QPushButton(tr("Unselect All"), this);
    connect(unSelectAllThread, &QPushButton::pressed, [this]() { unSelectAll(threadList_);});
    multiThreadButtonLayout->addWidget(selectAllThread);
    multiThreadButtonLayout->addWidget(unSelectAllThread);
    multiThreadButtonWidget->setLayout(multiThreadButtonLayout);
    multiThreadWidgetLayout->addWidget(multiThreadButtonWidget);
    multiThreadDockWidget->setLayout(multiThreadWidgetLayout);
    threadDock_->setWidget(multiThreadDockWidget);
    addDockWidget(Qt::LeftDockWidgetArea, threadDock_);
    threadFilterAction_ = threadDock_->toggleViewAction();
    connect(threadFilterAction_, SIGNAL(toggled(bool)), threadDock_, SLOT(setVisible(bool)));


    enclaveDock_ = new QDockWidget(tr("Enclave Filter"), this);
    enclaveDock_->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    enclavesList_ = new QListWidget();
    //multiECallDockWidget has 2 layouts, the first one is the main one vertical layout and has another widget which has the horizontal layout for the buttons
    auto *multiEnclaveDockWidget = new QWidget();
    auto multiEnclaveWidgetLayout = new QVBoxLayout();
    multiEnclaveWidgetLayout->addWidget(enclavesList_);
    //multiECallButtonWidget has the horizontal layout and the 2 push buttons
    auto *multiEnclaveButtonWidget = new QWidget();
    auto *multiEnclaveButtonLayout = new QHBoxLayout();
    auto selectAllEnclaves = new QPushButton(tr("Select All"), this);
    connect(selectAllEnclaves, &QPushButton::pressed, [this]() { selectAll(enclavesList_);});
    auto unSelectAllEnclaves = new QPushButton(tr("Unselect All"), this);
    connect(unSelectAllEnclaves, &QPushButton::pressed, [this]() { unSelectAll(enclavesList_);});
    multiEnclaveButtonLayout->addWidget(selectAllEnclaves);
    multiEnclaveButtonLayout->addWidget(unSelectAllEnclaves);
    multiEnclaveButtonWidget->setLayout(multiEnclaveButtonLayout);
    multiEnclaveWidgetLayout->addWidget(multiEnclaveButtonWidget);
    multiEnclaveDockWidget->setLayout(multiEnclaveWidgetLayout);
    enclaveDock_->setWidget(multiEnclaveDockWidget);
    addDockWidget(Qt::LeftDockWidgetArea, enclaveDock_);
    enclaveFilterAction_ = enclaveDock_->toggleViewAction();
    connect(enclaveFilterAction_, SIGNAL(toggled(bool)), enclaveDock_, SLOT(setVisible(bool)));



    eCallDock_ = new QDockWidget(tr("ECall Filter"), this);
    eCallDock_->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    eCallList_ = new QListWidget();
    //multiECallDockWidget has 2 layouts, the first one is the main one vertical layout and has another widget which has the horizontal layout for the buttons
    auto *multiECallDockWidget = new QWidget();
    auto multiECallWidgetLayout = new QVBoxLayout();
    multiECallWidgetLayout->addWidget(eCallList_);
    //multiECallButtonWidget has the horizontal layout and the 2 push buttons
    auto *multiECallButtonWidget = new QWidget();
    auto *multiECallButtonLayout = new QHBoxLayout();
    auto selectAllECalls = new QPushButton(tr("Select All"), this);
    connect(selectAllECalls, &QPushButton::pressed, [this]() { selectAll(eCallList_);});
    auto unSelectAllECalls = new QPushButton(tr("Unselect All"), this);
    connect(unSelectAllECalls, &QPushButton::pressed, [this]() { unSelectAll(eCallList_);});
    multiECallButtonLayout->addWidget(selectAllECalls);
    multiECallButtonLayout->addWidget(unSelectAllECalls);
    multiECallButtonWidget->setLayout(multiECallButtonLayout);
    multiECallWidgetLayout->addWidget(multiECallButtonWidget);
    multiECallDockWidget->setLayout(multiECallWidgetLayout);
    eCallDock_->setWidget(multiECallDockWidget);
    addDockWidget(Qt::LeftDockWidgetArea, eCallDock_);
    eCallFilterAction_ = eCallDock_->toggleViewAction();
    connect(eCallFilterAction_, SIGNAL(toggled(bool)), eCallDock_, SLOT(setVisible(bool)));


    oCallDock_ = new QDockWidget(tr("OCall Filter"), this);
    oCallDock_->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    oCallList_ = new QListWidget();
    //multiOCallDockWidget has 2 layouts, the first one is the main one vertical layout and has another widget which has the horizontal layout for the buttons
    auto *multiOCallDockWidget = new QWidget();
    auto multiOCallWidgetLayout = new QVBoxLayout();
    multiOCallWidgetLayout->addWidget(oCallList_);
    //multiOCallButtonWidget has the horizontal layout and the 2 push buttons
    auto *multiOCallButtonWidget = new QWidget();
    auto *multiOCallButtonLayout = new QHBoxLayout();
    auto selectAllOCalls = new QPushButton(tr("Select All"), this);
    connect(selectAllOCalls, &QPushButton::pressed, [this]() { selectAll(oCallList_);});
    auto unSelectAllOCalls = new QPushButton(tr("Unselect All"), this);
    connect(unSelectAllOCalls, &QPushButton::pressed, [this]() { unSelectAll(oCallList_);});
    multiOCallButtonLayout->addWidget(selectAllOCalls);
    multiOCallButtonLayout->addWidget(unSelectAllOCalls);
    multiOCallButtonWidget->setLayout(multiOCallButtonLayout);
    multiOCallWidgetLayout->addWidget(multiOCallButtonWidget);
    multiOCallDockWidget->setLayout(multiOCallWidgetLayout);
    oCallDock_->setWidget(multiOCallDockWidget);
    addDockWidget(Qt::LeftDockWidgetArea,oCallDock_);
    oCallFilterAction_ = oCallDock_->toggleViewAction();
    connect(oCallFilterAction_, SIGNAL(toggled(bool)), oCallDock_, SLOT(setVisible(bool)));

    auto *applyDock = new QDockWidget(tr("Apply Filter"), this);
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
    if(!db)
        return;
    if(updateThreads() | updateECalls() | updateOCalls() | updateEnclaves() | updateTimeFilter())
    {
        updateScene = true;
        filter = new moe::ThreadFilter(db,chosenThreads.toList().toVector());
        filter->execute();
        delete filter;
        filter  = new moe::ECallOCallFilter(db,chosenEcalls.toList().toVector(), chosenOcalls.toList().toVector(),
                                            chosenEnclaves.toList().toVector(), chosenTimeFilter);
        filter->execute();
        delete filter;
    }

    if(updateScene)
    {
        resetPressed(); //reset the scales and offsets
        clearSequenceListNode(); //delete all renderables without leak
        scene_->clear(); //clear the scene so that no duplication
        visualizeThreads(db->getThreads_(),factor_); //load the filtered objects
        moe::SceneData data{scene_};
        sceneRootNode_->initialize(data, sceneTransformation); //reintialize the objects and add them to the scene
        zoomAndScrollTofirstEvent();
        render(); //render the objects to their right places

    }
}

void MainWindow::resetFilter()
{
    resetThreadsEcallsOcallsEnclavesAndTimeline();
    applyFilter();
}

void MainWindow::resetThreadsEcallsOcallsEnclavesAndTimeline()
{
    for (int i = 0; i < threadList_->count() ; ++i)
    {
        threadList_->item(i)->setCheckState(Qt::Checked);
    }
    for (int j = 0; j < eCallList_->count() ; ++j)
    {
        eCallList_->item(j)->setCheckState(Qt::Checked);
    }
    for (int k = 0; k < oCallList_->count() ; ++k)
    {
        oCallList_->item(k)->setCheckState(Qt::Checked);
    }
    for (int l = 0; l < enclavesList_->count() ; ++l)
    {
        enclavesList_->item(l)->setCheckState(Qt::Checked);
    }
    if(db)
    {
        endTimeFilter->setText(QString::number(db->getProgramEndTime()));
    }
}

void MainWindow::unSelectAll(QListWidget *list)
{
    for (int i = 0; i < list->count() ; ++i)
    {
        list->item(i)->setCheckState(Qt::Unchecked);
    }
}

void MainWindow::selectAll(QListWidget *list)
{
    for (int i = 0; i < list->count() ; ++i)
    {
        list->item(i)->setCheckState(Qt::Checked);
    }
}

void MainWindow::clearSequenceListNode()
{
    auto it = sequenceListNode_->children_.begin();
    while (it != sequenceListNode_->children_.end())
    {
        delete *it;
        it++;
    }
    sequenceListNode_->children_.clear();
}

void MainWindow::updateTraces() {
    resetPressed();
    clearSequenceListNode();
    scene_->clear();
    if(measureLine_)
    {
        sceneRootNode_->children_.removeAll(measureLine_);
        unRegisterObersver(measureLine_);
        delete measureLine_;
    }
    /*
     * testing window height
     */
    factor_ = (this->height() * 0.75) / db->getProgramTotalTime();
    measureLine_ = new moe::MeasureLine(moe::Transform2D(1,0,0,1,scene_->sceneRect().x()+5,50),db->getProgramTotalTime(),this->height() * 0.75, 40);
    scaleLineStep = db->getProgramTotalTime()/measureLine_->getNumOfScaleLines();
    registerObserver(measureLine_);
    sceneRootNode_->children_.push_back(measureLine_);
    generateThreadList();
    generateECallList();
    generateOCallList();
    generateEncalveList();
    visualizeThreads(db->getThreads_(), factor_);
    moe::SceneData data{scene_};
    sceneRootNode_->initialize(data, sceneTransformation);
    zoomAndScrollTofirstEvent();
    render();

}

QTableWidget *MainWindow::loadOCallStats()
{
    const QVector<moe::CallStatistics> &oCallStatsList = db->getOcallStatistics();
    auto table = new QTableWidget();
    table->setRowCount(oCallStatsList.size());
    table->setColumnCount(9);

    table->setSortingEnabled(false);
    table->setHorizontalHeaderLabels(QString("ID;Call Name;Count;Average;Median;Standard Deviation;99th Percentile;95th Percentile;90th Percentile").split(";"));
    for (int i = 0; i < table->rowCount(); ++i)
    {
        auto item = new QTableWidgetItem;
        item->setData(Qt::EditRole,oCallStatsList[i].callId_);
        table->setItem(i,0, item);
        //table->setItem(i,0, new QTableWidgetItem(QString::number(oCallStatsList[i].callId_)));
        table->setItem(i,1, new QTableWidgetItem(oCallStatsList[i].callSymbolName_));
        table->setItem(i,2, new QTableWidgetItem(QString::number(oCallStatsList[i].count_,'f',0)));
        if(oCallStatsList[i].count_ != 0)
        {
            /*auto testItem = new QTableWidgetItem;
            moe::TimeStamp test = moe::TimeStamp(QString::number(oCallStatsList[i].callAvg_,'f',0));
            testItem->setData(Qt::EditRole,test);
            table->setItem(i,3,testItem);*/
            table->setItem(i,3, new QTableWidgetItem(moe::checkTimeUnit(oCallStatsList[i].callAvg_,0) + " (" + QString::number(oCallStatsList[i].callAvg_,'f',0) + " ns)"));
            table->setItem(i,4, new QTableWidgetItem(moe::checkTimeUnit(oCallStatsList[i].median_,0) + " (" + QString::number(oCallStatsList[i].median_,'f',0) + " ns)"));
            table->setItem(i,5, new QTableWidgetItem(moe::checkTimeUnit(oCallStatsList[i].standardDeviation_,0) + " (" + QString::number(oCallStatsList[i].standardDeviation_,'f',0)+ " ns)"));
            table->setItem(i,6, new QTableWidgetItem(moe::checkTimeUnit(oCallStatsList[i]._99thPercentile_,0) + " (" + QString::number(oCallStatsList[i]._99thPercentile_,'f',0)+ " ns)"));
            table->setItem(i,7, new QTableWidgetItem(moe::checkTimeUnit(oCallStatsList[i]._95thPercentile_,0) + " (" + QString::number(oCallStatsList[i]._95thPercentile_,'f',0)+ " ns)"));
            table->setItem(i,8, new QTableWidgetItem(moe::checkTimeUnit(oCallStatsList[i]._90thPercentile_,0) + " (" + QString::number(oCallStatsList[i]._90thPercentile_,'f',0)+ " ns)"));
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

QTableWidget *MainWindow::loadECallStats()
{
    const QVector<moe::CallStatistics> &eCallStatsList = db->getEcallStatistics();

    auto table = new QTableWidget();
    table->setRowCount(eCallStatsList.size());
    table->setColumnCount(9);

    table->setSortingEnabled(false);
    table->setHorizontalHeaderLabels(QString("ID;Call Name;Count;Average;Median;Standard Deviation;99th Percentile;95th Percentile;90th Percentile").split(";"));
    for (int i = 0; i < table->rowCount(); ++i)
    {
        //moe::AbstractTimeUnit itemInfo;
        auto item = new QTableWidgetItem;
        item->setData(Qt::EditRole,eCallStatsList[i].callId_);
        table->setItem(i,0, item);
        //table->setItem(i,0, new QTableWidgetItem(QString::number(eCallStatsList[i].callId_)));
        table->setItem(i,1, new QTableWidgetItem(eCallStatsList[i].callSymbolName_));
        table->setItem(i,2, new QTableWidgetItem(QString::number(eCallStatsList[i].count_,'f',0)));
        table->setItem(i,3, new QTableWidgetItem(moe::checkTimeUnit(eCallStatsList[i].callAvg_,0) + " (" + QString::number(eCallStatsList[i].callAvg_,'f',0)+ " ns)"));
        table->setItem(i,4, new QTableWidgetItem(moe::checkTimeUnit(eCallStatsList[i].median_,0) + " (" + QString::number(eCallStatsList[i].median_,'f',0)+ " ns)"));
        table->setItem(i,5, new QTableWidgetItem(moe::checkTimeUnit(eCallStatsList[i].standardDeviation_,0) + " (" + QString::number(eCallStatsList[i].standardDeviation_,'f',0)+ " ns)"));
        table->setItem(i,6, new QTableWidgetItem(moe::checkTimeUnit(eCallStatsList[i]._99thPercentile_,0) + " (" + QString::number(eCallStatsList[i]._99thPercentile_,'f',0)+ " ns)"));
        table->setItem(i,7, new QTableWidgetItem(moe::checkTimeUnit(eCallStatsList[i]._95thPercentile_,0) + " (" + QString::number(eCallStatsList[i]._95thPercentile_,'f',0)+ " ns)"));
        table->setItem(i,8, new QTableWidgetItem(moe::checkTimeUnit(eCallStatsList[i]._90thPercentile_,0) + " (" + QString::number(eCallStatsList[i]._90thPercentile_,'f',0)+ " ns)"));
        /*itemInfo = moe::checkAbsTimeUnit(eCallStatsList[i].callAvg_, 0);
        table->setItem(i,3, new QTableWidgetItem(itemInfo.timeWithUnit_ + " (" + itemInfo.absTimeWithUnit_ + " ns)"));

        itemInfo = moe::checkAbsTimeUnit(eCallStatsList[i].median_, 0);
        table->setItem(i,4, new QTableWidgetItem(itemInfo.timeWithUnit_ + " (" + itemInfo.absTimeWithUnit_ + " ns)"));

        itemInfo = moe::checkAbsTimeUnit(eCallStatsList[i].standardDeviation_, 0);
        table->setItem(i,5, new QTableWidgetItem(itemInfo.timeWithUnit_ + " (" + itemInfo.absTimeWithUnit_ + " ns)"));

        itemInfo = moe::checkAbsTimeUnit(eCallStatsList[i]._99thPercentile_, 0);
        table->setItem(i,6, new QTableWidgetItem(itemInfo.timeWithUnit_ + " (" + itemInfo.absTimeWithUnit_ + " ns)"));

        itemInfo = moe::checkAbsTimeUnit(eCallStatsList[i]._95thPercentile_, 0);
        table->setItem(i,7, new QTableWidgetItem(itemInfo.timeWithUnit_ + " (" + itemInfo.absTimeWithUnit_ + " ns)"));

        itemInfo = moe::checkAbsTimeUnit(eCallStatsList[i]._90thPercentile_, 0);
        table->setItem(i,8, new QTableWidgetItem(itemInfo.timeWithUnit_ + " (" + itemInfo.absTimeWithUnit_ + " ns)"));*/
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

    /*
     * test customPlot
     */

   /* QCustomPlot *customPlot = new QCustomPlot();
    const QVector<moe::CallStatistics> &eCallStatsList = db->getEcallStatistics();

    QSharedPointer<QCPAxisTickerText> plottedEcallNames(new QCPAxisTickerText);
    for (int l = 0; l < eCallStatsList.size() ; ++l) {
        plottedEcallNames->addTick(5*(l+1), eCallStatsList[l].callSymbolName_);
    }
    customPlot->xAxis->setTicker(plottedEcallNames);
    QVector<qreal> plottedMedian(eCallStatsList.size());
    for (int m = 0; m < eCallStatsList.size() ; ++m) {
        plottedMedian[m] = eCallStatsList[m].median_;
    }
    QCPGraph *graph1 = customPlot->addGraph();
    graph1->setData(plottedEcallNames,plottedMedian);*/

}

void MainWindow::generateCallStatistics()
{
    if(!db->statsGenerated())
    {
        db->loadEcallsStats();
        db->loadOcallsStats();
        statisticsDialog_ = new QDialog();
        statisticsDialog_->setWindowTitle(tr("E/OCall Statistics"));
        auto statsDiagLayout = new QVBoxLayout();

        auto statsDiagTab = new QTabWidget();

        auto ecallStatsTable = loadECallStats();
        auto ocallStatsTable = loadOCallStats();

        statsDiagTab->addTab(ecallStatsTable, "ECall");
        statsDiagTab->addTab(ocallStatsTable, "OCall");
        statsDiagLayout->addWidget(statsDiagTab);
        statisticsDialog_->setLayout(statsDiagLayout);
        statisticsDialog_->setWindowFlags(Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint);
        statisticsDialog_->show();
    } else {
        statisticsDialog_->show();
    }
}

//ToDo can be later done with separate class for refinement and better abstraction
void MainWindow::generateCallStaticAnalysis()
{
    if(!db->staticAnalysisGenerated())
    {
        db->loadEcallAnalysis();
        db->loadOcallAnalysis();
        const QVector<moe::ECallStaticAnalysis> &ecallStaticAnalysisList = db->getEcallStaticAnalysis();
        const QVector<moe::OCallStaticAnalysis> &ocallStaticAnalysisList = db->getOcallStaticAnalysis();
        const QMap<int,moe::ECallDynamicAnalysis> &ecallDynamicAnalysisMap = db->getEcallDynamicAnalysis();
        const QMap<int,moe::OCallDynamicAnalysis> &ocallDynamicAnalysisMap = db->getOcallDynamicAnalysis();

        analysisDialig_ = new QDialog();
        analysisDialig_->setWindowTitle(tr("Call Static Analysis"));
        auto  dialogLayout = new QVBoxLayout();

        auto analysisDialogTab = new QTabWidget();
        auto staticAnalysisDialogTab = new QTabWidget();
        auto dynamicAnalysisDialogTab = new QTabWidget();

        auto eCallStaticAnalysisTable = new QTableWidget();
        eCallStaticAnalysisTable->setRowCount(ecallStaticAnalysisList.size());
        eCallStaticAnalysisTable->setColumnCount(2);
        eCallStaticAnalysisTable->setHorizontalHeaderLabels(QString("Call Name;Analysis").split(";"));
        for (int i = 0; i < ecallStaticAnalysisList.size() ; ++i)
        {
            eCallStaticAnalysisTable->setItem(i,0,new QTableWidgetItem(ecallStaticAnalysisList[i].callName_));
            auto textItem = new QTableWidgetItem(ecallStaticAnalysisList[i].analysisText_);
            !ecallStaticAnalysisList[i].warning ? textItem->setBackground(QBrush(Qt::green)) : textItem->setForeground(QBrush(Qt::red));
            eCallStaticAnalysisTable->setItem(i,1,textItem);
        }
        eCallStaticAnalysisTable->resizeRowsToContents();
        eCallStaticAnalysisTable->resizeColumnsToContents();
        eCallStaticAnalysisTable->horizontalHeader()->setStretchLastSection(true);
        eCallStaticAnalysisTable->verticalHeader()->setStretchLastSection(true);
        eCallStaticAnalysisTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
        eCallStaticAnalysisTable->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);


        auto *oCallStaticAnalysisTable = new QTableWidget();
        oCallStaticAnalysisTable->setRowCount(ocallStaticAnalysisList.size());
        oCallStaticAnalysisTable->setColumnCount(2);
        oCallStaticAnalysisTable->setHorizontalHeaderLabels(QString("Call Name;Analysis").split(";"));
        for (int j = 0; j < ocallStaticAnalysisList.size() ; ++j)
        {
            oCallStaticAnalysisTable->setItem(j,0,new QTableWidgetItem(ocallStaticAnalysisList[j].callName_));
            auto textItem = new QTableWidgetItem(ocallStaticAnalysisList[j].analysisText_);
            !ocallStaticAnalysisList[j].warning ? textItem->setBackground(QBrush(Qt::green)) : textItem->setForeground(QBrush(Qt::red));
            oCallStaticAnalysisTable->setItem(j,1,textItem);
        }
        oCallStaticAnalysisTable->resizeRowsToContents();
        oCallStaticAnalysisTable->resizeColumnsToContents();
        oCallStaticAnalysisTable->horizontalHeader()->setStretchLastSection(true);
        oCallStaticAnalysisTable->verticalHeader()->setStretchLastSection(true);
        oCallStaticAnalysisTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
        oCallStaticAnalysisTable->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

        auto eCallDynamicAnalysisTable = new QTableWidget();
        eCallDynamicAnalysisTable->setRowCount(ecallDynamicAnalysisMap.size());
        eCallDynamicAnalysisTable->setColumnCount(3);
        eCallDynamicAnalysisTable->setHorizontalHeaderLabels(QString("Id;Call Name;Analysis").split(";"));
        QMap<int,moe::ECallDynamicAnalysis>::const_iterator k;
        int tblCntr = 0;
        for (k = ecallDynamicAnalysisMap.begin();  k != ecallDynamicAnalysisMap.end() ; k++)
        {
            auto item = new QTableWidgetItem;
            item->setData(Qt::EditRole,k.value().callId_);
            eCallDynamicAnalysisTable->setItem(tblCntr, 0, item);
            eCallDynamicAnalysisTable->setItem(tblCntr, 1, new QTableWidgetItem(k.value().callName_));
            auto textItem = new QTableWidgetItem(k.value().analysisText_);
            !k.value().warning ? textItem->setBackground(QBrush(Qt::green)) : textItem->setForeground(QBrush(Qt::red));
            eCallDynamicAnalysisTable->setItem(tblCntr, 2, textItem);
            tblCntr++;
        }
        eCallDynamicAnalysisTable->resizeColumnsToContents();
        eCallDynamicAnalysisTable->resizeRowsToContents();
        eCallDynamicAnalysisTable->horizontalHeader()->setStretchLastSection(true);
        eCallDynamicAnalysisTable->verticalHeader()->setStretchLastSection(true);
        eCallDynamicAnalysisTable->verticalHeader()->hide();
        eCallDynamicAnalysisTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
        eCallDynamicAnalysisTable->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

        auto oCallDynamicAnalysisTable = new QTableWidget();
        oCallDynamicAnalysisTable->setRowCount(ocallDynamicAnalysisMap.size());
        oCallDynamicAnalysisTable->setColumnCount(3);
        oCallDynamicAnalysisTable->setHorizontalHeaderLabels(QString("Id;Call Name;Analysis").split(";"));
        tblCntr = 0;
        QMap<int,moe::OCallDynamicAnalysis>::const_iterator l;
        for (l = ocallDynamicAnalysisMap.begin();  l != ocallDynamicAnalysisMap.end() ; l++)
        {
            auto item = new QTableWidgetItem;
            item->setData(Qt::EditRole,l.value().callId_);
            oCallDynamicAnalysisTable->setItem(tblCntr, 0, item);
            oCallDynamicAnalysisTable->setItem(tblCntr, 1, new QTableWidgetItem(l.value().callName_));
            auto textItem = new QTableWidgetItem(l.value().analysisText_);
            !l.value().warning ? textItem->setBackground(QBrush(Qt::green)) : textItem->setForeground(QBrush(Qt::red));
            oCallDynamicAnalysisTable->setItem(tblCntr, 2, textItem);
            tblCntr++;
        }
        oCallDynamicAnalysisTable->resizeColumnsToContents();
        oCallDynamicAnalysisTable->resizeRowsToContents();
        oCallDynamicAnalysisTable->horizontalHeader()->setStretchLastSection(true);
        oCallDynamicAnalysisTable->verticalHeader()->setStretchLastSection(true);
        oCallDynamicAnalysisTable->verticalHeader()->hide();
        oCallDynamicAnalysisTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
        oCallDynamicAnalysisTable->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

        analysisDialogTab->addTab(staticAnalysisDialogTab,"Static");
        analysisDialogTab->addTab(dynamicAnalysisDialogTab,"Dynamic");
        staticAnalysisDialogTab->addTab(eCallStaticAnalysisTable,"ECall");
        staticAnalysisDialogTab->addTab(oCallStaticAnalysisTable,"OCall");
        dynamicAnalysisDialogTab->addTab(eCallDynamicAnalysisTable, "ECall");
        dynamicAnalysisDialogTab->addTab(oCallDynamicAnalysisTable, "OCall");

        dialogLayout->addWidget(analysisDialogTab);
        analysisDialig_->setLayout(dialogLayout);
        analysisDialig_->setWindowFlags(Qt::WindowMaximizeButtonHint | Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint);
        analysisDialig_->show();
    } else {
        analysisDialig_->show();
    }
}
