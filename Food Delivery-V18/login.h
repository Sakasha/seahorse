#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <openssl/sha.h>
#include "utility.h"

#ifndef LOGIN_H
#define LOGIN_H

/*
    This file handles all login related functions including login and registration

    Since it uses openssl for hashing the password, use the following command to compile

    "gcc -o login login.h -lssl -lcrypto"
*/

#define MAX_LINE_LENGTH 200
#define MAX_USERNAME_LENGTH 11                   // 10 digits + 1 for null terminator
#define MAX_PASSWORD_LENGTH 21                   // 20 chars + 1 for null terminator
#define HASH_LENGTH SHA256_DIGEST_LENGTH * 2 + 1 // Hex representation of hash

#define CREDENTIAL_FILENAME "./data/credentials.txt"

//---------------------------------------------------------------------------------//
// ------------------- Method Definitions---------- -------------------------------//
//---------------------------------------------------------------------------------//
int validateCredentials(const char *username, const char *password);
int isValidUsername(const char *username);
int isValidPassword(const char *password);
void hashPassword(const char *password, char *hashedPassword);
int registerUser(char *username);

//---------------------------------------------------------------------------------//
// ------------------- Method Implementation Starts -------------------------------//
//---------------------------------------------------------------------------------//

//
// Validates user credentials by comparing what is entered  with what is the file
//
int validateCredentials(const char *username, const char *password)
{
    // printf("### about to validate username: %s , password : %s\n", username, password);

    int retVal = 1;

    FILE *file = fopen(CREDENTIAL_FILENAME, "r");

    if (!file)
    {
        perror("Error opening credentials file");
        return retVal;
    }

    char line[MAX_LINE_LENGTH];
    char hashedPassword[HASH_LENGTH];

    hashPassword(password, hashedPassword);

    while (fgets(line, sizeof(line), file))
    {
        char fileUsername[MAX_USERNAME_LENGTH];
        char fileHashedPassword[HASH_LENGTH];

        sscanf(line, "%s %s", fileUsername, fileHashedPassword);

        if (strcmp(username, fileUsername) == 0 && strcmp(hashedPassword, fileHashedPassword) == 0)
        {
            retVal = 0;
            break;
        }
    }

    fclose(file);
    return retVal;
}

//
// Validate if the user name is valid.
// A valid user name should be equal to 10 digits
//
int isValidUsername(const char *username)
{

    // printf("#### username %s, strlen(username): %d \n", username, strlen(username));
    if (strlen(username) != 10)
    {
        return 1;
    }
    for (int i = 0; i < 10; i++)
    {
        if (!isdigit(username[i]))
        {
            return 1;
        }
    }
    return 0;
}

//
// Check if the password is valid
// A valid password should be greter than 3 and less than 20 chars
//
int isValidPassword(const char *password)
{
    if (strlen(password) > 3 && strlen(password) < 20)
    {
        return 0;
    }
    return 1;
}

//
// A method to check if the user name is already registered
//
int isUsernameRegistered(const char *username)
{
    FILE *file = fopen(CREDENTIAL_FILENAME, "r");
    if (!file)
    {
        perror("Error opening credentials file");
        return 0;
    }

    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), file))
    {
        char fileUsername[MAX_USERNAME_LENGTH];

        sscanf(line, "%s", fileUsername);

        if (strcmp(username, fileUsername) == 0)
        {
            fclose(file);
            return 1;
        }
    }

    fclose(file);
    return 0;
}

//
// A hashing method
//
void hashPassword(const char *password, char *hashedPassword)
{
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, password, strlen(password));
    SHA256_Final(hash, &sha256);

    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
    {
        sprintf(hashedPassword + (i * 2), "%02x", hash[i]);
    }
    hashedPassword[HASH_LENGTH - 1] = 0;
}

//
// Registers new users credentials in the file
//
int registerUser(char *username)
{

    printf("#### username %s \n", username);

    int choice = 0;

    char password[MAX_PASSWORD_LENGTH];

    if (isValidUsername(username) != 0)
    {
        printf("Invalid username. It must be exactly 10 digits.\n");
        return 10;
    }

    if (isUsernameRegistered(username))
    {
        printf("Username already registered. Please choose a different username.\n");
        return 11;
    }

    while (1)
    {

        printf("Enter password (up to 20 characters): ");
        scanf("%s", password);

        if (isValidPassword(password) != 0)
        {
            printRed("Invalid password. It must be at most 20 characters long.\n");
            continue;
        }
        else
        {
            break;
        }
    }

    char hashedPassword[HASH_LENGTH];
    hashPassword(password, hashedPassword);

    FILE *file = fopen(CREDENTIAL_FILENAME, "a+");
    if (!file)
    {
        perror("Error opening credentials file");
        choice = 12;
    }
    else
    {

        fprintf(file, "%s %s\n", username, hashedPassword);
        fclose(file);
    }

    printf("User registered successfully!\n");
    choice = 0;
    return choice;
}



#endif
