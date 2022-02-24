﻿#include <iostream>
#include <random>
#include <string>
#include <stdio.h>

using namespace std;

int board[9][9] { 0 };

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
    // dodałem se indexy żeby łatwiej się sprawdzało, można dodać potem czy coś
    // printf("    ");
    // for (int i = 0; i < 9; i++) cout << (i % 3 == 0 && i != 0 ? "   " : " ") << i;
    // putc('\n',stdout); 
    //printf("    ");
    //cout << string(9 * 3 - 4,'-') << "\n";
    for (int i = 0; i < 9; i++, cout << "\n") {
        // cout << i << "  ";
        for (int j = 0; j < 9; j++) {
            if (j % 3 == 0 || j == 0) cout << "| ";
            cout << board[i][j] << " ";
        }
    }
}

bool is_valid(int x, int y) {
    int l = board[y][x];    
    if(l == 0) // check
        cout << "yo ziom pomyłka, sprawdzasz puste pole, wstaw tam coś najpierw\n"; return false;
    
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

int main() {
    draw_board();
}

