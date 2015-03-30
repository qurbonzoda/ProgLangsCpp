/**
* Created by Abduqodir on 20.03.2015.
* user inputs the size of the simulation field, the number of the alive Cells at the beginning,
* the number of simulation steps. Then user can select either to enter the coordinates of the
* alive Cells or let the programm choose them randomly.
* alive - populated
* dead - empty
* The programm performs the simulation steps in the following order.
*   In an empty cell, beside which exactly three populated emerges life
*   If populated cells has two or three populated adjacent it continues to live, otherwise it becomes empty
*   If at some step populated cell is at a border of the field, the field expands in this direction by one cell
*   If at some step all adjecent cells of a border are empty, the border will be deleted. This way
*   keeping the field in optimal size.
* Output
*   Number of populated cells every 10 steps
*   The average ratio born / died for every 10 steps
*   Population growth at the end of the simulation
*   Step number, after which the population size did not change (if it happens)
*   Field size and the current position at the end of the simulation (if user wants to) "." means dead Cell, whereas "#" alive
*/

#include <iostream>
#include <random>
#include <list>
#include <assert.h>
#include "field.h"
using namespace std;

// If a populated cell is at a border of the field, the field expands in this direction by one cell
void expand(int const &minH, int const &maxH, int const &minW, int const &maxW, int const &height, int const &width, Field *field) {
    if (minW == 0) {
        field->expandLeft();
    }
    if (maxW == width - 1) {
        field->expandRight();
    }
    if (minH == 0) {
        field->expandUp();
    }
    if (maxH == height - 1) {
        field->expandDown();
    }
}

// If all adjecent cells of a border are empty, the border will be deleted
void shrink(int const &minH, int const &maxH, int const &minW, int const &maxW, int const &height, int const &width, Field *field) {
    for (int i = 0; i < minW - 1; i++) {
        field->shrinkLeft();
    }
    for (int i = maxW + 2; i < width; i++) {
        field->shrinkRight();
    }
    for (int i = 0; i < minH - 1; i++) {
        field->shrinkUp();
    }
    for (int i = maxH + 2; i < height; i++) {
        field->shrinkDown();
    }
}

// gets leftmost, rightmost, upmost and downmost alive cell
void getMinMax(int &minH, int &maxH, int &minW, int &maxW, int const &h, int const &w) {
    minH = min (minH, h);
    minW = min (minW, w);
    maxH = max (maxH, h);
    maxW = max (maxW, w);
}

