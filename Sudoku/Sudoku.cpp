#include <iostream>

using namespace std;
int board[9][9] { 0 };



void draw_board() {

    for (int i = 0; i < 9; i++, cout << "\n") {
        if (i % 3 == 0) cout << "-------------------------\n";
        for (int j = 0; j < 9; j++) {
            if (j % 3 == 0) cout << "| ";
            cout << board[i][j] << " ";
            if (j == 8) cout << "| ";
        }
        if (i == 8) cout << "\n-------------------------\n";
    }

}

bool check_for_round_end() {
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
            if (board[i][j] == 0) return false;
    return true;
}

int main() {

    /*for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
            board[i][j] = 7;*/
    //draw_board();

    cout << check_for_round_end();



}




