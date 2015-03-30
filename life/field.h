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

    int getAliveCells() const {
        return aliveCells;
    }

    int getHeight() const {
        return height;
    }

    int getWidth() const {
        return width;
    }

    Cell *getHead() const {
        return head;
    }

    // returns pointer to the Cell at xth row and yth column
    Cell *getCell(int const &y, int const &x);

    void moveLeft();

    void moveRight();

    void moveUp();

    void moveDown();

    // changes the state of the given Cell to the given state
    void setCellState(Cell *cell, cellState const &state);

    // draws the field. "." means dead Cell, whereas "#" alive
    void Draw();

    //checks if the field has correct structure. just for debugging
    void Check();

    // creats a column and stick it to the left or right side of the field depending on the given flag
    void buildVertical(Cell *_i, Cell *_j, bool flag);

    // creats a row and stick it to the up or down side of the field depending on the given flag
    void buildHorizontal(Cell *_i, Cell *_j, bool flag);

    // expands the left side of the field sticking dead cells
    void expandLeft();

    // expands the right side of the field sticking dead cells
    void expandRight();

    // expands the upper side of the field sticking dead cells
    void expandUp();

    // expands the lower side of the field sticking dead cells
    void expandDown();

    // deletes the left or right side of the field depending on the given flag
    void deleteVertical(Cell *_i, bool flag);

    // deletes the upper or lower side of the field depending on the given flag
    void deleteHorizontal(Cell *_i, bool flag);

    // shrinks the left side of the field
    void shrinkLeft();

    // shrinks the right side of the field
    void shrinkRight();

    // shrinks the upper side of the field
    void shrinkUp();

    // shrinks the lower side of the field
    void shrinkDown();

    // returns amount of alive adjecents of the given Cell;
    int aliveAdjecents(Cell *cell);
};
#endif //_LIFE_FIELD_H_
