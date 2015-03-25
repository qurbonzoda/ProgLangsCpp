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
*   Field size and the current position at the end of the simulation (if user wants to)
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
    cout << "Do you want to see the field?" << endl;
    cout << "Y: Yes, show me the field" << endl;
    cout << "N: No, I don't need it" << endl;
    cin >> str;
    if (str == "Y" || str == "y") {
        cout << "Here is the field at the end of the simulation:" << endl;
        field->Draw();
    }
    return 0;
}