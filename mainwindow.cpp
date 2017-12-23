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

void MainWindow::moveUp()
{
    bool changed = false;

    for (int x = FIELD_SIZE - 1; x > 0; x--){
        for (int y = 0; y <  FIELD_SIZE; y++){
            if (  (cells.at( getIndex( x-1, y ) )->text() == "" ) &&  (cells.at( getIndex( x, y ) )->text() != "" ))
            {
                cells.at( getIndex( x-1, y ) )->setText( cells.at( getIndex( x, y ) )->text() );
                cells.at( getIndex( x, y ) )->setText("");
                changed = true;
            }
            if (  (cells.at( getIndex( x-1, y ) )->text() != "" ) &&  (cells.at( getIndex( x, y ) )->text() != "" ))
            {
                if (  cells.at( getIndex( x-1, y ) )->text() == cells.at( getIndex( x, y ) )->text() )
                {
                    cells.at( getIndex( x-1, y ) )->setText( QString::number( cells.at( getIndex( x, y ) )->text().toInt() * 2 ) );
                    cells.at( getIndex( x, y ) )->setText("");
                    changed = true;
                }
            }
        }
    }
    if (!changed)
    {
        qDebug()<< "can't move!";
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
            this->addTile();
            break;
        default:
            break;
        }
    }
    return QObject::eventFilter(obj, event);
}

