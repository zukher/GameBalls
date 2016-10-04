#include "Board.h"
#include "Cell.h"

Board::Board()
{
    _dimension = 0;
    _countBalls = 0;
}

Board::Board(const Board &other)
{
    _dimension = other._dimension;
    _countBalls = other._countBalls;
    init();

    for (int i = 0; i < _dimension; ++i)
        for (int j = 0; j < _dimension; ++j)
        {
            if (other._board[i][j] == NULL)
                _board[i][j] = NULL;
            else
                _board[i][j] = new Cell(*(other._board[i][j]));
        }
}

Board::~Board()
{
    //destroy cells
    for (int i = 0; i < _dimension; ++i)
        for (int j = 0; j < _dimension; ++j)
            if (_board[i][j] != NULL)
                delete _board[i][j];
}

// create board with cells
bool Board::init()
{
    bool resultInit = true;

    _board.resize(_dimension);
    for (int i = 0; i < _dimension; ++i)
        _board[i].resize(_dimension);

    for (int i = 0; i < _dimension; ++i)
        for (int j = 0; j < _dimension; ++j)
        {
            Cell *cell = createCell();
            if (cell != NULL)
                _board[i][j] = cell;
            else
            {
                resultInit = false;
                return resultInit;
            }
        }

    return resultInit;
}

// load board from inputOptions
bool Board::load(const QString &inputOptions)
{
    bool resultLoad = false;

    if (inputOptions.isEmpty())
        return resultLoad;

    QStringList listOptions;
    listOptions = inputOptions.split(' ');
    int countOptions = listOptions.count();

    _dimension = listOptions.takeFirst().toInt();
    if (_dimension > 0 && countOptions > 3)
    {
        resultLoad = init();
        _countBalls = listOptions.takeFirst().toInt();
        int countHoles = _countBalls;
        int countWalls = listOptions.takeFirst().toInt();

        int countNextOptions = _countBalls*2 + countHoles*2 + countWalls*4;

        if ((countOptions == 3 + countNextOptions) &&
                (_countBalls > 0))
        {
            resultLoad &= setObjects(BALL, _countBalls, listOptions);

            resultLoad &= setObjects(HOLE, countHoles, listOptions);

            resultLoad &= setWalls(countWalls, listOptions);
        }
        else
            resultLoad = false;
    }

    return resultLoad;
}


// create objects on the board
bool Board::setObjects(Object object, int countObjects,
                       QStringList &listOptions)
{
    bool resultSetObjects = true;

    for (int k = 1; k <= countObjects; ++k)
    {
        int j = listOptions.takeFirst().toInt() - 1;
        int i = listOptions.takeFirst().toInt() - 1;

        if (isValidPosition(i, j) && _board[i][j]->noObject())
            _board[i][j]->setObject(object, k);
        else
        {
            resultSetObjects = false;
            break;
        }
    }

    return resultSetObjects;
}

// create walls on the board
bool Board::setWalls(int countWalls, QStringList &listOptions)
{
    bool resultSetWalls = true;

    for (int k = 1; k <= countWalls; ++k)
    {
        int j1 = listOptions.takeFirst().toInt() - 1;
        int i1 = listOptions.takeFirst().toInt() - 1;
        int j2 = listOptions.takeFirst().toInt() - 1;
        int i2 = listOptions.takeFirst().toInt() - 1;

        if (isValidPosition(i1, j1) &&
                isValidPosition(i2, j2))
        {
            // Horizontal wall
            if ((i1 == i2) && (j2 == j1 + 1))
            {
                _board[i1][j1]->addWall(WALL_RIGHT);
                _board[i2][j2]->addWall(WALL_LEFT);
            }

            // Vertical wall
            if ((i2 == i1 + 1) && (j1 == j2))
            {
                _board[i1][j1]->addWall(WALL_DOWN);
                _board[i2][j2]->addWall(WALL_UP);
            }
        }
        else
        {
            resultSetWalls = false;
            break;
        }
    }

    return resultSetWalls;
}


bool Board::move(Direction direction)
{
    _boardLose = false;
    bool canMove = false;

    switch(direction)
    {
    case UP:
        for (int i = 0; i < _dimension; ++i)
            for (int j = 0; j < _dimension; ++j)
                canMove |= moveUp(i, j);
        break;
    case DOWN:
        for (int i = _dimension - 1; i >= 0; --i)
            for (int j = 0; j < _dimension; ++j)
                canMove |= moveDown(i, j);
        break;
    case LEFT:
        for (int i = 0; i < _dimension; ++i)
            for (int j = 0; j < _dimension; ++j)
                canMove |= moveLeft(i, j);
        break;
    case RIGHT:
        for (int i = 0; i < _dimension; ++i)
            for (int j = _dimension - 1; j >= 0; --j)
                canMove |= moveRight(i, j);
        break;
    }

    if (_boardLose)
        canMove = false;

    return canMove;
}


bool Board::moveRight(int i, int j)
{
    bool canMove = false;
    Cell *cell = _board[i][j];

    // move only balls
    if (cell->getObject() == BALL &&
            !cell->containsWall(WALL_RIGHT))
    {
        int newj = j + 1;

        // keep going in right until we encounter barrier or get out bounds
        while(isValidPosition(i, newj) &&
              !_board[i][newj]->containsBarrier(WALL_RIGHT))
        {
            newj++;
        }

        // out of bounds
        if (!isValidPosition(i, newj))
        {
            if (j != newj - 1)
            {
                _board[i][_dimension - 1]->setObject(*cell);
                cell->removeObject();
                canMove = true;
            }
        }
        // collision
        else
        {
            Cell *newCell = _board[i][newj];

            // collision with cell of hole
            canMove = collisionHole(cell, newCell);

            // collision with cell of ball
            if (newCell->getObject() == BALL)
            {
                if (j != newj - 1)
                {
                    _board[i][newj - 1]->setObject(*cell);
                    cell->removeObject();
                    canMove = true;
                }
            }
            else
                // collision with cell of barrier
                if (newCell->containsWall(WALL_RIGHT))
                {
                    newCell->setObject(*cell);
                    cell->removeObject();
                    canMove = true;
                }
        }
    }

    return canMove;
}

