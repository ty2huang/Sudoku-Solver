#include <iostream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;

struct Grid {
    vector<vector<int>> grid;
    bool error;
    int nextAvailCellRow;
    int nextAvailCellCol;

    void initialize();
    void updateNextAvailCell();
    void findNextRowCol(int &row, int &col);
    bool isValueInBox(int val, int row, int col);
    bool isValidToInsert(int val, int row, int col);
    Grid insertValue(int val, int row, int col);
    void printGrid();
};

void Grid::initialize() {
    error = false;
    char input;
    for (int i = 0; i < 9; i++) {
        vector<int> row;
        for (int j = 0; j < 9; j++) {
            cin >> input;
            int val = input - '0';
            row.push_back(val);
        }
        grid.push_back(row);
    }
    nextAvailCellRow = 0; nextAvailCellCol = 0;
    updateNextAvailCell();
}

void Grid::updateNextAvailCell() {
    while (nextAvailCellRow < 9) {
        while (nextAvailCellCol < 9) {
            if (grid[nextAvailCellRow][nextAvailCellCol] == 0) return;
            nextAvailCellCol++;
        }
        nextAvailCellRow++;
        nextAvailCellCol = 0;
    }
}

void Grid::printGrid() {
    for (int i = 0; i < 9; i++) {
        if (i == 3 || i == 6) {
            cout << "===========" << endl;
        }
        for (int j = 0; j < 9; j++) {
            int val = grid[i][j];
            if (j == 3 || j == 6) {
                cout << "|";
            }
            if (val == 0) {
                cout << '-';
            }
            else {
                cout << val;
            }
        }
        cout << endl;
    }
    cout << endl;
}

void Grid::findNextRowCol(int &row, int &col) {
    for (int i = row; i < 9; i++) {
        for (int j = col; j < 9; j++) {
            if (grid[i][j] == 0) {
                row = i; col = j;
                return;
            }
        }
    }
    row = -1; col = -1;
}

bool Grid::isValueInBox(int val, int row, int col) {
    row = (row / 3) * 3;
    col = (col / 3) * 3;
    for (int i = row; i < row+3; i++) {
        for (int j = col; j < col+3; j++) {
            if (val == grid[i][j]) return true;
        }
    }
    return false;
}

bool Grid::isValidToInsert(int val, int row, int col) {
    bool isValueInRow = false;
    for (int j = 0; j < 9; j++) {
        if (val == grid[row][j]) {
            isValueInRow = true;
            break;
        }
    }
    bool isValueInCol = false;
    for (int i = 0; i < 9; i++) {
        if (val == grid[i][col]) {
            isValueInCol = true;
            break;
        }
    }
    return !isValueInRow && !isValueInCol && !isValueInBox(val, row, col);
}

Grid Grid::insertValue(int val, int row, int col) {
    Grid newGrid = *this;
    newGrid.grid[row][col] = val;
    newGrid.updateNextAvailCell();
    return newGrid;
}

Grid solveSudoku(Grid start) {
    if (start.nextAvailCellRow == 9) {
        return start;
    }
    int row = start.nextAvailCellRow;
    int col = start.nextAvailCellCol;
    Grid solution;
    for (int val = 1; val <= 9; val++) {
        if (start.isValidToInsert(val, row, col)) {
            Grid temp = start.insertValue(val, row, col);
            solution = solveSudoku(temp);
            if (!solution.error) return solution;
        }
    }
    start.error = true;
    return start;
}

int main() {
    string prompt;
    while (1) {
        Grid start;
        start.initialize();
        getline(cin, prompt); //take out empty line
        cout << endl << "The initial sudoku puzzle looks like this:" << endl;
        start.printGrid();

        Grid solution = solveSudoku(start);
        if (solution.error) {
            cout << "This sudoku puzzle has no solution." << endl;
        }
        else {
            cout << "The solution looks like this:" << endl;
            solution.printGrid();
        }
        
        cout << "Would you like to try again? (y/n)" << endl;
        while (1) {
            getline(cin, prompt);
            if (prompt == "y" || prompt == "n") break;
            cout << "Invalid entry, please try again (y/n):" << endl;
        }
        if (prompt == "n") break;
    }
    return 0;
}