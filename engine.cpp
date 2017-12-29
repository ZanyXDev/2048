#include "engine.h"

Engine::Engine(QObject *parent) : QObject(parent)
{

}

void Engine::setFieldSize(int fsize)
{
    fieldsize = fsize;
    currentCells.clear();
    undoCells.clear();
}

int Engine::getFieldSize()
{
    return fieldsize;
}

void Engine::setCellValue(int index, int value)
{
    if (index > 0  && index <= undoCells.count())
    {
       undoCells.replace( index, value );
    }
}

int Engine::getCellValue(int index)
{
    if (index > 0  && index <= undoCells.count())
    {
        return undoCells.at(index);
    }
    return 0;
}

void Engine::addEmptyCellValue()
{
    undoCells.append(0);
}

int Engine::getIndex(int row, int col)
{
    return col + ( row * fieldsize ) ;
}

int Engine::getRandomIndex()
{
    qDebug() << Q_FUNC_INFO;
    random();
    return getIndex ( (rand() % fieldsize), (rand() % fieldsize) );
}

QString Engine::getTwoInRandomPow()
{
    qDebug() << Q_FUNC_INFO;
    int value = findMaxCell();
    if (value > 2)
    {
        value = log( value / 2) / log( 2 );
    }else
    {
        value = 3;
    }

    random();
    // For select max pow find max number in field, divide on the two and get log2(value)
    int new_step = rand() % value;
    if (new_step == 0)
    {
        new_step = 1;
    }
    qDebug()<< "value:"<< value << "random:"<< pow(2,new_step);
    return QString::number((int) pow(2,new_step));
}

// --------------------------------------- PUBLIC SLOTS ---------------------
void Engine::undo()
{
    emit canUndo(false);
}

//------------------------------------ PRIVATE ------------------------------
int Engine::findMaxCell()
{
    qDebug() << Q_FUNC_INFO;
    int value = 0;
    foreach (int item, currentCells) {
        if (item > value)
        {
            value = item;
        }
    }
    return value;
}
