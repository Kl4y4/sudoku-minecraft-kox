#include <iostream>
#include "kolorki.hpp"
#include <random>

using namespace std;


// =^w^=
int board[6][7] { 0 };

void draw_board() {
    for (int i = 0; i < 6; i++, cout << " |\n") {
        for (int j = 0; j < 7; j++) {
            cout << " | ";
            if (board[i][j] == 1)      writeInStyle(BLUE, "@", 0);
            else if (board[i][j] == 2) writeInStyle(RED, "@", 0);
            else cout << board[i][j];            
        }
    }
    cout << " -----------------------------\n";
}

bool win_move(int x,int y) {
    int n = 0, k = board[y][x];
    // pionowo
    for(int i = y-3; i <= y+3; i++){
        if(i >= 6 || i < 0) continue;
        if(board[i][x] == k) n++;   // jeżeli się powtarza to 1
        else                 n = 0; // jeżeli jakaś inna to wyzerować
        if(n == 4) return true;
    }
    
    // poziomo
    n = 0;
    for(int i = x-3; i <= x+3; i++){
        if(i >= 7 || i < 0) continue;
        if(board[y][i] == k) n++;   // jeżeli się powtarza to 1
        else                 n = 0; // jeżeli jakaś inna to wyzerować
        if(n == 4) return true;
    }
    
    n = 0;
    for(int i = -3; i <= 3; i++){
        if((x+i >= 7 || x+i < 0) || (y+i >= 6 || y+i < 0)) continue;
        if(board[y+i][x+i] == k) n++;
        else                     n = 0;
        if(n == 4) return true;
    }

    n = 0;
    for(int i = 3; i >= -3; i--){
        if((x+i >= 7 || x+i < 0) || (y-i >= 6 || y-i < 0)) continue;
        if(board[y-i][x+i] == k) n++;
        else                     n = 0;
        if(n == 4) return true;
    }
    
    return false;
}

int get_user_input(int& x, int player) {

    puts("WYKONAJ RUCH\n");
    cin >> x;

    if (x < 1 || x > 7) {
        cout << "POZA POLEM<!!!!";
        get_user_input(x, player);
    }

    x--;

    int ind = 9;
    for (int i = 5; i >= 0; i--) {
        if (board[i][x] == 0) {
            ind = i;
            break;
        }
    }

    if (ind == 9) {
        cout << "WYBRANANA KOLUMNA PEŁŁNY";
        get_user_input(x, player);
    }

    board[ind][x] = player;
    
    return ind;
    
}

int main() {
    draw_board();
    int player = 1;
    int p_x,p_y;

    printf("Gracz %i: ",player);
    p_y = get_user_input(p_x,player);
    while(!win_move(p_x,p_y)){
        draw_board();
        // zmienic na kolejnego gracza
        if(player == 1) player = 2;
        else            player = 1;

        printf("Gracz %i: ",player);
        p_y = get_user_input(p_x,player);
    }
    printf("WYGRAŁ GRACZ %i\n",player);
    draw_board();
}
