#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>

#ifndef _UTILZ_
#define _UTILZ_

#define MAX_ID_SIZE 8



//---------------------------------------------------------------------------------//
// ------------------- Method Definitions---------- -------------------------------//
//---------------------------------------------------------------------------------//
void generateID(char *ID, int len, char startChar);
char* generateRandomID(int size, char startLetter);
char* convertToLowercase(const char *str);
void printRed(const char *str);
void printBlue(const char *str) ;
char *trim(char *string);
void captcha();



//---------------------------------------------------------------------------------//
// ------------------- Method Implementation Starts -------------------------------//
//---------------------------------------------------------------------------------//



//
// Function to generate random ID
//
void generateID(char *ID, int len, char startChar)
{
    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    // Prefix first character with what ever character passed
    ID[0] = startChar;

    for (int i = 1; i < len - 1; i++)
    {
        ID[i] = charset[rand() % (sizeof(charset) - 1)];
    }
    ID[len - 1] = '\0';
}

//
// Function to generate a random ID with a specified size and starting letter
//
char* generateRandomID(int size, char startLetter) {
    static char id[MAX_ID_SIZE]; // Static array to store ID
    id[0] = startLetter; // Set the first character as the starting letter

    // Generate random numbers for the remaining characters
    for (int i = 1; i < size; i++) {
        id[i] = '0' + rand() % 10; // Generate random digit
    }
    id[size] = '\0'; // Null-terminate the string
    return id;
}


//
//Converting a string into lowercase
//
char* convertToLowercase(const char *str) {
    // Calculate the length of the input string
    size_t len = strlen(str);

    // Allocate memory for the lowercase string
    char *lowercase_str = (char*)malloc(len + 1); // +1 for the null terminator

    // Check if memory allocation was successful
    if (lowercase_str == NULL) {
        printf("Memory allocation failed.\n");
        exit(1); // Exit with failure code
    }

    // Iterate through each character of the input string
    for (size_t i = 0; i < len; i++) {
        // Convert character to lowercase and store in lowercase_str
        lowercase_str[i] = tolower(str[i]);
    }

    // Add null terminator at the end of the lowercase string
    lowercase_str[len] = '\0';

    return lowercase_str;
}

//
// Prints a string in red color
//
void printRed(const char *str) {
    printf("\033[31m%s\033[0m", str);
}

//
// Prints a string in blue color
//
void printBlue(const char *str) {
    printf("\033[40m%s\033[0m", str);
}

//
// Utility method to trim string
//
char *trim(char *string)
{
    static char buff[100];

    // trim prefix
    while ((*string) == ' ' ) {
         string++;
    }

    // find end of original string

    int i = 0;
    while (*string) {
         buff[i++] = *string;
         string++;
    }

    // trim suffix
    while ((buff[i]) == ' ' ) {
        buff[i] = '\0';
        i--;
    }
    return buff;
}

//
// Function to generate captcha
//
void captcha() 
{   
    getchar();
    srand(time(NULL));
    int num1, num2, result, userAnswer;
    char operator;
    char input[10]; // Buffer to read user input

    for (int attempt = 1; attempt <= 5; attempt++) 
    {
        num1 = rand() % 20 + 1;
        num2 = rand() % 20 + 1;
        operator = "+-*/"[rand() % 4];

        printf("\nAttempt %d: What is %d %c %d?\n", attempt, num1, operator, num2);

        switch (operator) 
        {
            case '+': result = num1 + num2; break;
            case '-': result = num1 - num2; break;
            case '*': result = num1 * num2; break;
            case '/': 
                // Ensure no division by zero
                while (num2 == 0) {
                    num2 = rand() % 20 + 1;
                }
                result = num1 / num2; 
                break;
        }

        int validInput = 0;
        while (!validInput) 
        {
            printf("Your Answer: ");
            if (fgets(input, sizeof(input), stdin) != NULL) 
            {
                // Check if input is a valid integer
                if (sscanf(input, "%d", &userAnswer) == 1) 
                {
                    validInput = 1;
                } 
                else 
                {
                    printf("Invalid input! Please enter an integer.\n");
                }
            }
        }

        if (userAnswer == result) 
        {
            printf("\nLogin Successful\n");
            return;
        } 
        else 
        {
            printf("\nIncorrect captcha, Try again\n");
        }
    }

    printf("Login Failed\n");
}

#endif

