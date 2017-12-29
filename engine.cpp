#include "engine.h"

Engine::Engine(QObject *parent) : QObject(parent)
{

}

void Engine::setFieldSize(int fsize)
{
    fieldsize = fsize;
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
// --------------------------------------- PUBLIC SLOTS ---------------------
void Engine::undo()
{
    emit canUndo(false);
}
