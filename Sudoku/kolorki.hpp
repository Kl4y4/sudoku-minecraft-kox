#include <cstdio>
#include <string>
#include <iostream>

/*
#define BLACK_PAWN "♙"
#define BLACK_KNIGHT "♘"
#define BLACK_BISHOP "♗"
#define BLACK_ROOK "♖"
#define BLACK_QUEEN "♕"
#define BLACK_KING "♔"

#define WHITE_PAWN "♟︎"
#define WHITE_KNIGHT "♞"
#define WHITE_BISHOP "♝"
#define WHITE_ROOK "♜"
#define WHITE_QUEEN "♛"
#define WHITE_KING "♚"
*/

// https://wikiless.alefvanoon.xyz/wiki/ANSI_escape_code?lang=en#SGR_(Select_Graphic_Rendition)_parameters
// Style do wybrania
enum STYLES {
    RESET = 0,
    INVERSE = 7,   // Zamienia miejscami kolor tła i tekstu
    BLINK = 5,     // Mryga lol
    UNDERLINE = 4, // Podkreśla
    BLACK = 30, // kolor tekstu
    RED,
    GREEN,
    BLUE = 34,
    VIOLET,
    CYAN,
    B_BLACK = 40, // kolor tła
    B_RED,
    B_GREEN,
    B_BLUE = 44,
    B_VIOLET,
    B_CYAN,
    B_WHITE
};

//std::wstring black_pieces[] = { L"♘",L"♗",L"♖",L"♕",L"♔" };
//std::wstring white_pieces[] = { L"♟︎",L"♞",L"♝",L"♜",L"♛",L"♚" };

// Mozna uzyć zeby zmienic styl dla wszystkiego co bedzie napisane nastepne
void changeStyle(const int style) {
    printf("\033[%dm", style);
}

// Napisze podany tekst w podanym stylu
void writeInStyle(const int style, const std::string text, const int padding = 0) {
    changeStyle(RESET);
    changeStyle(style);
    std::cout << text;
    changeStyle(RESET);
}
