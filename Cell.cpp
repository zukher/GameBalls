#include "Cell.h"

Cell::Cell()
{
    _id = 0;
    _object = OBJECT_NONE;
    _listWalls.clear();
}

Cell::Cell(const Object &object, int id)
{
    setObject(object, id);
}

Cell::Cell(const Cell &other)
{
    setObject(other);
    _listWalls = other._listWalls;
}

void Cell::setObject(const Cell &other)
{
    _id = other._id;
    _object = other._object;
}

void Cell::setObject(const Object &object, int id)
{
    _id = id;
    _object = object;
}

bool Cell::noObject()
{
    return (_object == OBJECT_NONE);
}

Object &Cell::getObject()
{
    return _object;
}

void Cell::removeObject()
{
    _object = OBJECT_NONE;
    _id = 0;
}

void Cell::addWall(const Wall &wall)
{
    _listWalls << wall;
}

QList<Wall> &Cell::getWalls()
{
    return _listWalls;
}

bool Cell::containsWall(const Wall &wall)
{
    return _listWalls.contains(wall);
}

bool Cell::containsBarrier(const Wall &wallBarrier)
{
    return (_object != OBJECT_NONE ||
            containsWall(wallBarrier));
}

bool Cell::compareId(const Cell &other)
{
    return (_id == other._id);
}

int Cell::getId()
{
    return _id;
}
