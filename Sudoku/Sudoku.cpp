#include <iostream>
#include "kolorki.hpp"
#include <random>

#define MAX_NUMBERS 10

using namespace std;

int board[9][9] { 0 };

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
    for (int i = 0; i < 9; i++, cout << "\n") {
        for (int j = 0; j < 9; j++) {
            if (j % 3 == 0 || j == 0) cout << "| ";
            cout << board[i][j] << " ";
        }
    }
}


bool is_valid(int x, int y) {
    int l = board[y][x];    
    //if(l == 0) // check
    
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (i == y && j != x) if (board[i][j] == l) return false;  // |
            if (j == x && i != y) if (board[i][j] == l) return false ; // -
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
    srand(time(0));
    draw_board();
    puts("\n--------------------------------------------");
    shuffle_numbers(10);
    draw_board();
}
