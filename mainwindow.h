#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenuBar>
#include <QVector>
#include <QLabel>
#include <QMenu>
#include <QAction>
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
    void undo();  
    void doNewGame_4x4();
    void doNewGame_5x5();
    void doNewGame_6x6();

private:
    Ui::MainWindow *ui;
    QVector <QLabel *> cells;
    QVector <int> undoCells;
    int mScore;
    int fieldsize;

    QMenu *gameMenu;
    QMenu *helpMenu;
    QMenu *settingsMenu;

    QAction *newGame_4x4;
    QAction *newGame_5x5;
    QAction *newGame_6x6;

    void createGameField(int fsize);

    void deleteGameField();

    bool isClearGameField();
    void addTile();
    void moveUp();
    void moveDown();
    void moveLeft();
    void moveRigth();
    void paintField();
    bool moveCell(int x,int y, int x1,int y1);
    void moveCellsHorizontal(bool direction);
    void moveCellsVertical(bool direction);


    QString getTwoInRandomPow();
    bool isFindCell(QString cType);
    int findMaxCell();
    int getRandomIndex();
    int getIndex(int x, int y);
    void printDebugField(QString direction);
    void addScore(int score);
    void saveStateForUndo();

    void createMenuAction();
    void createMenus();
    void createConnection();

    void startGame();

protected:
    void keyReleaseEvent(QKeyEvent *event);
};

#endif // MAINWINDOW_H