// collision with cell of hole
bool Board::collisionHole(Cell *cell, Cell *newCell)
{
    bool canCollision = false;

    if (newCell->getObject() == HOLE)
    {
        if (newCell->compareId(*cell))
        {
            newCell->removeObject();
            cell->removeObject();
            canCollision = true;
            _countBalls--;
        }
        else
            _boardLose = true;
    }

    return canCollision;
}


bool Board::moveLeft(int i, int j)
{
    bool canMove = false;
    Cell *cell = _board[i][j];

    // move only balls
    if (cell->getObject() == BALL &&
            !cell->containsWall(WALL_LEFT))
    {
        int newj = j - 1;

        // keep going in left until we encounter barrier or get out bounds
        while(isValidPosition(i, newj) &&
              !_board[i][newj]->containsBarrier(WALL_LEFT))
        {
            newj--;
        }

        // out of bounds
        if (!isValidPosition(i, newj))
        {
            if (newj + 1 != j)
            {
                _board[i][0]->setObject(*cell);
                cell->removeObject();
                canMove = true;
            }
        }
        // collision
        else
        {
            Cell *newCell = _board[i][newj];

            // collision with cell of hole
            canMove = collisionHole(cell, newCell);

            // collision with cell of ball
            if (newCell->getObject() == BALL)
            {
                _board[i][newj + 1]->setObject(*cell);
                cell->removeObject();
                canMove = true;
            }
            else
                // collision with cell of barrier
                if (newCell->containsWall(WALL_LEFT))
                {
                    newCell->setObject(*cell);
                    cell->removeObject();
                    canMove = true;
                }
        }
    }

    return canMove;
}

bool Board::moveUp(int i, int j)
{
    bool canMove = false;

    Cell *cell = _board[i][j];

    // move only balls
    if (cell->getObject() == BALL &&
            !cell->containsWall(WALL_UP))
    {
        int newi = i - 1;

        // keep going in up until we encounter barrier or get out bounds
        while(isValidPosition(newi, j) &&
               !_board[newi][j]->containsBarrier(WALL_UP))
        {
            newi--;
        }

        // out of bounds
        if (!isValidPosition(newi, j))
        {
            if (newi + 1 != i)
            {
                _board[0][j]->setObject(*_board[i][j]);
                cell->removeObject();
                canMove = true;
            }
        }
        // collision
        else
        {
            Cell *newCell = _board[newi][j];

            // collision with cell of hole
            canMove = collisionHole(cell, newCell);

            // collision with cell of ball
            if (newCell->getObject() == BALL)
            {
                _board[newi + 1][j]->setObject(*cell);
                cell->removeObject();
                canMove = true;
            }
            else
                // collision with cell of barrier
                if (newCell->containsWall(WALL_UP))
                {
                    newCell->setObject(*cell);
                    cell->removeObject();
                    canMove = true;
                }
        }
    }

    return canMove;
}

bool Board::moveDown(int i, int j)
{
    bool canMove = false;

    Cell *cell = _board[i][j];

    // move only balls
    if (cell->getObject() == BALL &&
            !cell->containsWall(WALL_DOWN))
    {
        int newi = i + 1;

        // keep going in down until we encounter barrier or get out bounds
        while(isValidPosition(newi, j) &&
               !_board[newi][j]->containsBarrier(WALL_DOWN))
        {
            newi++;
        }

        // out of bounds
        if (!isValidPosition(newi, j))
        {
            if (newi - 1 != i)
            {
                _board[_dimension - 1][j]->setObject(*_board[i][j]);
                cell->removeObject();
                canMove = true;
            }
        }
        // collision
        else
        {
            Cell *newCell = _board[newi][j];

            // collision with cell of hole
            canMove = collisionHole(cell, newCell);

            // collision with cell of ball
            if (newCell->getObject() == BALL)
            {
                if (newi - 1 != i)
                {
                    _board[newi - 1][j]->setObject(*cell);
                    cell->removeObject();
                    canMove = true;
                }
            }
            else
                // collision with cell of barrier
                if (newCell->containsWall(WALL_DOWN))
                {
                    newCell->setObject(*cell);
                    cell->removeObject();
                    canMove = true;
                }
        }
    }
//    print();
    return canMove;
}

bool Board::isValidPosition(int i, int j)
{
    return ((0 <= i) && (i < _dimension) &&
            (0 <= j) && (j < _dimension));
}

Cell *Board::createCell()
{
    return new Cell;
}

void Board::print()
{
    for (int i = 0; i < _dimension; ++i)
    {
        for (int j = 0; j < _dimension; ++j)
            printf("%i ", _board[i][j]->getObject());
        printf("\n");
    }

}

Cell* Board::getCell(int i, int j)
{
    Cell *cell = NULL;

    if (isValidPosition(i, j))
        cell = _board[i][j];

    return cell;
}

int Board::getCountBalls()
{
    return _countBalls;
}

int Board::getDimension()
{
    return _dimension;
}
