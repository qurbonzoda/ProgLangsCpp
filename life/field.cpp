//
// Created by Abduqodir on 30.03.2015.
//

#include "field.h"

Field::Field(int height, int width) : height(height), width(width) {
    visionX = visionY = 0;
    monitorSize = 20;
    aliveCells = 0;
    head = new Cell();
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

int Field::getAliveCells() const {
    return aliveCells;
}

int Field::getHeight() const {
    return height;
}

int Field::getWidth() const {
    return width;
}

Cell *Field::getHead() const {
    return head;
}

Cell *Field::getCell(int const &y, int const &x) {
    Cell *_i;
    _i = head;
    for (int i = 0; i < y; i++) {
        _i = _i->getDown();
    }
    for (int i = 0; i < x; ++i) {
        _i = _i->getRight();
    }
    return _i;
}

void Field::moveLeft() {
    if (visionX > 0) {
        visionX--;
    }
    else {
        cout << "You are at left border" << endl;
    }
}

void Field::moveRight() {
    if (visionX + monitorSize < width) {
        visionX++;
    }
    else {
        cout << "You are at right border" << endl;
    }
}

void Field::moveUp() {
    if (visionY > 0) {
        visionY--;
    }
    else {
        cout << "You are at upper border" << endl;
    }
}

void Field::moveDown() {
    if (visionY + monitorSize < height) {
        visionY++;
    }
    else {
        cout << "You are at lower border" << endl;
    }
}

void Field::setCellState(Cell *cell, cellState const &state) {
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

void Field::Draw() {
    Cell *_i = getCell(visionY, visionX);
    for (int i = visionY; i < min(height, visionY + monitorSize); ++i, _i = _i->getDown()) {
        Cell *_j = _i;
        for (int j = visionX; j < min(width, visionX + monitorSize); ++j, _j = _j->getRight()) {
            if (_j->getState() == _dead) {
                cout << ".";
            }
            else {
                cout << "#";
            }
        }
        cout << endl;
    }
}

void Field::Check() {
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

void Field::buildVertical(Cell *_i, Cell *_j, bool flag) {
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

void Field::buildHorizontal(Cell *_i, Cell *_j, bool flag) {
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

void Field::expandLeft() {
    Cell *_i = head;
    _i->setLeft(new Cell());
    _i->getLeft()->setRight(_i);
    _i = _i->getLeft();
    head = _i;
    Cell *_j = _i->getRight();
    buildVertical(_i, _j, 1);
    width++;
}

void Field::expandRight() {
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

void Field::expandUp() {
    Cell *_i = head;
    _i->setUp(new Cell());
    _i->getUp()->setDown(_i);
    _i = _i->getUp();
    head = _i;
    Cell *_j = _i->getDown();
    buildHorizontal(_i, _j, 1);
    height++;
}

void Field::expandDown() {
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

void Field::deleteVertical(Cell *_i, bool flag) {
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

void Field::deleteHorizontal(Cell *_i, bool flag) {
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

void Field::shrinkLeft() {
    if (head != NULL) {
        head = head->getRight();
        Cell *_i = head;
        if (head != NULL) {
            deleteVertical(_i, 1);
        }
    }
    if (width > 0) {
        width--;
    }
}

void Field::shrinkRight() {
    if (head != NULL) {
        Cell *_i = head;
        for (int i = 1; i < width - 1; i++) {
            assert (_i != NULL);
            _i = _i->getRight();
        }
        assert (_i != NULL);
        deleteVertical(_i, 0);
    }
    if (width > 0) {
        width--;
    }
}

void Field::shrinkUp() {
    if (head != NULL) {
        head = head->getDown();
        Cell *_i = head;
        if (head != NULL) {
            deleteHorizontal(_i, 1);
        }
    }
    if (height > 0) {
        height--;
    }
}

void Field::shrinkDown() {
    if (head != NULL) {
        Cell *_i = head;
        for (int i = 1; i < height - 1; i++) {
            assert (_i != NULL);
            _i = _i->getDown();
        }
        assert (_i != NULL);
        deleteHorizontal(_i, 0);
    }
    if (height > 0) {
        height--;
    }
}

int Field::aliveAdjecents(Cell *cell) {
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