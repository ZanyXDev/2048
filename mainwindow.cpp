#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->statusBar->showMessage("Свайп для передвижения плиток. 2 + 2 = 4. Собери 2048.");

    this->createGameField();

    connect(ui->newGameBtn,SIGNAL(clicked()),this,SLOT(startGame()));
    connect(this,SIGNAL(valueScoreChanged(int)),ui->scoreCounter,SLOT(display(int)));

    connect(ui->undoStepBtn,SIGNAL(clicked()),this,SLOT(undo()));
    connect(this,SIGNAL(canUndo(bool)),ui->undoStepBtn,SLOT(setEnabled(bool)));

    emit canUndo(false);
}

MainWindow::~MainWindow()
{
    delete ui;
}
// ----------------------- PRIVATE ----------------------------------------

void MainWindow::createGameField()
{
    for (int x = 0; x < FIELD_SIZE; x++){
        for (int y = 0; y < FIELD_SIZE; y++){
            QLabel *item = new QLabel("");
            item->setAlignment(Qt::AlignCenter);
            item->setStyleSheet("QLabel { background-color: #cdc1b5; color: black; font:20pt; font-weight:400; border-radius: 5px;}");
            cells.append(item);
            undoCells.append(0);

            ui->gridGame->addWidget(item,x,y);
        }
    }
    this->mScore = 0;

}

void MainWindow::addTile()
{
    if ( isFindCell("") ){
        bool isSuccessSetTile = false;
        do{
            int index = getRandomIndex();
            if (cells.at(index)->text() == "" )
            {
                cells.at(index)->setText(getTwoInRandomPow());
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

int MainWindow::getIndex(int row, int col)
{
    return col + ( row * FIELD_SIZE ) ;
}

void MainWindow::moveCellsHorizontal(bool direction)
{
    bool isMoved;

    for (int row = 0;  row <  FIELD_SIZE; row++){
        do {
            isMoved = false;
            if (direction)
            { // move up
                for (int col = 0; col < FIELD_SIZE-1; col++){
                    if (moveCell( row, col, row, col+1 ))
                    {
                        isMoved = true;
                    }
                }
            }else
            { // move down
                for (int col = FIELD_SIZE-1; col > 0 ; col--){
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

    for (int col = 0;  col <  FIELD_SIZE; col++){
        do {
            isMoved = false;
            if (direction)
            { // move up
                for (int row = 0; row < FIELD_SIZE-1; row++){
                    if (moveCell( row, col, row+1, col ))
                    {
                        isMoved = true;
                    }
                }
            }else
            { // move down
                for (int row = FIELD_SIZE-1; row > 0 ; row--){
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
    if (cells.at( getIndex( oldX, oldY ) )->text() != "" )
    {
        // need move cell
        if (  cells.at( getIndex( newX, newY) )->text() == ""  )
        {
            cells.at( getIndex( newX, newY ) )->setText( cells.at( getIndex( oldX, oldY ) )->text() );
            cells.at( getIndex( oldX, oldY ) )->setText("");
            isMoved = true;
        }

        if (cells.at( getIndex( newX, newY ) )->text() == cells.at( getIndex( oldX, oldY ) )->text() )
        {
            int value = cells.at( getIndex( oldX, oldY) )->text().toInt() * 2;
            this->addScore(value);

            cells.at( getIndex( newX, newY ) )->setText( QString::number( value ) );
            cells.at( getIndex( oldX, oldY ) )->setText("");
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

int MainWindow::getRandomIndex()
{
    random();
    return getIndex ( (rand() % FIELD_SIZE), (rand() % FIELD_SIZE) );
}

QString MainWindow::getTwoInRandomPow()
{
    random();
    int new_step = rand() % MAX_POW;
    if (new_step == 0)
    {
        new_step = 1;
    }
    return QString::number((int) pow(2,new_step));
}

void MainWindow::clearGameField()
{
    foreach (QLabel *item, cells) {
        item->setText("");
        item->setAlignment(Qt::AlignCenter);
        item->setStyleSheet("QLabel { background-color: #cdc1b5; color: black; font:20pt; font-weight:400; border-radius: 5px;}");
    }
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

    for (int row = 0;  row <  FIELD_SIZE; row++){
        qDebug() << "|"<< cells.at( getIndex( row, 0) )->text() << "|"<< cells.at( getIndex( row, 1) )->text()
                 << "|"<< cells.at( getIndex( row, 2) )->text() << "|"<< cells.at( getIndex( row, 3) )->text();
    }

}

void MainWindow::addScore(int score)
{
    mScore += score;
    emit valueScoreChanged(mScore);
}

void MainWindow::saveStateForUndo()
{
    for (int col = 0;  col <  FIELD_SIZE; col++){
        for (int row = 0;  row <  FIELD_SIZE; row++){
            int index = getIndex( row, col );
            undoCells.replace( index, cells.at( index )->text().toInt() );
        }
    }

    emit canUndo(true);
}

// ----------------------- PUBLIC SLOTS -----------------------------------
void MainWindow::startGame()
{
    this->clearGameField();
    this->addTile();
    this->paintField();
}

void MainWindow::undo()
{
    for (int col = 0;  col <  FIELD_SIZE; col++){
        for (int row = 0;  row <  FIELD_SIZE; row++){
            int index = getIndex( row, col );
            int value = undoCells.at( index );
            if ( value == 0 ){
                cells.at( index )->setText("");
            }else{
                cells.at( index )->setText( QString::number( value ) );
            }
        }
    }
    this->paintField();
    emit canUndo(false);
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

