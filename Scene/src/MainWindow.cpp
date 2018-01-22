


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


void MainWindow::wheelEvent(QWheelEvent *event)
{
    if (event->modifiers() & Qt::ControlModifier)
    {
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

void MainWindow::scrollToNextEvent()
{
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
    scene_ = new QGraphicsScene(this);
    view_ = new QGraphicsView(scene_,this);
    view_->setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);
    //view_->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    //view_->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    view_->setMouseTracking(true);
    view_->setInteractive(true);
    view_->setGeometry(0, 0, this->rect().width()*0.8, this->rect().height()*0.8);
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
    //toDo testing the new update initialize method
    //scene_->clear();
    for(auto i: scene_->items()) {
        scene_->removeItem(i);
    }
    scene_->setBackgroundBrush(Qt::white);
    moe::SceneData data{scene_};
    sceneRootNode_->render(data,sceneTransformation);
    scene_->update();
    viewArea_->show();
    //view_->show(); //ToDo replaced it with viewArea.show (needs testing a little bit)
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
    QMainWindow::resizeEvent(event);

    viewArea_->setGeometry(0, 0, this->rect().width()*0.8 , this->rect().height()*0.8);
    view_->setGeometry(0, 0, this->rect().width()*0.8 , this->rect().height()*0.8);
    view_->setFrameStyle(0);
    scene_->setSceneRect(view_->rect());
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
        measureLine_->setPixel_line_depth_(this->height()*0.75);
        factor_ = (double)(this->height()*0.75)/db->getProgramTotalTime();
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
    view_->update();
}

void MainWindow::loadFile(const QString& fileName)
{
    if(fileName.contains(".db")) {
        if (db) {
            delete db;
        }
        db = new moe::SgxDatabaseStructure(fileName);
        updateTraces();
    }
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
    if (yOffset_ + (yOffset/factor) >= 0) {
        qreal oldXCordinate = sequenceListNode_->getTransform().getX();
        //std::cerr << "it should be back to zero now, before setTransform , and yOffset is " << yOffset_ << "and pixelScroll is " << pixelScroll << std::endl;
        sequenceListNode_->setTransform(moe::Transform2D(1,0,0,1,oldXCordinate,0));
        //std::cerr << "transform after is " << sequenceListNode_->getTransform().getY() << std::endl;
        yOffset_ = 0;

    } else if (moe::signum(yOffset_)*(yOffset_ + (yOffset/factor)) <= (db->getProgramTotalTime() * yScale_)){
        pixelScroll = yOffset;
        /*qreal print = (yOffset_ + (yOffset/factor));
        std::cerr << " yoffset_ + (yOffset/factor) " << print << " db->getProgramTotalTime() * yScale_" << db->getProgramTotalTime() * yScale_ << std::endl;
        std::cerr << " yoffset_ is now at " << yOffset_ << " and the program total length is" << db->getProgramTotalTime() * yScale_ << std::endl;
        pixelScroll = (((db->getProgramTotalTime() * yScale_) + yOffset_) * factor_) * -1;
        std::cerr << "pixel Scroll is now " << pixelScroll << std::endl;
        yOffset_ -= (db->getProgramTotalTime() * yScale_) + yOffset_;*/
        //std::cerr << "it should be back to zero now, before setTransform , and yOffset is " << yOffset_ << "and pixelScroll is " << pixelScroll << std::endl;
        sequenceListNode_->setTransform(sequenceListNode_->getTransform() * moe::Transform2D(1, 0, 0, 1, 0, pixelScroll));
        //std::cerr << "transform after is " << sequenceListNode_->getTransform().getY() << std::endl;
        yOffset_ += (yOffset/factor);
    } else {
        /*if(yOffset_ == db->getProgramTotalTime()*yScale_ *-1)
            return;
        scrollTo(-1*(db->getProgramTotalTime())*yScale_,factor_);*/
       // std::cerr << "we are heeeeeeeere boys " << std::endl;

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
    render();
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
    eCallList_->clear();
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
    threadList_->clear();
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
    //thread dock which will have double layout later
    threadDock_ = new QDockWidget(tr("Thread Filter"), this);
    threadDock_->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    threadList_ = new QListWidget();
    //multiThreadDockWidget has 2 layouts, the first one is the main one vertical layout and has another widget which has the horizontal layout for the buttons
    QWidget *multiThreadDockWidget = new QWidget();
    auto multiThreadWidgetLayout = new QVBoxLayout();
    multiThreadWidgetLayout->addWidget(threadList_);
    //multiThreadButtonWidget has the horizontal layout and the 2 push buttons
    QWidget *multiThreadButtonWidget = new QWidget();
    QHBoxLayout *multiThreadButtonLayout = new QHBoxLayout();
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



    eCallDock_ = new QDockWidget(tr("ECall Filter"), this);
    eCallDock_->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    eCallList_ = new QListWidget();
    //multiECallDockWidget has 2 layouts, the first one is the main one vertical layout and has another widget which has the horizontal layout for the buttons
    QWidget *multiECallDockWidget = new QWidget();
    auto multiECallWidgetLayout = new QVBoxLayout();
    multiECallWidgetLayout->addWidget(eCallList_);
    //multiECallButtonWidget has the horizontal layout and the 2 push buttons
    QWidget *multiECallButtonWidget = new QWidget();
    QHBoxLayout *multiECallButtonLayout = new QHBoxLayout();
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
    QWidget *multiOCallDockWidget = new QWidget();
    auto multiOCallWidgetLayout = new QVBoxLayout();
    multiOCallWidgetLayout->addWidget(oCallList_);
    //multiOCallButtonWidget has the horizontal layout and the 2 push buttons
    QWidget *multiOCallButtonWidget = new QWidget();
    QHBoxLayout *multiOCallButtonLayout = new QHBoxLayout();
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
    if(updateThreads() | updateECalls() | updateOCalls())
    {
        updateScene = true;
        filter = new moe::ThreadFilter(db,chosenThreads.toList().toVector());
        filter->execute();
        delete filter;
        filter  = new moe::ECallOCallFilter(db,chosenEcalls.toList().toVector(), chosenOcalls.toList().toVector());
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
    for (int k = 0; k < oCallList_->count() ; ++k)
    {
        oCallList_->item(k)->setCheckState(Qt::Checked);
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
    factor_ = (double)(this->height() * 0.7) / db->getProgramTotalTime();
    measureLine_ = new moe::MeasureLine(moe::Transform2D(1,0,0,1,scene_->sceneRect().x()+5,50),db->getProgramTotalTime(),this->height()*0.7, 40);
    registerObserver(measureLine_);
    sceneRootNode_->children_.push_back(measureLine_);
    visualizeThreads(db->getThreads_(), factor_);
    generateThreadList();
    generateECallList();
    generateOCallList();
    moe::SceneData data{scene_};
    sceneRootNode_->initialize(data, sceneTransformation);
    zoomAndScrollTofirstEvent();
    render();
}

