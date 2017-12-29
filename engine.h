#ifndef ENGINE_H
#define ENGINE_H

#include <QObject>
#include <QVector>
#include <QDebug>

#include <math.h>


class Engine : public QObject
{
    Q_OBJECT
public:
    explicit Engine(QObject *parent = 0);

    void clearCells();

    void setCellValue(int index, int value);
    void addEmptyCellValue();

    int  getCellValue(int index);

    void setFieldSize(int fsize);
    int  getFieldSize();

    int  getIndex(int row, int col);
    int  getRandomIndex();

   QString getTwoInRandomPow();

signals:
    void canUndo(bool state);

public slots:
    void undo();

private:
    QVector <int> currentCells;
    QVector <int> undoCells;
    int fieldsize;

    int findMaxCell();
};

#endif // ENGINE_H
