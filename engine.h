#ifndef ENGINE_H
#define ENGINE_H

#include <QObject>
#include <QVector>
#include <QDebug>

class Engine : public QObject
{
    Q_OBJECT
public:
    explicit Engine(QObject *parent = 0);

    void setCellValue(int index, int value);
    void addEmptyCellValue();

    int  getCellValue(int index);

    void setFieldSize(int fsize);
    int  getFieldSize();

    int  getIndex(int row, int col);
    int  getRandomIndex();

signals:
    void canUndo(bool state);

public slots:
    void undo();

private:
    QVector <int> undoCells;
    int fieldsize;
};

#endif // ENGINE_H
