
#include "MainWindow.h"
#include "MathUtility.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent)
{
    createActions();
    createMenus();
    createToolbar();
    createStatusBar();
    applySettings();
    drawScene();

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
        verticalZoom(pow((double) 2, event->delta() / 240.0));
    } else {
        verticalScroll(moe::signum(event->delta())*100000000,factor_); //ToDo scrolling schould be negative in the other direction
    }

    //std::cout << event->delta() /360.0<< std::endl;
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
    verticalScroll(100000000, factor_);
}

void MainWindow::scrollDownPressed()
{
    verticalScroll(-100000000, factor_);
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
    scene_ = new QGraphicsScene(this);
    QPushButton* scrollUp = new QPushButton(tr("Scroll Up"), this);
    QPushButton* scrollDown = new QPushButton(tr("Scroll Down"), this);
    QPushButton* zoomIn = new QPushButton(tr("Zoom In"), this);
    QPushButton* zoomOut = new QPushButton(tr("Zoom Out") , this);
    QPushButton* reset = new QPushButton(tr("Reset"), this);
    QPushButton* scrollRight = new QPushButton(tr("Scroll Right"), this);
    QPushButton* scrollLeft = new QPushButton(tr("Scroll Left"), this);
    QPushButton* scrollToNextEvent = new QPushButton(tr("Next Event"), this);
    reset->move(0,155);
    zoomOut->move(0,105);
    zoomIn->move(0,55);
    scrollUp->move(0,205);
    scrollDown->move(0,255);
    scrollRight->move(0,305);
    scrollLeft->move(0,355);
    scrollToNextEvent->move(0,405);
    zoomIn->connect(zoomIn, SIGNAL(clicked()) , this, SLOT(onZoomInPressed()));
    zoomOut->connect(zoomOut, SIGNAL(clicked()), this, SLOT(onZoomOutPressed()));
    reset->connect(reset,SIGNAL(clicked()),this, SLOT(resetPressed()));
    scrollUp->connect(scrollUp,SIGNAL(clicked()), this,SLOT(scrollUpPressed()));
    scrollDown->connect(scrollDown,SIGNAL(clicked()), this,SLOT(scrollDownPressed()));
    scrollRight->connect(scrollRight,SIGNAL(clicked()), this, SLOT(scrollRightPressed()));
    scrollLeft->connect(scrollLeft,SIGNAL(clicked()), this, SLOT(scrollLeftPressed()));
    scrollToNextEvent->connect(scrollToNextEvent, SIGNAL(clicked()), this, SLOT(scrollToNextEvent()));
    view_ = new QGraphicsView(scene_, this);
    view_->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view_->setRenderHint(QPainter::Antialiasing);
    view_->setGeometry(200, 53, this->rect().width()*0.8, this->rect().height());
    scene_->setSceneRect(view_->rect());
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

void MainWindow::visualizeThreads(const QVector<moe::MyThread> threads, qreal factor) {//ToDo added factor for test purposes
    resetPressed();
    for (int i = 0; i < threads.length() ; ++i) {
        moe::SequenceDiagram* thread = threads[i].toRenderable(factor);
        thread->setTransform(moe::Transform2D(1,0,0,1, scene_->sceneRect().x() + (90 * (i+2)), 30));
        sequenceListNode_->children_.push_back(thread);
        //ToDo fix the zoom events so that they dont depend on sequence diagram object (partially fixed, still need to be refactored)
        //ToDo Scale the blocks and the line to a specific measure scale (bugy as hell)
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
    qreal currentTime = (qreal)((yOffset_ * moe::signum(yOffset_)) / yScale_)/10000;

    std::cerr << " yOffset ist : " << (yOffset_ * moe::signum(yOffset_))/yScale_ << std::endl;
    qreal new_yOffset = 0;

    if (currentTime != 0)
    {
        bool succFound = false;

        for (int i = 0; i < threads.length() ; ++i)
        {
            for (int j = 0; j < threads[i].threadEcalls_.length() ; ++j)
            {
                if(threads[i].threadEcalls_[j]->start_time_/10000 > currentTime)
                {
                    new_yOffset = threads[i].threadEcalls_[j]->start_time_;
                    std::cout << "next event is found, the current time is : " << currentTime << std::endl;
                    std::cout << "the new yOffset should be now : " << threads[i].threadEcalls_[j]->start_time_ << std::endl;
                    std::cout << (threads[i].threadEcalls_[j]->start_time_ > currentTime) << std::endl;

                    succFound = true; //ToDo this should later be adjusted for more threads
                    break;
                }
            }
            /*if(succFound)
            {
                break;
            } else {
                new_yOffset = 0;
            }*/
        }
    } else {
        new_yOffset = threads[0].threadEcalls_[0]->start_time_;
    }
    scrollTo(-new_yOffset * yScale_,factor);
}
//ToDo needs implementation for all ecalls of all threads according to their start time

/**
 * abstract function for vertical scall events
 * @param yOffset
 * @param factor
 */
void MainWindow::verticalScroll(qreal yOffset, qreal factor)
{
    sequenceListNode_->setTransform(sequenceListNode_->getTransform() * moe::Transform2D(1,0,0,1,0,yOffset * factor));
    yOffset_ += yOffset;
    moe::ScrollEvent e = {yScale_,yOffset_};
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
    for (moe::Renderable* r: sequenceListNode_->children_)
    {
        moe::SequenceDiagram *s = static_cast<moe::SequenceDiagram*>(r);
        s->setLineScale(yScale);
    }
    yScale_ *= (yScale);
    moe::ZoomEvent e = {yScale_,yOffset_};
    notify(&e);
    render();
}

void MainWindow::scrollTo(qreal yOffset, qreal factor) {
    sequenceListNode_->setTransform(moe::Transform2D(1,0,0,1,0,yOffset * factor));
    yOffset_ = yOffset;     // so that, it jumps to the target location and doesnt added the targets location to the current offset
    moe::ScrollEvent e = {yScale_,yOffset_};
    notify(&e);
    render();
}

