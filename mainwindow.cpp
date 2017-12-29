#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->statusBar->showMessage("Свайп для передвижения плиток. 2 + 2 = 4. Собери 2048.");

    engine = new Engine(this);

    this->createMenuAction();
    this->createMenus();
    this->createConnection();

   // emit canUndo(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}
// ----------------------- PRIVATE ----------------------------------------

void MainWindow::createGameField(int fsize)
{
    engine->setFieldSize(fsize);

    for (int x = 0; x < engine->getFieldSize(); x++){
        for (int y = 0; y < engine->getFieldSize(); y++){
            QLabel *item = new QLabel("");
            item->setAlignment(Qt::AlignCenter);
            item->setStyleSheet("QLabel { background-color: #cdc1b5; color: black; font:20pt; font-weight:400; border-radius: 5px;}");
            cells.append(item);
            engine->addEmptyCellValue();

            ui->gridGame->addWidget(item,x,y);
        }
    }
    this->mScore = 0;
}



void MainWindow::deleteGameField()
{
    qDebug() << Q_FUNC_INFO;

    QLayoutItem *child;
    while ((child = ui->gridGame->takeAt(0)) != 0) {
        if (QWidget* widget = child->widget())
        {
            widget->deleteLater();
        }
        delete child;
    }

    cells.clear();
    undoCells.clear();
}

void MainWindow::addTile()
{
    qDebug() << Q_FUNC_INFO;
    if ( isFindCell("") ){
        bool isSuccessSetTile = false;
        do{
            int index = engine->getRandomIndex();
            if (cells.at(index)->text() == "" )
            {
                cells.at(index)->setText(engine->getTwoInRandomPow());
                isSuccessSetTile = true;
            }
        } while (!isSuccessSetTile);
    }

}

void MainWindow::paintField()
{
    foreach (QLabel *item, cells) {
        switch (item->text().toInt()) {
        case 0:
            item->setStyleSheet("QLabel { background-color: #cdc1b5; color: black; font:20pt; font-weight:400; border-radius: 5px;}");
            break;
        case 2:
            item->setStyleSheet("QLabel { background-color: #eee4da; color: black; font:20pt; font-weight:400; border-radius: 5px;}");
            break;
        case 4:
            item->setStyleSheet("QLabel { background-color: #eddfc4; color: black; font:20pt; font-weight:400;border-radius: 5px;}");
            break;
        case 8:
            item->setStyleSheet("QLabel { background-color: #f4b17a; color: white; font:20pt; font-weight:400;border-radius: 5px;}");
            break;
        case 16:
            item->setStyleSheet("QLabel { background-color: #f79663; color: white; font:20pt; font-weight:400;border-radius: 5px;}");
            break;
        case 32:
            item->setStyleSheet("QLabel { background-color: #f67d62; color: white; font:20pt; font-weight:400;border-radius: 5px;}");
            break;
        case 64:
            item->setStyleSheet("QLabel { background-color: #f65e39; color: white; font:20pt; font-weight:400; border-radius: 5px;}");
            break;
        case 128:
            item->setStyleSheet("QLabel { background-color: #edce73; color: white; font:20pt; font-weight:400;border-radius: 5px;}");
            break;
        case 256:
            item->setStyleSheet("QLabel { background-color: #e9cf58; color: white; font:20pt; font-weight:400;border-radius: 5px;}");
            break;
        case 512:
            item->setStyleSheet("QLabel { background-color: #edc651; color: white; font:20pt; font-weight:400;border-radius: 5px;}");
            break;
        case 1024:
            item->setStyleSheet("QLabel { background-color: #eec744; color: white; font:20pt; font-weight:400;border-radius: 5px;}");
            break;
        case 2048:
            item->setStyleSheet("QLabel { background-color: #edca64; color: white; font:20pt; font-weight:400;border-radius: 5px;}");
            break;
        }
    }
}



void MainWindow::moveCellsHorizontal(bool direction)
{
    bool isMoved;

    for (int row = 0;  row <  engine->getFieldSize(); row++){
        do {
            isMoved = false;
            if (direction)
            { // move up
                for (int col = 0; col < engine->getFieldSize()-1; col++){
                    if (moveCell( row, col, row, col+1 ))
                    {
                        isMoved = true;
                    }
                }
            }else
            { // move down
                for (int col = engine->getFieldSize()-1; col > 0 ; col--){
                    if (moveCell( row, col, row, col-1 ))
                    {
                        isMoved = true;
                    }
                }
            }
        }while (isMoved);
    }
}

