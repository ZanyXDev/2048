#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->createGameField();
    QObject::connect(ui->newGameBtn,SIGNAL(clicked()),this,SLOT(startGame()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

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
}


// ----------------------- PUBLIC SLOTS -----------------------------------
void MainWindow::startGame()
{
    this->addTile();
}

void MainWindow::addTile()
{
    if ( isFindEmptyCell() ){
        bool isSuccessSetTile = false;
        do{
            int index = getRandomIndex();
            if (cells.at(index)->text() == "" ){
                cells.at(index)->setText(getTwoInRandomPow());
                isSuccessSetTile = true;
            }
        } while (!isSuccessSetTile);
    }

}


// ----------------------- Protected  --------------------------------------
int MainWindow::getRandomIndex()
{
    random();
    int posX = rand() % FIELD_SIZE;
    int posY = rand() % FIELD_SIZE;
    return (posX + (posY * FIELD_SIZE));
}

QString MainWindow::getTwoInRandomPow()
{
    random();
    int new_step = rand() % MAX_POW;
    if (new_step == 0){
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
        if (item->text() == ""){
            return true;
        }
    }
    return false;

}

