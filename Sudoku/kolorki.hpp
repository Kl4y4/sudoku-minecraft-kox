#include <cstdio>
#include <string>
#include <iostream>

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
