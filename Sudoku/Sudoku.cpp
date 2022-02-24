#include <iostream>

using namespace std;

void draw_board(int _board[9][9]) {

    for (int i = 0; i < 9; i++, cout << "\n") {
        for (int j = 0; j < 9; j++) {
            if (j % 3 == 0 || j == 0) cout << "| ";
            cout << _board[i][j] << " ";
        }
    }

}

int main() {

    int board[9][9] { 0 };

    draw_board(board);

}




