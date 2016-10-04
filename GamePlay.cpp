#include "GamePlay.h"
#include "Cell.h"

GamePlay::GamePlay()
{
    _minSizeSolution = 0;
}

bool GamePlay::setInputOptions(const QString &inputOptions)
{
    bool statusLoadBoard = _board.load(inputOptions);
    if (statusLoadBoard)
    {
        int dimension = _board.getDimension();
        _minSizeSolution = dimension*dimension;
    }

    return statusLoadBoard;
}

void GamePlay::analyseBoard()
{
    int dimension = _board.getDimension();

    int iFirstBall = -1;
    int jFirstBall = -1;
    int iFirstHole = -1;
    int jFirstHole = -1;
    for (int i = 0; i < dimension; ++i)
        for (int j = 0; j < dimension; ++j)
        {
            Cell *cell = _board.getCell(i, j);
            if (cell != NULL &&
                    cell->getObject() == BALL && cell->getId() == 1)
            {
                iFirstBall = i;
                jFirstBall = j;

                if (iFirstHole != -1)
                    return setFirstDirection(iFirstBall, jFirstBall, iFirstHole, jFirstHole);
            }

            if (cell != NULL &&
                    cell->getObject() == HOLE && cell->getId() == 1)
            {
                iFirstHole = i;
                jFirstHole = j;

                if (iFirstBall != -1)
                    return setFirstDirection(iFirstBall, jFirstBall, iFirstHole, jFirstHole);
            }
        }
}

// set direction priority for first ball and first hole
void GamePlay::setFirstDirection(int iFirstBall, int jFirstBall, int iFirstHole, int jFirstHole)
{
    if (iFirstBall < iFirstHole)
        _listDirections << DOWN;
    if (iFirstBall > iFirstHole)
        _listDirections << UP;
    if (jFirstBall > jFirstHole)
        _listDirections << LEFT;
    if (jFirstBall < jFirstHole)
        _listDirections << RIGHT;
}


void GamePlay::fullDirectionsList()
{
    QList<Direction> listDirections;
    listDirections << RIGHT << DOWN << LEFT << UP;

    foreach(Direction direction, listDirections)
        if (!_listDirections.contains(direction))
            _listDirections << direction;
}

void GamePlay::play()
{
    analyseBoard();
    fullDirectionsList();

    foreach(Direction direction, _listDirections)
        findSolution(_board, direction, QString());

    foreach(QString solution, _listSolutions)
        printf("%s\n", solution.toLocal8Bit().constData());

    if (_listSolutions.isEmpty())
        printf("No solution\n");
}

void GamePlay::findSolution(const Board &board, const Direction &currentDirection, const QString &solution)
{
    Board newBoard(board);
    QString currentSolution = solution;

    if (!newBoard.move(currentDirection) || currentSolution.size() + 1 > _minSizeSolution)
        return;

    currentSolution.append(getSymbol(currentDirection));

    if (newBoard.getCountBalls() != 0)
    {

        foreach(Direction direction, _listDirections)
            if (direction != currentDirection)
                findSolution(newBoard, direction, currentSolution);
    }
    else
        setMinSolution(currentSolution);
}

char GamePlay::getSymbol(const Direction &direction)
{
    char symbol = ' ';

    switch (direction)
    {
    case UP:
        symbol = 'N';
        break;
    case DOWN:
        symbol = 'S';
        break;
    case LEFT:
        symbol = 'W';
        break;
    case RIGHT:
        symbol = 'E';
        break;
    }

    return symbol;
}

void GamePlay::setMinSolution(const QString &solution)
{
    int sizeSolution = solution.size();
    if (sizeSolution <= _minSizeSolution)
    {
        if (sizeSolution < _minSizeSolution)
            _listSolutions.clear();

        _listSolutions << solution;
        _minSizeSolution = sizeSolution;
    }
}
