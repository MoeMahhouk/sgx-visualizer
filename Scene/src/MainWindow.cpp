
#include "MainWindow.h"


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
    for (moe::Renderable* r: sequenceListNode_->children_)
    {
        moe::SequenceDiagram *s = static_cast<moe::SequenceDiagram*>(r);
        s->setLineScale(pow((double)2, event->delta() / 240.0));
    }
    moe::ZoomEvent e = {sequenceDiagram->getSequenceLine_(),sequenceListNode_};
    notify(&e);
    //notify(*sequenceDiagram->getSequenceLine_(),*sequenceListNode_,moe::Observer::ZOOM);
    //measureLine_->generateScales((sequenceDiagram->getLineScale()), -sequenceListNode_->getTransform().getY());
    render();
    std::cout <<pow((double)2, event->delta() / 240.0)<< std::endl;
}

void MainWindow::onZoomInPressed()
{
    for (moe::Renderable* r: sequenceListNode_->children_)
    {
        moe::SequenceDiagram *s = static_cast<moe::SequenceDiagram*>(r);
        s->setLineScale(1.2);
    }
    moe::ZoomEvent e = {sequenceDiagram->getSequenceLine_(),sequenceListNode_};
    //e.r1 = sequenceDiagram->getSequenceLine_();
    //e.r2 = sequenceListNode_;
    notify(&e);
    //measureLine_->generateScales((sequenceDiagram->getLineScale()), -sequenceListNode_->getTransform().getY());
    render();
}

void MainWindow::onZoomOutPressed()
{
    for (moe::Renderable* r: sequenceListNode_->children_)
    {
        moe::SequenceDiagram *s = static_cast<moe::SequenceDiagram*>(r);
        s->setLineScale(1/1.2);
    }
    moe::ZoomEvent e = {sequenceDiagram->getSequenceLine_(),sequenceListNode_};
    notify(&e);
    //notify(*sequenceDiagram->getSequenceLine_(),*sequenceListNode_,moe::Observer::ZOOM);
    //measureLine_->generateScales(sequenceDiagram->getLineScale(), -sequenceListNode_->getTransform().getY());
    render();
}

void MainWindow::resetPressed()
{
    //measureLine_->resetScales();
    for (moe::Renderable* r: sequenceListNode_->children_)
    {
        moe::SequenceDiagram *s = static_cast<moe::SequenceDiagram*>(r);
        s->resetLineScales();
    }
    sequenceListNode_->setTransform(moe::Transform2D());
    moe::ResetEvent e = {sequenceDiagram->getSequenceLine_(),sequenceListNode_};
    notify(&e);
    //notify(*sequenceDiagram->getSequenceLine_(),*sequenceListNode_,moe::Observer::RESET);
    //measureLine_->generateScales(sequenceDiagram->getLineScale(), -sequenceListNode_->getTransform().getY());
    render();
}

void MainWindow::scrollUpPressed()
{
    sequenceListNode_->setTransform(sequenceListNode_->getTransform() * moe::Transform2D(1,0,0,1,0,20));
    moe::ScrollEvent e = {sequenceDiagram->getSequenceLine_(),sequenceListNode_};
    notify(&e);
    //notify(*sequenceDiagram->getSequenceLine_(),*sequenceListNode_,moe::Observer::SCROLL);
    //measureLine_->generateScales(sequenceDiagram->getLineScale(), -sequenceListNode_->getTransform().getY());
    render();
}

void MainWindow::scrollDownPressed()
{
    sequenceListNode_->setTransform(sequenceListNode_->getTransform() * moe::Transform2D(1,0,0,1,0,-20));
    moe::ZoomEvent e = {sequenceDiagram->getSequenceLine_(),sequenceListNode_};
    notify(&e);
    //notify(*sequenceDiagram->getSequenceLine_(),*sequenceListNode_,moe::Observer::SCROLL);
    //measureLine_->generateScales(sequenceDiagram->getLineScale(), -sequenceListNode_->getTransform().getY());
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
    view_->setRenderHint(QPainter::Antialiasing);
    view_->setGeometry(200, 53, this->rect().width()*0.8, this->rect().height());
    scene_->setSceneRect(view_->rect());
    //zoomIn->setFixedSize(20,20);
    //zoomIn->setParent(view_);
    sceneRootNode_ = new moe::EmptyRenderable();
    sequenceListNode_ = new moe::EmptyRenderable();
    sequenceDiagram = new moe::SequenceDiagram(moe::Transform2D(1,0,0,1, scene_->sceneRect().x()+90,scene_->sceneRect().center().y()/4),tr("test1"),500);
    measureLine_ = new moe::MeasureLine(moe::Transform2D(1,0,0,1,
                                                                    scene_->sceneRect().x()+10,
                                                                    sequenceDiagram->getTransform().getY()+sequenceDiagram->getTopBlock_().getHeight()),
            500, 26);

    //Event listner test
    registerObserver(measureLine_);
    //
std::cout << "MainWindow 1" << std::endl;
    sceneRootNode_->children_.push_back(sequenceListNode_);
    std::cout << "MainWindow 2" << std::endl;
    sceneRootNode_->children_.push_back(measureLine_);
    std::cout << "MainWindow 3" << std::endl;
    sequenceListNode_->children_.push_back(sequenceDiagram);
    std::cout << "MainWindow 4" << std::endl;
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
    visualizeThreads(db->getThreads_()); //ToDo still should be tested
    //std::cerr << db->getThreadStartTime(0) << std::endl;
   // std::cerr << db.getNumberOfRows("threads") << std::endl;
   //db.testMethod(0);
    // db.close();

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

void MainWindow::visualizeThreads(const QVector<moe::MyThread> threads) {
    for (int i = 0; i < threads.length() ; ++i) {
      /*  moe::SequenceDiagram* thread = threads[i].toRenderable(); /*new moe::SequenceDiagram(moe::Transform2D(1,0,0,1, scene_->sceneRect().x() + (90 * (i+1)),
                                                                                 scene_->sceneRect().center().y()/4),
                                                                "Pthread -- " + threads[i].pthread_id_ % 10000,
                                                                threads[i].total_time_);
        thread->setTransform(moe::Transform2D(1,0,0,1, scene_->sceneRect().x() + (90 * (i+1)),
                                              scene_->sceneRect().center().y()/4));
        sequenceListNode_->children_.push_back(thread);*/
    }
}

