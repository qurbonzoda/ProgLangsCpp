/**
* Created by Abduqodir on 20.03.2015.
* The file defines class Field. Field has height and width and consists of Cells.
* Also Field has aliveCells which stores amount of Cells with alive state and
* pointer head refering to the leftmost and upmost cell. Each variable has getter and setter
*/


#ifndef _LIFE_FIELD_H_
#define _LIFE_FIELD_H_

#include <iostream>
#include <assert.h>
#include "cell.h"

using namespace std;

class Field {
    int width, height, aliveCells;
    int visionX, visionY, monitorSize;
    Cell *head;

public:
    // Class constructer : creats height * width Cells which states are dead.
    // head refers to the leftmost and upmost cell
    Field(int height, int width);

    int getAliveCells() const; // returns amount of populated cells

    int getHeight() const; // returns height of the field

    int getWidth() const; // returns height of the field

    Cell *getHead() const; // returns upmost and leftmost cell of the field

    Cell *getCell(int const &y, int const &x); // returns pointer to the Cell at xth row and yth column

    void moveLeft(); // moves the viewer left

    void moveRight(); // moves the viewer right

    void moveUp(); // moves the viewer up

    void moveDown(); // moves the viewer down

    void setCellState(Cell *cell, cellState const &state); // changes the state of the given Cell to the given state

    void Draw(); // draws the field. "." means dead Cell, whereas "#" alive

    void Check(); //checks if the field has correct structure. just for debugging

    void buildVertical(Cell *_i, Cell *_j, bool flag); // creats a column and stick it to the left or right side of the field depending on the given flag

    void buildHorizontal(Cell *_i, Cell *_j, bool flag); // creats a row and stick it to the up or down side of the field depending on the given flag

    void expandLeft(); // expands the left side of the field sticking dead cells

    void expandRight(); // expands the right side of the field sticking dead cells

    void expandUp(); // expands the upper side of the field sticking dead cells

    void expandDown(); // expands the lower side of the field sticking dead cells

    void deleteVertical(Cell *_i, bool flag); // deletes the left or right side of the field depending on the given flag

    void deleteHorizontal(Cell *_i, bool flag); // deletes the upper or lower side of the field depending on the given flag

    void shrinkLeft(); // shrinks the left side of the field

    void shrinkRight(); // shrinks the right side of the field

    void shrinkUp(); // shrinks the upper side of the field

    void shrinkDown(); // shrinks the lower side of the field

    int aliveAdjecents(Cell *cell); // returns amount of alive adjecents of the given Cell;
};
#endif //_LIFE_FIELD_H_
