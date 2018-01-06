
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
        verticalScroll(moe::signum(event->delta())*10000000,factor_); //ToDo scrolling schould be negative in the other direction
    }

    std::cout << event->delta() /360.0<< std::endl;
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
   /* sequenceListNode_->setTransform(sequenceListNode_->getTransform() * moe::Transform2D(1,0,0,1,0,1000000 * factor_));
    yOffset_ += 1000000;
    //moe::ScrollEvent e = {sequenceDiagram->getSequenceLine_(),sequenceListNode_};
    moe::ScrollEvent e = {yScale_,yOffset_};
    notify(&e);
    render();*/
}

void MainWindow::scrollDownPressed()
{
    verticalScroll(-100000000, factor_);
    /*sequenceListNode_->setTransform(sequenceListNode_->getTransform() * moe::Transform2D(1,0,0,1,0,-1000000 * factor_));
    yOffset_ -= 1000000;
    //moe::ZoomEvent e = {sequenceDiagram->getSequenceLine_(),sequenceListNode_};
    moe::ScrollEvent e = {yScale_,yOffset_};
    notify(&e);
    render();*/
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
    reset->move(0,155);
    zoomOut->move(0,105);
    zoomIn->move(0,55);
    scrollUp->move(0,205);
    scrollDown->move(0,255);
    scrollRight->move(0,305);
    scrollLeft->move(0,355);
    zoomIn->connect(zoomIn, SIGNAL(clicked()) , this, SLOT(onZoomInPressed()));
    zoomOut->connect(zoomOut, SIGNAL(clicked()), this, SLOT(onZoomOutPressed()));
    reset->connect(reset,SIGNAL(clicked()),this, SLOT(resetPressed()));
    scrollUp->connect(scrollUp,SIGNAL(clicked()), this,SLOT(scrollUpPressed()));
    scrollDown->connect(scrollDown,SIGNAL(clicked()), this,SLOT(scrollDownPressed()));
    scrollRight->connect(scrollRight,SIGNAL(clicked()), this, SLOT(scrollRightPressed()));
    scrollLeft->connect(scrollLeft,SIGNAL(clicked()), this, SLOT(scrollLeftPressed()));
    view_ = new QGraphicsView(scene_, this);
    view_->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view_->setRenderHint(QPainter::Antialiasing);
    view_->setGeometry(200, 53, this->rect().width()*0.8, this->rect().height());
    scene_->setSceneRect(view_->rect());
    //zoomIn->setFixedSize(20,20);
    //zoomIn->setParent(view_);
    sceneRootNode_ = new moe::EmptyRenderable();
    sequenceListNode_ = new moe::EmptyRenderable();
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
    moe::DataBaseManager* db = new moe::SgxDatabaseStructure(fileName);
    factor_ = 500.0/db->getProgramTotalTime();
    measureLine_ = new moe::MeasureLine(moe::Transform2D(1,0,0,1,scene_->sceneRect().x()+10,50),db->getProgramTotalTime(),500,40);
    registerObserver(measureLine_);
    visualizeThreads(db->getThreads_(), factor_); //ToDo still should be tested
    scrollToNextEvent(db->getThreads_(), factor_);
    //std::cerr << "factor is this small : " << 1000.0/db->getProgramTotalTime() << std::endl;
    sceneRootNode_->children_.push_back(measureLine_);
    render();
    delete db;
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
        //ToDo Write a query to get the relative start time of each call to their parent caller (done)
        //ToDo Scale the blocks and the line to a specific measure scale (bugy as hell)
    }
}
//ToDo needs implementation for all ecalls of all threads according to their start time
void MainWindow::scrollToNextEvent(const QVector<moe::MyThread> threads, qreal factor) {
    if (threads.isEmpty()){
        return;
    }
    std::cerr << " yOffset ist : " << threads[0].threadEcalls_[0]->start_time_ << std::endl;
    yOffset_ = 0;
    qreal new_yOffset = threads[0].threadEcalls_[0]->start_time_ ;
    for (int i = 0; i < threads.length() ; ++i) { // ToDo this is not finished and only takes the first ecall of each thread and compare them
        if(threads[i].threadEcalls_[0]->start_time_ * factor < yOffset_) {
            new_yOffset = threads[i].threadEcalls_[0]->start_time_;
        }
    }
    verticalScroll(-new_yOffset,factor);

    /*sequenceListNode_->setTransform(sequenceListNode_->getTransform() * moe::Transform2D(1,0,0,1,0,-yOffset_));
    moe::ScrollEvent e = {yScale_,-yOffset_/factor};
    notify(&e);
    */

}

void MainWindow::verticalScroll(qreal yOffset, qreal factor) {
    sequenceListNode_->setTransform(sequenceListNode_->getTransform() * moe::Transform2D(1,0,0,1,0,yOffset * factor));
    yOffset_ += yOffset;
    moe::ScrollEvent e = {yScale_,yOffset_};
    notify(&e);
    render();
}

void MainWindow::verticalZoom(qreal yScale, qreal factor) {
    for (moe::Renderable* r: sequenceListNode_->children_)
    {
        moe::SequenceDiagram *s = static_cast<moe::SequenceDiagram*>(r);
        s->setLineScale(yScale);
        yScale_ *= (yScale);
    }
    moe::ZoomEvent e = {yScale_,yOffset_};
    notify(&e);
    render();
}

