#ifndef GAMEPLAY_H
#define GAMEPLAY_H

#include <QStringList>
#include "Board.h"

class GamePlay
{
public:
    GamePlay();
    bool setInputOptions(const QString&);
    void findSolution(const Board &board, const Direction &direction, const QString &solution);
    void play();

private:
    Board _board;
    int _minSizeSolution;
    QStringList _listSolutions;
    QList<Direction> _listDirections;

    char getSymbol(const Direction &direction);
    void setMinSolution(const QString &solution);
    void analyseBoard();
    void fullDirectionsList();
    void setFirstDirection(int iFirstBall, int jFirstBall, int iFirstHole, int jFirstHole);

};

#endif // GAMEPLAY_H
