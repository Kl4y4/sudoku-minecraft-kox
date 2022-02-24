#include <iostream>

using namespace std;

int board[9][9] { 0 };

void draw_board() {
    for (int i = 0; i < 9; i++, cout << "\n") {
        for (int j = 0; j < 9; j++) {
            if (j % 3 == 0 || j == 0) cout << "| ";
            cout << board[i][j] << " ";
        }
    }

}

int main() {
    draw_board(board);
}




