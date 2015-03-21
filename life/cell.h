/**
* Created by Abduqodir on 19.03.2015.
* The file defines class Cell. Cell has pointers to it's adjecent(left, right, up, down) Cells if they exist.
* Also Cell has state, either alive or dead. Constructor of the class defines pointers NULL
* and defines the state of the Cell dead. Each variable has getter and setter.
*/

#ifndef _LIFE_CELL_H_
#define _LIFE_CELL_H_

#include <cstdlib>

enum cellState {_dead, _alive};

class Cell {
    Cell *left, *right, *up, *down;
    cellState state;

    public:

    Cell() {
        state = _dead;
        left = right = up = down = NULL;
    }

    Cell *getLeft() const {
        return left;
    }

    Cell *getRight() const {
        return right;
    }

    Cell *getUp() const {
        return up;
    }

    Cell *getDown() const {
        return down;
    }

    cellState const &getState() const {
        return state;
    }

    void setLeft(Cell *left) {
        Cell::left = left;
    }

    void setUp(Cell *up) {
        Cell::up = up;
    }

    void setRight(Cell *right) {
        Cell::right = right;
    }

    void setDown(Cell *down) {
        Cell::down = down;
    }

    void setState(cellState const &state) {
        Cell::state = state;
    }
};
#endif //_LIFE_CELL_H_
