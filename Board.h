#ifndef BOARD_H
#define BOARD_H

#include <QByteArray>
#include <QStringList>
#include <QVector>
#include "Cell.h"

enum Direction { UP, RIGHT, DOWN, LEFT };

class Cell;

class Board
{
public:
    Board();
    Board(const Board &other);
    ~Board();

    bool load(const QString &inputParametrs);
    bool move(Direction direction);

    int getCountBalls();
    int getDimension();

    Cell *getCell(int i, int j);

private:
    QVector<QVector<Cell*> > _board;
    int _dimension;
    int _countBalls;
    bool _boardLose;

    bool init();
    bool setObjects(Object object, int countObjects,
                    QStringList &listOptions);
    bool setWalls(int countWalls, QStringList &listOptions);

    Cell *createCell();

    bool isValidPosition(int i, int j);

    bool moveRight(int i, int j);
    bool moveLeft(int i, int j);
    bool moveUp(int i, int j);
    bool moveDown(int i, int j);

    bool collisionHole(Cell *cell, Cell *newCell);

    void print();
};

#endif // BOARD_H
