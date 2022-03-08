#include <iostream>
#include "kolorki.hpp"
#include <random>
#include <math.h>
#include <ctype.h>
#include <string>
#include <windows.h>

using namespace std;

// =^w^=
int board[6][7]{ 0 };

void clear() {
    COORD topLeft = { 0, 0 };
    HANDLE console = GetStdHandle((DWORD)- 11);
    CONSOLE_SCREEN_BUFFER_INFO screen;
    DWORD written;

    GetConsoleScreenBufferInfo(console, &screen);
    FillConsoleOutputCharacterA(
        console, ' ', screen.dwSize.X * screen.dwSize.Y, topLeft, &written
    );
    FillConsoleOutputAttribute(
        console, 0x0002 | 0x0004 | 0x0001,
        screen.dwSize.X * screen.dwSize.Y, topLeft, &written
    );
    SetConsoleCursorPosition(console, topLeft);
}

void draw_board() {
    clear();
    for (int i = 0; i < 6; i++, cout << " |\n") {
        for (int j = 0; j < 7; j++) {
            cout << " | ";
            if (board[i][j] == 1) writeInStyle(BLUE, to_string(board[i][j]), 0);
            else if (board[i][j] == 2) writeInStyle(RED, to_string(board[i][j]), 0);
            else cout << " ";
        }
    }
    cout << " -----------------------------\n";
}

bool win_move(int x, int y) {
    int n = 0, k = board[y][x];
    // pionowo
    for (int i = y - 3; i <= y + 3; i++) {
        if (i >= 6 || i < 0) continue;
        if (board[i][x] == k) n++;   // jeżeli się powtarza to 1
        else                 n = 0; // jeżeli jakaś inna to wyzerować
        if (n == 4) return true;
    }

    // poziomo
    n = 0;
    for (int i = x - 3; i <= x + 3; i++) {
        if (i >= 7 || i < 0) continue;
        if (board[y][i] == k) n++;   // jeżeli się powtarza to 1
        else                 n = 0; // jeżeli jakaś inna to wyzerować
        if (n == 4) return true;
    }

    n = 0;
    for (int i = -3; i <= 3; i++) {
        if ((x + i >= 7 || x + i < 0) || (y + i >= 6 || y + i < 0)) continue;
        if (board[y + i][x + i] == k) n++;
        else                     n = 0;
        if (n == 4) return true;
    }

    n = 0;
    for (int i = 3; i >= -3; i--) {
        if ((x + i >= 7 || x + i < 0) || (y - i >= 6 || y - i < 0)) continue;
        if (board[y - i][x + i] == k) n++;
        else                     n = 0;
        if (n == 4) return true;
    }

    return false;
}

int get_user_input(int& xi, int player) {

    puts("WYKONAJ RUCH\n");
    string x = "";
    cin >> x;
    cout << x << "\n";
    
    try {
        xi = stoi(x, nullptr);
    }
    catch (exception e) {
        cout << "Not a number brrrr";
        get_user_input(xi, player);
    }

    if (xi < 1 || xi > 7) {
        cout << "POZA POLEM<!!!!";
        get_user_input(xi, player);
    }

    xi--;

    int ind = 9;
    for (int i = 5; i >= 0; i--) {
        if (board[i][xi] == 0) {
            ind = i;
            break;
        }
    }

    if (ind == 9) {
        cout << "WYBRANANA KOLUMNA PEŁŁNY";
        get_user_input(xi, player);
    }

    board[ind][xi] = player;

    return ind;

}

bool check_for_draw() {
    for (int i = 0; i < 7; i++) if (board[0][i] == 0) return false;
    return true;
}

void round() {

    int player = 1;
    int p_x = 0, p_y = 0;

    printf("Gracz %i: ", player);
    p_y = get_user_input(p_x, player);

    while (!win_move(p_x, p_y)) {

        if (check_for_draw()) {
            cout << "REMISIS!!!!";
            return;
        }

        draw_board();

        // zmienic na kolejnego gracza
        if (player == 1) player = 2;
        else            player = 1;

        printf("Gracz %i: ", player);
        p_y = get_user_input(p_x, player);
    }

    draw_board();
    printf("Gracz %i wygrał!!!!!!!!", player);

}

int welcome_screen() {
    
    changeStyle(RED);

    cout << "                                              (                    \n";
    cout << "     (                                    )   )\\ )                 \n";
    cout << "     )\\                        (       ( /(  (()/(        (   (    \n";
    cout << "   (((_)   (    (      (      ))\\  (   )\\())  /(_)) (    ))\\  )(   \n";
    cout << "   )\\___   )\\   )\\ )   )\\ )  /((_) )\\ (_))/  (_))_| )\\  /((_)(()\\  \n";
    cout << "  ((/ __| ((_) _(_/(  _(_/( (_))  ((_)| |_   | |_  ((_)(_))(  ((_) \n";
    cout << "   | (__ / _ \\| ' \\))| ' \\))/ -_)/ _| |  _|  | __|/ _ \\| || || '_| \n";
    cout << "    \\___|\\___/|_||_| |_||_| \\___|\\__|  \\__|  |_|  \\___/ \\_,_||_|   \n";

    changeStyle(RESET);

    return 0;
}

void clear_board() {

    for (int i = 0; i < 6; i++)
        for (int j = 0; j < 7; j++)
            board[i][j] = 0;

}

int main() {


    string reset = "";
    bool ifContinue = 1;

    welcome_screen();

    int enemy;
    cout << "1 - Play withh othe rplatya\n2 - Plray wtih AII";
    cin >> enemy;

    //if 1 gamer if 2 konkluter :))

    draw_board();

    while (ifContinue) {

        round();

        cout << "Czy chcesz zagrać jeszcze raz???? [y/n]";
        cin >> reset;

        if (reset == "y" || reset == "Y") {
            ifContinue = 1;
            clear_board();
        }
        else {
            cout << "Dziękujemy za grem :))";
            ifContinue = 0;
        }

    }

}
