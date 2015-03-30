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
    Cell(); // Cell constructer defines adjecents(pointers) NULL and defines the state of the Cell dead

    Cell *getLeft() const; // returns *left cell

    Cell *getRight() const; // returns *right cell

    Cell *getUp() const; // returns *up cell

    Cell *getDown() const; // returns *down cell

    cellState const &getState() const; // returns cell state(dead or alive)

    void setLeft(Cell *left); // sets left cell

    void setUp(Cell *up); // sets up cell

    void setRight(Cell *right); // sets right cell

    void setDown(Cell *down); // sets down cell

    void setState(cellState const &state); // sets cell state
};
#endif //_LIFE_CELL_H_