void MainWindow::moveCellsVertical(bool direction)
{
    bool isMoved;

    for (int col = 0;  col <  engine->getFieldSize(); col++){
        do {
            isMoved = false;
            if (direction)
            { // move up
                for (int row = 0; row < engine->getFieldSize()-1; row++){
                    if (moveCell( row, col, row+1, col ))
                    {
                        isMoved = true;
                    }
                }
            }else
            { // move down
                for (int row = engine->getFieldSize()-1; row > 0 ; row--){
                    if (moveCell( row, col, row-1, col ))
                    {
                        isMoved = true;
                    }
                }
            }
        }while (isMoved);
    }
}

bool MainWindow::moveCell(int newX,int newY, int oldX,int oldY)
{
    // qDebug() << "old index: " << getIndex( oldX, oldY ) << "new index: "<< getIndex( newX, newY );
    //qDebug() << "old value: " << cells.at( getIndex( oldX, oldY ) )->text()  << "new value: " << cells.at( getIndex( newX, newY ) )->text();
    bool isMoved = false;
    if (cells.at( engine->getIndex( oldX, oldY ) )->text() != "" )
    {
        // need move cell
        if (  cells.at( engine->getIndex( newX, newY) )->text() == ""  )
        {
            cells.at( engine->getIndex( newX, newY ) )->setText( cells.at( engine->getIndex( oldX, oldY ) )->text() );
            cells.at( engine->getIndex( oldX, oldY ) )->setText("");
            isMoved = true;
        }

        if (cells.at( engine->getIndex( newX, newY ) )->text() == cells.at( engine->getIndex( oldX, oldY ) )->text() )
        {
            int value = cells.at( engine->getIndex( oldX, oldY) )->text().toInt() * 2;
            this->addScore(value);

            cells.at( engine->getIndex( newX, newY ) )->setText( QString::number( value ) );
            cells.at( engine->getIndex( oldX, oldY ) )->setText("");
            isMoved = true;
        }
    }
    return isMoved;
}

void MainWindow::moveUp()
{
    printDebugField("move_up");
    saveStateForUndo();
    moveCellsVertical(true);
    addTile();
    paintField();
}

void MainWindow::moveDown()
{
    printDebugField("move_down");
    saveStateForUndo();
    moveCellsVertical(false);
    addTile();
    paintField();
}

void MainWindow::moveLeft()
{
    printDebugField("move_left");
    saveStateForUndo();
    moveCellsHorizontal(true);
    addTile();
    paintField();
}

void MainWindow::moveRigth()
{
    printDebugField("move_rigth");
    saveStateForUndo();
    moveCellsHorizontal(false);
    addTile();
    paintField();
}





bool MainWindow::isFindCell(QString cType)
{
    foreach (QLabel *item, cells) {
        if (item->text() == cType)
        {
            return true;
        }
    }
    return false;
}



void MainWindow::printDebugField(QString direction)
{
    qDebug() << direction;
    /*
    for (int row = 0;  row <  engine->getFieldSize(); row++){
        qDebug() << "|"<< cells.at( getIndex( row, 0) )->text() << "|"<< cells.at( getIndex( row, 1) )->text()
                 << "|"<< cells.at( getIndex( row, 2) )->text() << "|"<< cells.at( getIndex( row, 3) )->text();
    }
*/
}

void MainWindow::addScore(int score)
{
    mScore += score;
    emit valueScoreChanged(mScore);
}

void MainWindow::saveStateForUndo()
{
    for (int col = 0;  col <  engine->getFieldSize(); col++){
        for (int row = 0;  row <  engine->getFieldSize(); row++){
            int index = engine->getIndex( row, col );
            engine->setCellValue(index, cells.at( index )->text().toInt() );
        }
    }

    //emit canUndo(true);
}

// ----------------------- PUBLIC SLOTS -----------------------------------
void MainWindow::startGame()
{    
    qDebug() << Q_FUNC_INFO;
    // this->clearGameField();
    this->addTile();
    this->paintField();
}

