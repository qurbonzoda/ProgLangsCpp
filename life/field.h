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
    Cell *head;

    public:
    // Class constructer : creats height * width Cells which states are dead.
    // head refers to the leftmost and upmost cell
    Field(int height, int width) : height(height), width(width) {
        head = new Cell();
        aliveCells = 0;
        Cell *_i, *_j, *_k;

        _i = head;
        for (int i = 0; i < Field::height; ++i) {
            if (i > 0) {
                _k = _i;
                _i->setDown(new Cell());
                _i->getDown()->setUp(_i);
                _i = _i->getDown();
            }
            _j = _i;
            for (int j = 1; j < Field::width; ++j) {
                _j->setRight(new Cell());
                _j->getRight()->setLeft(_j);
                _j = _j->getRight();
                if (i > 0) {
                    _k = _k->getRight();
                    _j->setUp(_k);
                    _k->setDown(_j);
                }
            }
        }
    }

    // returns pointer to the Cell at xth row and yth column
    Cell *getCell(int const &x, int const &y) {
        Cell *_i;
        _i = head;
        for (int i = 0; i < x; i++) {
            _i = _i->getDown();
        }
        for (int i = 0; i < y; ++i) {
            _i = _i->getRight();
        }
        return _i;
    }

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

    // changes the state of the given Cell to the given state
    void setCellState(Cell *cell, cellState const &state) {
        if (state == _alive) {
            if (cell->getState() == _dead) {
                cell->setState(_alive);
                aliveCells++;
            }
        }
        else {
            if(cell->getState() == _alive) {
                cell->setState(_dead);
                aliveCells--;
            }
        }
    }

    // draws the field. 0 means dead Cell, whereas 1 alive
    void Draw() {
        Cell *_i = head;
        for (int i = 0; i < height; ++i, _i = _i->getDown()) {
            Cell *_j = _i;
            for (int j = 0; j < width; ++j, _j = _j->getRight()) {
                cout << _j->getState() << " ";
            }
            cout << endl;
        }
    }

    //checks if the field has correct structure. just for debugging
    void Check() {
        Cell *_i = head;
        for (int i = 0; i < height; ++i, _i = _i->getDown()) {
            Cell *_j = _i;
            for (int j = 0; j < width - 1; ++j, _j = _j->getRight()) {
                assert (_j != NULL);
                assert (_j->getRight() != NULL);
                assert (_j->getRight()->getLeft() == _j);
                if (i > 0) {
                    assert (_j->getUp() != NULL);
                    assert (_j->getUp()->getDown() == _j);
                }
            }
            assert (_j != NULL);
            if (i > 0) {
                assert (_j->getUp() != NULL);
                assert (_j->getUp()->getDown() == _j);
            }
        }
    }

    // creats a column and stick it to the left or right side of the field depending on the given flag
    void buildVertical(Cell *_i, Cell *_j, bool flag) {
        for(int i = 1; i < height; i++) {
            assert (_i != NULL && _j != NULL);
            _i->setDown(new Cell());
            _i->getDown()->setUp(_i);
            _i = _i->getDown();
            _j = _j->getDown();
            assert (_i != NULL && _j != NULL);
            if (flag) {
                _i->setRight(_j);
                _j->setLeft(_i);
            }
            else {
                _i->setLeft(_j);
                _j->setRight(_i);
            }
        }
    }

    // creats a row and stick it to the up or down side of the field depending on the given flag
    void buildHorizontal(Cell *_i, Cell *_j, bool flag) {
        for(int i = 1; i < width; i++) {
            assert (_i != NULL && _j != NULL);
            _i->setRight(new Cell());
            _i->getRight()->setLeft(_i);
            _i = _i->getRight();
            _j = _j->getRight();
            assert (_i != NULL && _j != NULL);
            if (flag) {
                _i->setDown(_j);
                _j->setUp(_i);
            }
            else {
                _i->setUp(_j);
                _j->setDown(_i);
            }
        }
    }

    // expands the left side of the field sticking dead cells
    void expandLeft() {
        Cell *_i = head;
        _i->setLeft(new Cell());
        _i->getLeft()->setRight(_i);
        _i = _i->getLeft();
        head = _i;
        Cell *_j = _i->getRight();
        buildVertical(_i, _j, 1);
        width++;
    }

    // expands the right side of the field sticking dead cells
    void expandRight() {
        Cell *_i = head;
        for(int i = 1; i < width; i++) {
            assert (_i != NULL);
            _i = _i->getRight();
        }
        assert (_i != NULL);
        _i->setRight(new Cell());
        _i->getRight()->setLeft(_i);
        _i = _i->getRight();
        Cell *_j = _i->getLeft();
        buildVertical(_i, _j, 0);
        width++;
    }

    // expands the upper side of the field sticking dead cells
    void expandUp() {
        Cell *_i = head;
        _i->setUp(new Cell());
        _i->getUp()->setDown(_i);
        _i = _i->getUp();
        head = _i;
        Cell *_j = _i->getDown();
        buildHorizontal(_i, _j, 1);
        height++;
    }

    // expands the lower side of the field sticking dead cells
    void expandDown() {
        Cell *_i = head;
        for(int i = 1; i < height; i++) {
            assert (_i != NULL);
            _i = _i->getDown();
        }

        assert (_i != NULL);
        _i->setDown(new Cell());
        _i->getDown()->setUp(_i);
        _i = _i->getDown();
        Cell *_j = _i->getUp();
        buildHorizontal(_i, _j, 0);
        height++;
    }

    // deletes the left or right side of the field depending on the given flag
    void deleteVertical(Cell *_i, bool flag) {
        for(int i = 0; i < height; i++) {
            assert (_i != NULL);
            if (flag) {
                delete _i->getLeft();
                _i->setLeft(NULL);
            }
            else {
                delete _i->getRight();
                _i->setRight(NULL);
            }
        }
    }

    // deletes the upper or lower side of the field depending on the given flag
    void deleteHorizontal(Cell *_i, bool flag) {
        for(int i = 0; i < width; i++) {
            assert (_i != NULL);
            if (flag) {
                delete _i->getUp();
                _i->setUp(NULL);
            }
            else {
                delete _i->getDown();
                _i->setDown(NULL);
            }
        }
    }

    // shrinks the left side of the field
    void shrinkLeft() {
        head = head->getRight();
        Cell *_i = head;
        deleteVertical(_i, 1);
        width--;
    }

    // shrinks the right side of the field
    void shrinkRight() {
        Cell *_i = head;
        for(int i = 1; i < width - 1; i++) {
            assert (_i != NULL);
            _i = _i->getRight();
        }
        assert (_i != NULL);
        deleteVertical(_i, 0);
        width--;
    }

    // shrinks the upper side of the field
    void shrinkUp() {
        head = head->getDown();
        Cell *_i = head;
        deleteHorizontal(_i, 1);
        height--;
    }

    // shrinks the lower side of the field
    void shrinkDown() {
        Cell *_i = head;
        for(int i = 1; i < height - 1; i++) {
            assert (_i != NULL);
            _i = _i->getDown();
        }
        assert (_i != NULL);
        deleteHorizontal(_i, 0);
        height--;
    }

    // returns amount of alive adjecents of the given Cell;
    int aliveAdjecents(Cell *cell) {
        int res = (cell->getLeft() != NULL && cell->getLeft()->getState())
                + (cell->getRight() != NULL && cell->getRight()->getState())
                + (cell->getUp() != NULL && cell->getUp()->getState())
                + (cell->getDown() != NULL && cell->getDown()->getState())
                + (cell->getDown() != NULL && cell->getDown()->getLeft() != NULL && cell->getDown()->getLeft()->getState())
                + (cell->getDown() != NULL && cell->getDown()->getRight() != NULL && cell->getDown()->getRight()->getState())
                + (cell->getUp() != NULL && cell->getUp()->getLeft() != NULL && cell->getUp()->getLeft()->getState())
                + (cell->getUp() != NULL && cell->getUp()->getRight() != NULL && cell->getUp()->getRight()->getState());
        return res;
    }
};


#endif //_LIFE_FIELD_H_
