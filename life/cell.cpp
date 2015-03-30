//
// Created by Abduqodir on 30.03.2015.
//

#include "cell.h"

Cell::Cell() {
    state = _dead;
    left = right = up = down = NULL;
}

Cell *Cell::getLeft() const {
    return left;
}

Cell *Cell::getRight() const {
    return right;
}

Cell *Cell::getUp() const {
    return up;
}

Cell *Cell::getDown() const {
    return down;
}

cellState const &Cell::getState() const {
    return state;
}

void Cell::setLeft(Cell *left) {
    Cell::left = left;
}

void Cell::setUp(Cell *up) {
    Cell::up = up;
}

void Cell::setRight(Cell *right) {
    Cell::right = right;
}

void Cell::setDown(Cell *down) {
    Cell::down = down;
}

void Cell::setState(cellState const &state) {
    Cell::state = state;
}