//the main function - starting point
int main() {

    int height, width, aliveCells, steps;
    cout << "Enter height, width, amount of alive cells of the field and amount of simulation steps in the corresponding order" << endl;
    cin >> height >> width >> aliveCells >> steps;

    if (aliveCells > height * width) {
        cout << "Impossible case" << endl;
        return 0;
    }
    Field *field = new Field(height, width);
    field->Check();
    int minH = height + 1, minW = width + 1, maxH = -2, maxW = -2;

    selection:
    cout << "Do you want to enter starting positions?" << endl;
    cout << "Y: Yes, I define starting positions myself" << endl;
    cout << "N: No, let programm do it instead of me" << endl;
    string str;
    cin >> str;
    if (str == "N" || str == "n") {
        std::random_device rd;
        std::default_random_engine e1(rd());
        std::uniform_int_distribution<int> heightRand(0, height - 1);
        std::uniform_int_distribution<int> widthRand(0, width - 1);

        for (int i = 0; i < aliveCells; i++) {
            int h = heightRand(e1);
            int w = widthRand(e1);
            Cell *cur = field->getCell(h, w);
            assert (cur != NULL);
            if (cur->getState() == _alive) {
                i--;
                continue;
            }
            getMinMax(minH, maxH, minW, maxW, h, w);
            field->setCellState(cur, _alive);
        }
    }

    else if (str == "Y" || str == "y") {
        cout << "Enter coordinates - row and column at which intersection the cell in located" << endl;
        cout << "row: more than or equal to 0 and less than " << height << " (height)" << endl;
        cout << "column: more than or equal to 0 and less than " << width << " (width)" << endl;
        for (int i = 0; i < aliveCells; i++) {
            int h, w;
            cin >> h >> w;
            if (h < 0 || h >= height) {
                cout << "Invalid row number. Please enter correct coordinates" << endl;
                i--;
                continue;
            }
            if (w < 0 || w >= width) {
                cout << "Invalid column number. Please enter correct coordinates" << endl;
                i--;
                continue;
            }
            Cell *cur = field->getCell(h, w);
            assert (cur != NULL);
            if (cur->getState() == _alive) {
                cout << "The cell has already been mentioned. You may only enter coordinates of unmentioned cells" << endl;
                cout << "Enter " << aliveCells - i << " more unmentioned " << (aliveCells - i == 1 ? "cell" : "cells") << endl;
                i--;
                continue;
            }
            getMinMax(minH, maxH, minW, maxW, h, w);
            field->setCellState(cur, _alive);
        }
    }

    else {
        cout << "!!!Wrong option!!!" << endl;
        goto selection;
    }
    //field->Draw();
    expand(minH, maxH, minW, maxW, height, width, field);
    shrink(minH, maxH, minW, maxW, height, width, field);
    field->Check();
    //field->Draw();

    list< pair <Cell*, cellState> > changes;
    int noChange = 0, born = 0, died = 0;
    int startPop = field->getAliveCells();

    for (int k = 0; k < steps; ++k) {
        height = field->getHeight();
        width = field->getWidth();
        minH = height + 1;
        minW = width + 1;
        maxH = maxW = -2;
        int popBeforeStep = field->getAliveCells();
        changes.clear();

        Cell *_i = field->getHead();
        for (int i = 0; i < field->getHeight(); ++i, _i = _i->getDown()) {
            Cell *_j = _i;
            for (int j = 0; j < field->getWidth(); ++j, _j = _j->getRight()) {
                int adjecent = field->aliveAdjecents(_j);
                if (adjecent == 3) {
                    if (_j->getState() == _dead) {
                        born++;
                        changes.push_back( make_pair(_j, _alive) );
                    }
                    getMinMax(minH, maxH, minW, maxW, i, j);
                }
                else if (adjecent != 2) {
                    if (_j->getState() == _alive) {
                        died++;
                        changes.push_back( make_pair(_j, _dead) );
                    }
                }
                else if (_j->getState() == _alive) {
                    getMinMax(minH, maxH, minW, maxW, i, j);
                }
            }
        }

        expand(minH, maxH, minW, maxW, height, width, field);
        shrink(minH, maxH, minW, maxW, height, width, field);
        field->Check();

        for (auto p : changes) {
            field->setCellState(p.first, p.second);
        }

        if (popBeforeStep != field->getAliveCells()) {
            noChange = k + 1;
        }
        if ((k + 1) % 10 == 0) {
            cout << k + 1 << "th step" << endl;
            cout << "Amount of alive cells: " << field->getAliveCells() << endl;
            if (died == 0) {
                cout << "No dead, born: " << born << endl;
            }
            else {
                cout << "The average ratio born / dead: " << (double)born / died << endl;
            }
            cout << endl;
            born = died = 0;
        }
        //field->Draw();
    }
    cout << endl;
    cout << "The end of the simulation" << endl << endl;
    cout << "Population growth: " << field->getAliveCells() - startPop <<endl;
    cout << "Population was: " << startPop <<endl;
    cout << "Population became: " << field->getAliveCells() << endl << endl;
    if (noChange < steps) {
        cout << "After " << noChange << "-th step population size did not change" << endl << endl;
    }
    cout << "Size of the field at the end of the simulation:" << endl;
    cout << "Height: " << field->getHeight() << endl;
    cout << "Width: " << field->getWidth() << endl << endl;
    if (field->getWidth() == 0 || field->getHeight() == 0) {
        cout << "Field is empty" << endl;
    }
    else {
        cout << "Do you want to see the field?" << endl;
        cout << "Y: Yes, show me the field" << endl;
        cout << "N: No, I don't need it" << endl;
        cin >> str;
        if (str == "Y" || str == "y") {
            while (1) {
                field->Draw();
                cout << "Enter W to move up, S down, A left, D right and E to exit:" << endl;
                char choice;
                cin >> choice;
                switch (choice) {
                    case 'w':
                    case 'W':
                        field->moveUp();
                        break;
                    case 's':
                    case 'S':
                        field->moveDown();
                        break;
                    case 'a':
                    case 'A':
                        field->moveLeft();
                        break;
                    case 'd':
                    case 'D':
                        field->moveRight();
                        break;
                    case 'e':
                    case 'E':
                        return 0;
                    default:
                        cout << "Unknown command" << endl;
                }
            }
        }
    }
    return 0;
}

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