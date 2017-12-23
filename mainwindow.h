#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>
#include <QLabel>
#include <QKeyEvent>
#include <QDebug>
#include <math.h>

namespace Ui {
class MainWindow;
}


// @TODO replace define to setter and getter. Save values to preference? change from menu

#define FIELD_SIZE 4
#define MAX_POW 3

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void startGame(); // @TODO add parametr int = size {4,5,6} size of game filed

private:
    Ui::MainWindow *ui;
    QVector <QLabel *> cells;

    void createGameField();
    void addTile();
    void moveUp();
    void moveDown();
    void moveCell(int x,int y, int x1,int y1);
    void clearGameField();
    QString getTwoInRandomPow();
    bool isFindEmptyCell();
    int getRandomIndex();
    int getIndex(int x, int y);

protected:
    bool eventFilter(QObject *obj, QEvent *event);
};

#endif // MAINWINDOW_H