void MainWindow::undo()
{
    for (int col = 0;  col <  engine->getFieldSize(); col++){
        for (int row = 0;  row <  engine->getFieldSize(); row++){
            int index = engine->getIndex( row, col );
            int value = engine->getCellValue( index );
            if ( value == 0 ){
                cells.at( index )->setText("");
            }else{
                cells.at( index )->setText( QString::number( value ) );
            }
        }
    }
    this->paintField();
   //emit canUndo(false);
}

void MainWindow::doNewGame_4x4()
{

    if ( ui->gridGame->count() != 0) {
        this->deleteGameField();
    }

    this->createGameField(4);
    this->startGame();
}

void MainWindow::doNewGame_5x5()
{
    if ( ui->gridGame->count() != 0) {
        this->deleteGameField();
    }
    this->createGameField(5);
    this->startGame();
}

void MainWindow::doNewGame_6x6()
{
    if ( ui->gridGame->count() != 0) {
        this->deleteGameField();
    }
    this->createGameField(6);
    this->startGame();
}

//----------------------------------- Menu ---------------------------------------------
void MainWindow::createMenuAction()
{
    newGame_4x4 = new QAction (tr("Новая игра 4х4"),this);
    newGame_4x4->setStatusTip(tr("Запуск новой игры на 4х4"));

    newGame_5x5 = new QAction (tr("Новая игра 5х5"), this);
    newGame_5x5->setStatusTip(tr("Запуск новой игры на 5х5"));

    newGame_6x6 = new QAction (tr("Новая игра 6х6"),this);
    newGame_6x6->setStatusTip(tr("Запуск новой игры на 6х6"));

    exitAct = new QAction (tr("Выход"),this);
    exitAct->setStatusTip(tr("Завершение работы"));
    exitAct->setShortcut(QKeySequence::Quit);

    normalRandom = new QAction (tr("Обычный рандом"),this);
    normalRandom->setStatusTip(tr("Завершение работы"));

    koreanRandom = new QAction(tr("Корейский рандом"),this);
    koreanRandom->setStatusTip(tr("Завершение работы"));

    agroup = new QActionGroup(this);
    agroup->addAction(normalRandom);
    agroup->addAction(normalRandom);

}

void MainWindow::createMenus()
{
    gameMenu = this->menuBar()->addMenu(tr("Игра"));
    gameMenu->addAction(newGame_4x4);
    gameMenu->addAction(newGame_5x5);
    gameMenu->addAction(newGame_6x6);
    gameMenu->addSeparator();
    gameMenu->addAction(exitAct);
}

void MainWindow::createConnection()
{
    //--------------------------- connect ui items----------------------------------
    connect(ui->newGameBtn,SIGNAL(clicked()),this,SLOT(doNewGame_4x4()));
    connect(ui->undoStepBtn,SIGNAL(clicked()),this,SLOT(undo()));

    //--------------------------- connect Mainwindow items -------------------------
    connect(this,SIGNAL(canUndo(bool)),ui->undoStepBtn,SLOT(setEnabled(bool)));
    connect(this,SIGNAL(valueScoreChanged(int)),ui->scoreCounter,SLOT(display(int)));

    //--------------------------- connect menu -------------------------------------

    connect(newGame_4x4,SIGNAL(triggered()),this,SLOT (doNewGame_4x4()));
    connect(newGame_5x5,SIGNAL(triggered()),this,SLOT (doNewGame_5x5()));
    connect(newGame_6x6,SIGNAL(triggered()),this,SLOT (doNewGame_6x6()));
    connect(exitAct,SIGNAL(triggered()),this,SLOT (close()));
}

//------------------------ PROTECTED ------------------------------------
void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    qDebug() << Q_FUNC_INFO;
    qDebug("MainWindow keyPressEvent %d", event->key());
    if (event->type() == QEvent::KeyRelease)
    {
        switch (event->key()) {
        case Qt::Key_Up:
            this->moveUp();
            break;
        case Qt::Key_Down:
            this->moveDown();
            break;
        case Qt::Key_Left:
            this->moveLeft();
            break;
        case Qt::Key_Right:
            this->moveRigth();
            break;
        default:
            break;
        }
        return;
    }

    // allow for the default handling of the key
    QMainWindow::keyPressEvent(event); // aka the base class implementati
}

