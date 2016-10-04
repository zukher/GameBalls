#ifndef CELL_H
#define CELL_H

#include <QList>

enum Object { OBJECT_NONE, BALL, HOLE };

enum Wall { WALL_UP, WALL_DOWN, WALL_RIGHT, WALL_LEFT };

class Cell
{
public:
    Cell();
    Cell(const Object&, int id = 0);
    Cell(const Cell&);

    void setObject(const Cell&);
    void setObject(const Object&, int);
    bool noObject();
    Object &getObject();
    void removeObject();

    void addWall(const Wall&);
    QList<Wall> &getWalls();
    bool containsWall(const Wall&);
    bool containsBarrier(const Wall&);

    bool compareId(const Cell&);
    int getId();

private:
    int _id;
    Object _object;
    QList<Wall> _listWalls;
};

#endif // CELL_H
