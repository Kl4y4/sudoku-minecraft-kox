#include <iostream>
#include "kolorki.hpp"
#include <random>

#define MAX_NUMBERS 10

using namespace std;

int board[9][9]{ 0 };

int wylosowane[MAX_NUMBERS][2]{ 0 };

const int chunk_centers[9][2] = {
    {0,0},
    {3,0},
    {6,0},

    {0,3},
    {3,3},
    {6,3},

    {0,6},
    {3,6},
    {6,6},
};

void draw_board() {
    cout << "   1 2 3   4 5 6   7 8 9\n";
    for (int i = 0; i < 9; i++, cout << "\n") {
        if (i % 3 == 0) writeInStyle(CYAN, "  ------------------------\n", 0);
        for (int j = 0; j < 9; j++) {
            if (j == 0) cout << i + 1;
            if (j % 3 == 0) writeInStyle(CYAN, "| ", 0);
            cout << board[i][j] << " ";
            if (j == 8) writeInStyle(CYAN, "| ", 0);
        }
        if (i == 8) writeInStyle(CYAN, "\n  ------------------------\n", 0);
    }
}


bool is_valid(int x, int y) {
    int l = board[y][x];
    //if(l == 0) // check

    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (i == y && j != x) if (board[i][j] == l) return false;  // |
            if (j == x && i != y) if (board[i][j] == l) return false; // -
        }
    }

    int chunk_x = chunk_centers[(x / 3) + ((y / 3) * 3)][0];
    int chunk_y = chunk_centers[(x / 3) + ((y / 3) * 3)][1];

    int o = 0;
    for (int i = chunk_y; i < chunk_y + 3; i++) {
        for (int j = chunk_x; j < chunk_x + 3; j++) {
            o += board[i][j] == l;
        }
    }
    return (o == 1);
}

bool check_for_round_end() {
    for (int i = 0; i < 9; i++)
        for (int j = 0; j < 9; j++)
            if (board[i][j] == 0) return false;
    return true;
}

void get_user_input(int& x, int& y, int& number) {

    puts("WYKONAJ RUCH\n");
    cin >> x >> y >> number;
    x--; y--;

    if (number < 1 || number > 9 || x < 0 || x > 8 || y < 0 || y > 8) {
        puts("Zle numerki wybrany6 kolego");
        get_user_input(x, y, number);
    }

    for (int i = 0; i < MAX_NUMBERS; i++) {
        if (y == wylosowane[i][0] && x == wylosowane[i][1] || !is_valid(x, y)) {
            puts("Wybrane pole jest ZLE!!!!1!!1!");
            get_user_input(x, y, number);
        }
    }

}

void shuffle_numbers(int count) {
    int idx = 0;
    while (count) {
        int x = rand() % 9, y = rand() % 9;
        int n = rand() % 9 + 1;
        if (board[y][x] != 0) continue;
        board[y][x] = n;
        if (is_valid(x, y)) {
            count--;
            wylosowane[idx][0] = x;
            wylosowane[idx++][1] = y;
        }
        else board[y][x] = 0;
    }
}

int main() {

    // input gracza gamera
    int x, y, number;

    char b = '0xE2';
    cout << b;


}