#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    qApp->installEventFilter(this);
    ui->setupUi(this);
    this->createGameField();
    QObject::connect(ui->newGameBtn,SIGNAL(clicked()),this,SLOT(startGame()));
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

            ui->gridGame->addWidget(item,x,y);
        }
    }
    //qDebug() << "cells size:" <<cells.size();
}

void MainWindow::addTile()
{
    if ( isFindEmptyCell() ){
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


int MainWindow::getIndex(int x, int y)
{
    return y + ( x * FIELD_SIZE ) ;
}

/**
 * @brief MainWindow::moveCell
 * @param x coordinate new cell
 * @param y coordinate new cell
 * @param x1 coordinate old cell
 * @param y1 coordinate old cell
 */
void MainWindow::moveCell(int x,int y, int x1,int y1)
{
    if (  (cells.at( getIndex( x, y) )->text() == "" ) &&  (cells.at( getIndex( x1, y1 ) )->text() != "" ))
    {
        cells.at( getIndex( x, y ) )->setText( cells.at( getIndex( x1, y1 ) )->text() );
        cells.at( getIndex( x1, y1 ) )->setText("");
    }
    if (  (cells.at( getIndex( x, y ) )->text() != "" ) &&  (cells.at( getIndex( x1, y1 ) )->text() != "" ))
    {
        if (  cells.at( getIndex( x, y ) )->text() == cells.at( getIndex( x1, y1 ) )->text() )
        {
            cells.at( getIndex( x, y ) )->setText( QString::number( cells.at( getIndex( x1, y1) )->text().toInt() * 2 ) );
            cells.at( getIndex( x1, y1 ) )->setText("");
        }
    }
}

void MainWindow::moveUp()
{
    for (int x = FIELD_SIZE - 1; x > 0; x--){
        for (int y = 0; y <  FIELD_SIZE; y++){
            moveCell(x-1, y , x, y );
        }
    }
    if (isFindEmptyCell())
    {
        this->addTile();
    }

}

void MainWindow::moveDown()
{
    for (int x = 0; x < FIELD_SIZE - 1; x++){
        for (int y = 0; y <  FIELD_SIZE; y++){
            moveCell(x+1, y , x, y );
        }
    }

    if (isFindEmptyCell())
    {
        this->addTile();
    }
}

void MainWindow::moveLeft()
{
    for (int x = 0; x < FIELD_SIZE; x++){
        for (int y = FIELD_SIZE - 1; y > 0; y--){
            moveCell(x, y-1 , x, y );
        }
    }

    if (isFindEmptyCell())
    {
        this->addTile();
    }

}

void MainWindow::moveRigth()
{
    for (int x = 0; x < FIELD_SIZE; x++){
        for (int y = 0 ; y < FIELD_SIZE - 1; y++){
            moveCell(x, y+1 , x, y );
        }
    }
    if (isFindEmptyCell())
    {
        this->addTile();
    }
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
    int value = (int) pow(2,new_step);

    return QString::number(value);

}

void MainWindow::clearGameField()
{
    foreach (QLabel *item, cells) {
        item->setText("");
        item->setAlignment(Qt::AlignCenter);
        item->setStyleSheet("QLabel { background-color: #cdc1b5; color: black; font:20pt; font-weight:400; border-radius: 5px;}");
    }
}

bool MainWindow::isFindEmptyCell()
{
    foreach (QLabel *item, cells) {
        if (item->text() == "")
        {
            return true;
        }
    }
    return false;
}


// ----------------------- PUBLIC SLOTS -----------------------------------
void MainWindow::startGame()
{
    this->clearGameField();
    this->addTile();
}

// ----------------------- Protected  --------------------------------------

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (event->type() == QEvent::KeyPress)
    {

        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        qDebug() << "key " << keyEvent->key() << "from" << obj;
        switch (keyEvent->key()) {
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
    }
    return QObject::eventFilter(obj, event);
}

