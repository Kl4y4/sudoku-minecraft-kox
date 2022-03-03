#include <iostream>
#include "kolorki.hpp"
#include <random>

using namespace std;
int board[6][7]{ 0 };

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

void get_user_input(int& x, int& y) {

    puts("WYKONAJ RUCH\n");
    cin >> x >> y;

    for (int i = 0; i < 10; i++) {
        if (y == wylosowane[i][0] && x == wylosowane[i][1]) {
            puts("Wybrane pole jest już zajęte!!!!1!!1!");
            get_user_input(x, y);
        }
    }

}

int main() {
    
}