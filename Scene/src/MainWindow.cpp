

#include "MainWindow.h"
#include "Utility/MathUtility.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent)
{
    createActions();
    createMenus();
    createToolbar();
    createStatusBar();
    applySettings();
    drawScene();
    /*setCentralWidget(view_);
    threadDock_ = new QDockWidget(tr("Threads"), this);
    threadDock_->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    threadList_ = generateThreadList();
    threadDock_->setWidget(threadList_);
    addDockWidget(Qt::LeftDockWidgetArea, threadDock_);

    eCallDock_ = new QDockWidget(tr("ECalls"), this);
    eCallDock_->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    eCallList_ = generateECallList();
    eCallDock_->setWidget(eCallList_);
    addDockWidget(Qt::LeftDockWidgetArea, eCallDock_);*/

}

MainWindow::~MainWindow()
{
   // delete ui;
	delete sceneRootNode_;
    delete db;
}


void MainWindow::open()
{
    QString fileName = QFileDialog::getOpenFileName(this);
    if (!fileName.isEmpty())
        loadFile(fileName);
}


void MainWindow::wheelEvent(QWheelEvent *event) {
    if (event->modifiers() & Qt::ControlModifier) {
       // qreal oldYOffset = yOffset_;
        qreal yScaleFactor = pow((double) 2, event->delta()/ 240.0);
        verticalZoom(yScaleFactor);
        //scrollTo(oldYOffset * yScaleFactor, factor_);
        //std::cerr << "the oldYOffset is : " << oldYOffset * factor_ << std::endl;
    } else {
        verticalScroll(moe::signum(event->delta())*10,factor_); //ToDo scrolling schould be negative in the other direction
    }
}

void MainWindow::onZoomInPressed()
{
    verticalZoom(1.2);
}

void MainWindow::onZoomOutPressed()
{
    verticalZoom(1/1.2);
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
   // moe::ResetEvent e = {sequenceDiagram->getSequenceLine_(),sequenceListNode_}; //ToDo the old ResetEvent
    moe::ResetEvent e;
    notify(&e);
    render();
}

void MainWindow::scrollUpPressed()
{
    verticalScroll(20, factor_);
}

void MainWindow::scrollDownPressed()
{
    verticalScroll(-20, factor_);
    /**
     * filter test ToDo should be later implemented with listitems and chosing method
     */
    QVector<int> chosenElements;
    chosenElements.push_back(19);
    chosenElements.push_back(8);
    chosenElements.push_back(9);
    chosenElements.push_back(31);
    filter = new moe::ECallFilter(db,chosenElements);
    filter->execute();
    sequenceListNode_->children_.clear();
    visualizeThreads(db->getThreads_(), factor_);
    render();

}

void MainWindow::scrollRightPressed()
{
    sequenceListNode_->setTransform(sequenceListNode_->getTransform() * moe::Transform2D(1,0,0,1,-20,0));
    render();
}

