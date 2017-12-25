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


// @ToDo replace define to setter and getter. Save values to preference? change from menu
#define FIELD_SIZE 4
// @ToDo replace define to menu item
#define MAX_POW 3

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

signals:
    void canUndo(bool state);
    void valueScoreChanged(int newValue);

public slots:
    void startGame(); // @TODO add parametr int = size {4,5,6} size of game filed
    void undo();
    void keyReleaseEvent(QKeyEvent *event);


private:
    Ui::MainWindow *ui;
    QVector <QLabel *> cells;
    QVector <int> undoCells;
    int mScore;

    void createGameField();
    void addTile();
    void moveUp();
    void moveDown();
    void moveLeft();
    void moveRigth();
    void paintField();
    bool moveCell(int x,int y, int x1,int y1);
    bool moveCellInColumn(int col,bool direction);
    bool moveCellInRow(int row,bool direction);
    void clearGameField();
    QString getTwoInRandomPow();
    bool isFindEmptyCell();
    int getRandomIndex();
    int getIndex(int x, int y);
    void printDebugField(QString direction);
    void addScore(int score);
    void saveStateForUndo();
};

#endif // MAINWINDOW_H
