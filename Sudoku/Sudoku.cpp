#include <iostream>

using namespace std;

int board[9][9] { 0 };

int chunk_centers[9][2] = {
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


bool is_valid(int x, int y) {
    int l = board[y][x];
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
    if (o >= 2) return false;
    else return true;
}

int main() {
    draw_board();
}


