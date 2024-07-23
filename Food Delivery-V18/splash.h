#include <stdio.h>
#include <string.h>
#include <unistd.h> // for sleep function

#ifndef SPLASH_H
#define SPLASH_H

/*
    A method to display the splash screen

*/

#define CONSOLE_WIDTH 80  // Assuming the console width is 80
#define SPLASH_DELAY  1   // How many seconds should the splash screen displayed

//---------------------------------------------------------------------------------//
// ------------------- Method Definitions---------- -------------------------------//
//---------------------------------------------------------------------------------//
void printCentered(const char *text);  // Utility method to print in the middle ofthe screen
void printSplashScreen();

//---------------------------------------------------------------------------------//
// ------------------- Method Implementation Starts -------------------------------//
//---------------------------------------------------------------------------------//

void printCentered(const char *text)
{
    int len = strlen(text);
    int padding = (CONSOLE_WIDTH - len) / 2;
    for (int i = 0; i < padding; i++)
    {
        printf(" ");
    }
    printf("%s\n", text);
}

void  printSplashScreen()
{
    const char *welcome[] = {
        "   W   W   EEEEE   L       CCCC    OOO    M   M   EEEEE   ",
        "   W   W    E      L      C       O   O   MM MM   E       ",
        "     W     EEEE    L      C       O   O   M M M   EEE     ",
        "    W W    E       L      C       O   O   M   M   E       ",
        "   W   W   EEEEE   LLLLL   CCCC    OOO    M   M   EEEEE   "};

    const char *to[] = {
        "    TTTTT    OOO    ",
        "      T     O   O   ",
        "      T     O   O   ",
        "      T     O   O   ",
        "      T      OOO    "};

    const char *seahorse[] = {
        "   SSSS   EEEEE    AAA   H   H   OOO   RRRR    SSSS  EEEEE   ",
        "  S       E       A   A  H   H  O   O  R   R  S      E       ",
        "   SSS    EEEE    AAAAA  HHHHH  O   O  RRRR    SSS   EEE     ",
        "      S   E       A   A  H   H  O   O  R  R       S  E       ",
        "  SSSS    EEEEE   A   A  H   H   OOO   R   R  SSSS   EEEEE   "};


    printf("\n");
    int i, j;
    int lines = sizeof(welcome) / sizeof(welcome[0]);
    for (i = 0; i < lines; ++i)
    {
        printCentered(welcome[i]);
    }
    printf("\n");
    lines = sizeof(to) / sizeof(to[0]);
    for (i = 0; i < lines; ++i)
    {
        printCentered(to[i]);
    }
    printf("\n");
    lines = sizeof(seahorse) / sizeof(seahorse[0]);
    for (i = 0; i < lines; ++i)
    {
        printCentered(seahorse[i]);
    }

    printf("\n\n");
    printCentered("****   Your hyperlocal food delivery app   ****");
    printf("\n\n");

    // Delay for few seconds
    sleep(SPLASH_DELAY);


}

void printBye() {
    const char* bye[] = {
        " BBBBB  Y   Y  EEEEE ",
        " B    B  Y Y   E     ",
        " BBBBB    Y    EEE   ",
        " B    B   Y    E     ",
        " BBBBB    Y    EEEEE "
    };

    printf("\n");
    int lines = sizeof(bye) / sizeof(bye[0]);
    for (int i = 0; i < lines; ++i) {
        printCentered(bye[i]);
    }
    printf("\n");
}

#endif