void MainWindow::scrollLeftPressed()
{
    sequenceListNode_->setTransform(sequenceListNode_->getTransform() * moe::Transform2D(1,0,0,1,20,0));
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

void MainWindow::drawScene()
{
    //QDockWidget * mywid = new QDockWidget(tr("Buttons"),this);
    QPushButton* scrollUp = new QPushButton(tr("Scroll Up"), this);
    QPushButton* scrollDown = new QPushButton(tr("Scroll Down"), this);
    QPushButton* zoomIn = new QPushButton(tr("Zoom In"), this);
    QPushButton* zoomOut = new QPushButton(tr("Zoom Out") , this);
    QPushButton* reset = new QPushButton(tr("Reset"), this);
    QPushButton* scrollRight = new QPushButton(tr("Scroll Right"), this);
    QPushButton* scrollLeft = new QPushButton(tr("Scroll Left"), this);
    QPushButton* scrollToNextEventButton = new QPushButton(tr("Next Event"), this);
    /*QVBoxLayout * layout = new QVBoxLayout;
    layout->addWidget(scrollUp);
    layout->addWidget(scrollDown);
    layout->addWidget(zoomIn);
    layout->addWidget(zoomOut);
    layout->addWidget(reset);
    layout->addWidget(scrollRight);
    layout->addWidget(scrollLeft);
    layout->addWidget(scrollToNextEventButton);
    mywid->setLayout(layout);
    addDockWidget(Qt::LeftDockWidgetArea,mywid);*/
    reset->move(0,155);
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
    scrollToNextEventButton->connect(scrollToNextEventButton, SIGNAL(clicked()), this, SLOT(scrollToNextEvent()));
    scene_ = new QGraphicsScene(this);
    view_ = new QGraphicsView(scene_,this);
    //view_->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view_->setRenderHint(QPainter::Antialiasing);
    view_->setGeometry(200, 55, this->rect().width()*0.8, this->rect().height()*0.9);
    view_->setFrameStyle(0);

    //view_->centerOn(200,55);
    //view_->setFixedSize(this->rect().width()*0.8,this->rect().height()*0.9);
    //view_->fitInView(0,0,this->rect().width()*0.8,this->rect().height()*0.9,Qt::KeepAspectRatio);
    //QRect viewRect = view_->rect();
    //viewRect.setRect(0,0,this->rect().width()*0.8-200, this->rect().height()*0.9-55);
    scene_->setSceneRect(view_->rect());
    //scene_->setSceneRect(view_->rect().x()*0.8,view_->rect().y()*0.8,view_->rect().width()*0.8,view_->rect().height()*0.8);

    //zoomIn->setFixedSize(20,20);
    //zoomIn->setParent(view_);
    sceneRootNode_ = new moe::EmptyRenderable();
    sequenceListNode_ = new moe::EmptyRenderable(moe::Transform2D(1,0,0,1,0,0));
   // sequenceDiagram = new moe::SequenceDiagram(moe::Transform2D(1,0,0,1, scene_->sceneRect().x()+90,scene_->sceneRect().center().y()/4),tr("test1"),500);
    //measureLine_ = new moe::MeasureLine(moe::Transform2D(1,0,0,1,
      //                                                              scene_->sceneRect().x()+10,50
                                                                    /*sequenceDiagram->getTransform().getY()+sequenceDiagram->getTopBlock_().getHeight()*/
        //   ), 500, 26);

    //Event listner test
   // registerObserver(measureLine_);

std::cout << "MainWindow 1" << std::endl;
    sceneRootNode_->children_.push_back(sequenceListNode_);
    //std::cout << "MainWindow 2" << std::endl;
   // sceneRootNode_->children_.push_back(measureLine_);
    //std::cout << "MainWindow 3" << std::endl;
    //sequenceListNode_->children_.push_back(sequenceDiagram);
    //std::cout << "MainWindow 4" << std::endl;
    render();
}

void MainWindow::render() {
    scene_->clear();
    scene_->setBackgroundBrush(Qt::lightGray);
    moe::SceneData data{scene_};
    sceneRootNode_->render(data, sceneTransformation);
    scene_->update();
    view_->show();
}

void MainWindow::applySettings()
{
    QSettings settings("BachelorArbeit", "SceneTest");
    QSize size = settings.value("size", QSize(800, 600)).toSize();
    resize(size);
}

void MainWindow::writeSettings()
{
    QSettings settings("BachelorArbeit", "SceneTest");
    settings.setValue("size", size());
}


void MainWindow::loadFile(const QString& fileName)
{
    db = new moe::SgxDatabaseStructure(fileName);
    factor_ = 500.0/db->getProgramTotalTime();
    measureLine_ = new moe::MeasureLine(moe::Transform2D(1,0,0,1,scene_->sceneRect().x()+10,50),db->getProgramTotalTime(),500,40);
    registerObserver(measureLine_);
    visualizeThreads(db->getThreads_(), factor_); //ToDo still should be tested
   // scrollToNextEvent(db->getThreads_(), factor_);
    //std::cerr << "factor is this small : " << 1000.0/db->getProgramTotalTime() << std::endl;
    sceneRootNode_->children_.push_back(measureLine_);
    zoomAndScrollTofirstEvent();
    render();
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    writeSettings();
    event->accept();
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
    QMainWindow::resizeEvent(event);
    view_->update();
}
/**
 * visualize the threads and their transitions as sequence diagrams in the scene
 * @param threads
 * @param factor
 */
void MainWindow::visualizeThreads(const QVector<moe::MyThread> threads, qreal factor) {
    resetPressed();
    for (int i = 0; i < threads.length() ; ++i) {
        moe::SequenceDiagram* thread = threads[i].toRenderable(factor);
        thread->setTransform(moe::Transform2D(1,0,0,1, scene_->sceneRect().x() + (90 * (i+2)), 30));
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
    if (threads.isEmpty()){
        return;
    }
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
                    /*std::cout << "next event is found, the current time is : " << currentTime << std::endl;
                    std::cout << "the new yOffset should be now : " << threads[i].threadEcalls_[j]->start_time_/10000 << std::endl;
                    std::cout << (threads[i].threadEcalls_[j]->start_time_ > currentTime) << std::endl;*/
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

void MainWindow::scrollTo(qreal yOffset, qreal factor) {
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
void MainWindow::zoomAndScrollTofirstEvent() {
    if(db)
    {
        qreal yScaleNew;
        qreal startTimeOfFirstEvent = db->getThreads_()[0].threadEcalls_[0]->start_time_;
        int lastThreadIndex = db->getThreads_().length() - 1;
        int lastEcallIndex = db->getThreads_()[lastThreadIndex].threadEcalls_.length() - 1;
        qreal startTimeOfLastEvent = db->getThreads_()[lastThreadIndex].threadEcalls_[lastEcallIndex]->start_time_;
        qreal endTimeOfLastEvent = startTimeOfLastEvent + db->getThreads_()[lastThreadIndex].threadEcalls_[lastEcallIndex]->total_time_;
        yScaleNew = (qreal)db->getProgramTotalTime() / (endTimeOfLastEvent - startTimeOfFirstEvent);
        verticalZoom(yScaleNew,factor_);
        scrollTo(-startTimeOfFirstEvent * yScale_, factor_);
    }
}

QListWidget *MainWindow::generateECallList() {
    eCallList_ = new QListWidget();
    QListWidgetItem* item1 = new QListWidgetItem("test1", eCallList_);
    item1->setFlags(item1->flags() | Qt::ItemIsUserCheckable);
    QListWidgetItem* item2 = new QListWidgetItem("test2", eCallList_);
    item2->setFlags(item2->flags() | Qt::ItemIsUserCheckable);
    QListWidgetItem* item3 = new QListWidgetItem("test3", eCallList_);
    item3->setFlags(item3->flags() | Qt::ItemIsUserCheckable);
    eCallList_->show();
    return eCallList_;
}

QListWidget *MainWindow::generateThreadList() {
    threadList_ = new QListWidget();
    QListWidgetItem* item1 = new QListWidgetItem("test1", threadList_);
    item1->setFlags(item1->flags() | Qt::ItemIsUserCheckable);
    item1->setCheckState(Qt::Checked);
    QListWidgetItem* item2 = new QListWidgetItem("test2", threadList_);
    item2->setFlags(item2->flags() | Qt::ItemIsUserCheckable);
    item2->setCheckState(Qt::Checked);
    QListWidgetItem* item3 = new QListWidgetItem("test3", threadList_);
    item3->setFlags(item3->flags() | Qt::ItemIsUserCheckable);
    item3->setCheckState(Qt::Checked);
    return threadList_;
}

