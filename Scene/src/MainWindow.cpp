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
}


void MainWindow::open()
{
    QString fileName = QFileDialog::getOpenFileName(this);
    if (!fileName.isEmpty())
        loadFile(fileName);
}

void MainWindow::onZoomInPressed() {

    /*for (moe::SequenceDiagram* r: sequenceList) {
        r->getSequenceLine_()->getTransform().setYScale(r->getSequenceLine_()->getTransform().yScale() * 1.2 );

    }*/

   // std::cout << size().width() << " | "  << size().height() << "\n" <<endl;

    //measureLine_->getMeasureLine_().getTransform().setYScale(measureLine_->getMeasureLine_().getTransform().yScale() * 1.2);
//before changing the measureLine
    for (moe::Renderable* r: sequenceListNode_->children_) {
        ((moe::SequenceDiagram *)r)->getSequenceLine_()->getTransform().setYScale(((moe::SequenceDiagram *)r)->getSequenceLine_()->getTransform().yScale() * 1.2 );
    }
    //sequenceListNode_->getTransform().setYScale(sequenceListNode_->getTransform().yScale() * 1.2);
    measureLine_->generateScales((sequenceDiagram->getSequenceLine_()->getTransform().yScale()),
                                 -sequenceListNode_->getTransform().getY());
    render();
    std::cout << measureLine_->getMeasureLine_().getTransform().yScale() << "\n" << std::endl;
    //QPoint sceneCord = mapFrom()
    /*qDebug() << scene_->sceneRect().topLeft()
             << scene_->sceneRect().bottomRight();
    qDebug() << view_->rect().topLeft() << view_->rect().bottomRight();
    qDebug() << this->rect().topLeft() << this->rect().bottomRight();
    qDebug() << menuBar()->rect();*/
    /*for(moe::Renderable* r: renderables){
        qDebug() << (r->getTransform().getX()) << (r->getTransform().getY());

    }*/
    //qDebug() << lineOfTopElement2->getTransform().yScale();
    //std::cout << lineOfTopElement2->getTransform().yScale() << "\n" << endl;
    //std::cout << lineOfTopElement2->getTransform().getY() << "\n" << endl;
}

void MainWindow::onZoomOutPressed()
{
    //measureLine_->getMeasureLine_().getTransform().setYScale(measureLine_->getMeasureLine_().getTransform().yScale() * (1/1.2));//before changing the measureLine

    for (moe::Renderable* r: sequenceListNode_->children_) {
        ((moe::SequenceDiagram *)r)->getSequenceLine_()->getTransform().setYScale(((moe::SequenceDiagram *)r)->getSequenceLine_()->getTransform().yScale() * (1/1.2) );
    }
    measureLine_->generateScales((sequenceDiagram->getSequenceLine_()->getTransform().yScale()), -sequenceListNode_->getTransform().getY());
    /*for (moe::Renderable* r: sequenceListNode_->children_)
    {
        r->getLine()->getTransform().setYScale(r->getLine()->getTransform().yScale() * (1/1.2));
    }*/
    render();
    //qDebug() << lineOfTopElement2->getTransform().yScale();
}

void MainWindow::resetPressed()
{
    /*for(moe::SequenceDiagram* r : sequenceList)
    {
        r->getSequenceLine_()->getTransform().setXScale(1);
        r->getSequenceLine_()->getTransform().setYScale(1);
    }*/
    measureLine_->getMeasureLine_().getTransform().setXScale(1);
    measureLine_->getMeasureLine_().getTransform().setYScale(1);
    for (moe::Renderable* r: sequenceListNode_->children_) {

        ((moe::SequenceDiagram *)r)->getSequenceLine_()->getTransform().setXScale(1);
        ((moe::SequenceDiagram *)r)->getSequenceLine_()->getTransform().setYScale(1);

    }
    sequenceListNode_->setTransform(moe::Transform2D());
   // sceneTransformation = moe::Transform2D(); //before changing the measureLine
    measureLine_->generateScales((sequenceDiagram->getSequenceLine_()->getTransform().yScale()), sequenceListNode_->getTransform().getY());
    //MeasureLine->getTransform().setXScale(1);
    //MeasureLine->getTransform().setYScale(1);
    render();
}

void MainWindow::scrollUpPressed() {
   // sceneTransformation.setY(sceneTransformation.getY() + 20); //before changing the measureLine

    sequenceListNode_->setTransform(sequenceListNode_->getTransform() * moe::Transform2D(1,0,0,1,0,20));
    measureLine_->generateScales((sequenceDiagram->getSequenceLine_()->getTransform().yScale()), -sequenceListNode_->getTransform().getY());
    //measureLine_->getTransform().setTransfor(measureLine_->getTransform().getTransfor() * QTransform(1,0,0,1,0,20));
    render();
}

void MainWindow::scrollDownPressed() {
   // sceneTransformation.setY(sceneTransformation.getY() - 20);//before changing the measureLine

    sequenceListNode_->setTransform(sequenceListNode_->getTransform() * moe::Transform2D(1,0,0,1,0,-20));
    measureLine_->generateScales((sequenceDiagram->getSequenceLine_()->getTransform().yScale()), -sequenceListNode_->getTransform().getY());
    //measureLine_->getTransform().setTransfor(measureLine_->getTransform().getTransfor() * QTransform(1,0,0,1,0,-20));
    render();
}

void MainWindow::scrollRightPressed() {
    sequenceListNode_->setTransform(sequenceListNode_->getTransform() * moe::Transform2D(1,0,0,1,-20,0));
    render();
}

void MainWindow::scrollLeftPressed() {
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
    //std:: cout << scene_->sceneRect().center().x() << " and y is :" << scene_->sceneRect().center().y() << "\n" << endl;
    //sceneRootNodeOffset_ = new moe::EmptyRenderable();
    sceneRootNode_ = new moe::EmptyRenderable();
    sequenceListNode_ = new moe::EmptyRenderable();
    sequenceDiagram = new moe::SequenceDiagram(moe::Transform2D(QTransform(1,0,0,1, 60,scene_->sceneRect().center().y()/4)),tr("test1"));

    //std:: cout << "scenes cord after seqdiag" <<scene_->sceneRect().center().x() << " and y is :" << scene_->sceneRect().center().y() << "\n" << endl;
    measureLine_ = new moe::MeasureLine(moe::Transform2D(QTransform(1,0,0,1,
                                                                    scene_->sceneRect().x()+10,
                                                                    sequenceDiagram->getTransform().getY()+sequenceDiagram->getTopBlock_().getHeight())),
            500, 26);
    //sceneRootNodeOffset_->children_.push_back(sceneRootNode_);

    sceneRootNode_->children_.push_back(sequenceListNode_);
    sceneRootNode_->children_.push_back(measureLine_);
    sequenceListNode_->children_.push_back(sequenceDiagram);
    //std:: cout << "measureline cord" << measureLine_->getTransform().getX() << "and y is : " << measureLine_->getTransform().getY() << "\n" <<endl;
    //renderables.push_back(sequenceDiagram);
    //renderables.push_back(measureLine_);
    //sequenceList.push_back(sequenceDiagram);
    render();
}

void MainWindow::render() {
    scene_->clear();

    scene_->setBackgroundBrush(Qt::lightGray);

    moe::SceneData data{scene_};
    /*for (moe::Renderable* r: renderables)
        r->render(data, sceneTransformation);*/
    sceneRootNode_->render(data, sceneTransformation);
    //MeasureLine->render(data,sceneTransformation);
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
    if(fileName == tr("blabla") ) return;
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
