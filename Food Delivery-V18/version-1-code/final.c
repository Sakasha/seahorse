#include <stdio.h>//For scanf and printf
#include <stdlib.h>//For malloc and srand
#include <string.h>//For strcmp and other string functions including tokenisation
#include <time.h>//For time and srand
#include <termios.h>//For password not being displayed on the screen
#include <unistd.h>//For password
#include <ctype.h>//For isspace, olower and isdigit
#include <limits.h>//To define INT_MAX

//Structure for user details
typedef struct 
{
    char* username;
    char* mobile_number;
    char* password;
    char* user_location;
    char** previous_orders;
} User;

//memory allocation for user details
User* createInfo() 
{
    User* info = (User*)malloc(sizeof(User));
    info->username = (char*)malloc(50 * sizeof(char));
    info->mobile_number = (char*)malloc(20 * sizeof(char));
    info->password = (char*)malloc(50 * sizeof(char));
    info->user_location = (char*)malloc(50 * sizeof(char));
    info->previous_orders = (char**)malloc(100 * sizeof(char*));
    info->previous_orders[0] = NULL;
    return info;
}

//Function to generate captcha
void captcha() 
{
    srand(time(NULL));
    int num1, num2, result, userAnswer;
    char operator;

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
            case '/': result = num1 / num2; break;
        }

        printf("Your Answer: ");
        scanf("%d", &userAnswer);

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

//Function to input password
char* getPassword(char *password) 
{
    struct termios old, new;
    int i = 0;
    char ch;

    tcgetattr(STDIN_FILENO, &old);
    new = old;
    new.c_lflag &= ~(ECHO | ECHOE | ECHOK | ECHONL | ICANON);
    tcsetattr(STDIN_FILENO, TCSANOW, &new);

    while (1) 
    {
        ch = getchar();

        if (ch == '\n' || ch == '\r') 
        {
            password[i] = '\0';
            break;
        } 
        else if (ch == 127 && i > 0) 
        {
            i--;
            printf("\b \b");
        } 
        else if (i < 99) 
        {
            printf("*");
            password[i++] = ch;
        }
    }

    tcsetattr(STDIN_FILENO, TCSANOW, &old);

    return password;
}

//Function to remove leading and trailing white spaces
void trim(char str[]) 
{
    int start = 0, end = strlen(str) - 1;

    while (isspace(str[start])) 
    {
        start++;
    }

    while (end > start && isspace(str[end])) 
    {
        end--;
    }

    for (int i = 0; i <= end - start; i++) 
    {
        str[i] = str[start + i];
    }

    str[end - start + 1] = '\0';
}

//Function to check if mobile number has 10 digits
int isValidMobileNumber(const char* number) 
{
    int length = strlen(number);
    if (length != 10) 
    {
        return 0;
    }
    for (int i = 0; i < length; i++) 
    {
        if (!isdigit(number[i])) 
        {
            return 0;
        }
    }
    return 1;
}

//Function to check if inputted location is valid
int isValidLocation(const char* location) 
{
    const char* valid_locations[] = {"adyar", "sholinganallur", "anna nagar", "kalavakkam", "neelankarai"};
    for (int i = 0; i < 5; i++) 
    {
        if (strcmp(location, valid_locations[i]) == 0) 
        {
            return 1;
        }
    }
    return 0;
}

//Function to input the location
void getLocation(char* location) 
{
    const char* valid_locations[] = {"adyar", "sholinganallur", "anna nagar", "kalavakkam", "neelankarai"};
    int attempts = 0;
    while (attempts < 5) 
    {
        printf("\nEnter your location number:\n1. Adyar\n2. Sholinganallur\n3. Anna Nagar\n4. Kalavakkam\n5. Neelankarai\n");
        int choice;
        scanf("%d", &choice);
        getchar(); // consume the newline character

        if (choice >= 1 && choice <= 5) 
        {
            strcpy(location, valid_locations[choice - 1]);
            break;
        } 
        else 
        {
            attempts++;
            if (attempts < 5) 
            {
                printf("Invalid choice. It must be one of the listed options.\n");
                printf("Do you want to try again or exit? Enter 1 to try again, 0 to exit.\n");
                int retry_choice;
                scanf("%d", &retry_choice);
                getchar(); // consume the newline character
                if (retry_choice == 0) 
                {
                    printf("Exiting...\n");
                    exit(0);
                }
            } 
            else 
            {
                printf("Too many invalid attempts. Exiting...\n");
                exit(0);
            }
        }
    }
}

int main() 
{
    FILE* file = fopen("userdb.txt", "r+");
    User** info = (User**)malloc(100 * sizeof(User*));

    char* file_content = (char*)malloc(1000 * sizeof(char));
    int index = 0;

    while (fgets(file_content, 999, file)) //Read content from file
    {
        info[index] = createInfo();
        char previous_orders[10000];
        int no_content_read = sscanf(file_content, "%[^,],%[^,],%[^,],%[^,],%[^\n]\n", info[index]->username, info[index]->mobile_number, info[index]->password, info[index]->user_location, previous_orders);
        if (no_content_read == 5) 
        {
            char* order = strtok(previous_orders, ",");
            int order_index = 0;

            while (order != NULL) 
            {
                info[index]->previous_orders[order_index++] = strdup(order);
                order = strtok(NULL, ",");
            }
            info[index]->previous_orders[order_index] = NULL;
        }
        index++;
    }
    info[index] = NULL;

    char USERNAME[50];
    char temp[50];
    char PASSWORD[100];
    char temp1[50];

    printf("\nEnter your username\n");//Input user name
    fgets(USERNAME, 50, stdin);
    USERNAME[strcspn(USERNAME, "\n")] = '\0';

    int user_found = -1;

    for (int i = 0; i < index; i++)
    {
        strcpy(temp, info[i]->username);
        if (temp[strlen(temp) - 1] == ' ') 
        {
            temp[strlen(temp) - 1] = '\0';
        }
        if (strcmp(USERNAME, temp) == 0) 
        {
            user_found = i;
            break;
        }
    }

    if (user_found != -1) 
    {
        for (int j = 1; j <= 3; j++) 
        {
            printf("\nEnter password\n");
            getPassword(PASSWORD);
            PASSWORD[strcspn(PASSWORD, "\n")] = '\0';

            strcpy(temp1, info[user_found]->password);
            if (temp1[strlen(temp1) - 1] == ' ') 
            {
                temp1[strlen(temp1) - 1] = '\0';
            }
            if (strcmp(PASSWORD, temp1) == 0) 
            {
                printf("\n%s Login Successful\n", USERNAME);
                fclose(file);
                return 0;
            } 
            else 
            {
                if (j != 3) 
                {
                    printf("\nPassword Incorrect, %d attempt(s) left. Try Again\n", 3 - j);
                } 
                else 
                {
                    printf("\nPassword Incorrect, Login Failed\n");

                    printf("\nWould you like to change your password? Enter 1 to change, 0 to exit.\n");
                    int change_password;
                    scanf("%d", &change_password);
                    getchar();

                    if (change_password == 1) 
                    {
                        char new_password[100];
                        char retyped_password[100];
                        char retyped_password_new[100];

                        while (1) 
                        {
                            printf("\nEnter new password: \n");
                            getPassword(new_password);
                            new_password[strcspn(new_password, "\n")] = '\0';
                            trim(new_password);

                            printf("\nRe-enter your password\n");
                            getPassword(retyped_password);
                            retyped_password[strcspn(retyped_password, "\n")] = '\0';
                            trim(retyped_password);

                            if (strcmp(new_password, retyped_password) != 0) 
                            {
                                printf("\nThe re-entered password does not match with the previously entered password!\n");
                                printf("\nDo you want to see the password that you entered?\nEnter 1 to see\n");
                                int view_password;
                                scanf("%d", &view_password);
                                getchar();

                                if (view_password == 1) 
                                {
                                    printf("\nPassword entered: %s\n", new_password);
                                }

                                printf("\nRe-enter password\n");
                                getPassword(retyped_password_new);
                                retyped_password_new[strcspn(retyped_password_new, "\n")] = '\0';
                                trim(retyped_password_new);

                                if (strcmp(new_password, retyped_password_new) != 0) 
                                {
                                    printf("\nBoth the passwords don't match\nEnter 1 to sign up again\nEnter 0 to exit\n");
                                    int choice;
                                    scanf("%d", &choice);
                                    getchar();
                                    if (choice == 0) 
                                    {
                                        printf("Exiting...\n");
                                        fclose(file);
                                        return 0;
                                    }
                                } 
                                else 
                                {
                                    strcpy(info[user_found]->password, new_password);
                                    fseek(file, 0, SEEK_SET);
                                    for (int n = 0; n < index; n++) 
                                    {
                                        if (n > 0) 
                                        {
                                            fprintf(file, "\n");
                                        }
                                        fprintf(file, "%s,%s,%s,%s",
                                                info[n]->username,
                                                info[n]->mobile_number,
                                                info[n]->password,
                                                info[n]->user_location);
                                        if (info[n]->previous_orders[0] != NULL) 
                                        {
                                            fprintf(file, ",");
                                            for (int o = 0; info[n]->previous_orders[o] != NULL; o++) 
                                            {
                                                if (o > 0) 
                                                {
                                                    fprintf(file, ",");
                                                }
                                                fprintf(file, "%s", info[n]->previous_orders[o]);
                                            }
                                        }
                                    }
                                    printf("\nPassword changed successfully. Please login again.\n");
                                    fclose(file);
                                    return 0;
                                }
                            } 
                            else 
                            {
                                strcpy(info[user_found]->password, new_password);
                                fseek(file, 0, SEEK_SET);
                                for (int n = 0; n < index; n++) 
                                {
                                    if (n > 0) 
                                    {
                                        fprintf(file, "\n");
                                    }
                                    fprintf(file, "%s,%s,%s,%s",
                                            info[n]->username,
                                            info[n]->mobile_number,
                                            info[n]->password,
                                            info[n]->user_location);
                                    if (info[n]->previous_orders[0] != NULL) 
                                    {
                                        fprintf(file, ",");
                                        for (int o = 0; info[n]->previous_orders[o] != NULL; o++) 
                                        {
                                            if (o > 0) 
                                            {
                                                fprintf(file, ",");
                                            }
                                            fprintf(file, "%s", info[n]->previous_orders[o]);
                                        }
                                    }
                                }
                                printf("Password changed successfully. Please login again.\n");
                                fclose(file);
                                return 0;
                            }
                        }
                    } 
                    else 
                    {
                        printf("Login Failed\n");
                        fclose(file);
                        return 0;
                    }
                }
            }
        }
    } 
    else 
    {
        printf("\nYou don't have an account, you have to sign up\n");
        file = freopen("userdb.txt", "a", file);

        int signup_again = 1;
        int choice;
        char* password = (char*)malloc(100 * sizeof(char));
        char* retyped_password = (char*)malloc(100 * sizeof(char));
        char* retyped_password_new = (char*)malloc(100 * sizeof(char));
        User* new_user = createInfo();

        while (signup_again == 1) 
        {
            printf("\nEnter your username\n");
            fgets(new_user->username, 50, stdin);
            new_user->username[strcspn(new_user->username, "\n")] = '\0';

            int username_exists = 0;
            for (int i = 0; i < index; i++) 
            {
                if (strcmp(new_user->username, info[i]->username) == 0) 
                {
                    username_exists = 1;
                    break;
                }
            }

            if (username_exists) 
            {
                printf("Username already exists. Please enter your password.\n");

                for (int j = 1; j <= 3; j++) 
                {
                    printf("\nEnter password\n");
                    getPassword(password);
                    password[strcspn(password, "\n")] = '\0';

                    strcpy(temp1, info[user_found]->password);
                    if (temp1[strlen(temp1) - 1] == ' ') 
                    {
                        temp1[strlen(temp1) - 1] = '\0';
                    }
                    if (strcmp(password, temp1) == 0) 
                    {
                        printf("\n%s Login Successful\n", new_user->username);
                        fclose(file);
                        return 0;
                    } 
                    else 
                    {
                        if (j != 3) 
                        {
                            printf("Password Incorrect, %d attempt(s) left. Try Again\n", 3 - j);
                        } 
                        else 
                        {
                            printf("Password Incorrect, Login Failed\n");

                            printf("Would you like to change your password? Enter 1 to change, 0 to exit.\n");
                            int change_password;
                            scanf("%d", &change_password);
                            getchar();

                            if (change_password == 1) 
                            {
                                char new_password[100];
                                char retyped_password[100];
                                char retyped_password_new[100];

                                while (1) 
                                {
                                    printf("\nEnter new password: ");
                                    getPassword(new_password);
                                    new_password[strcspn(new_password, "\n")] = '\0';
                                    trim(new_password);

                                    printf("\nRe-enter your password\n");
                                    getPassword(retyped_password);
                                    retyped_password[strcspn(retyped_password, "\n")] = '\0';
                                    trim(retyped_password);

                                    if (strcmp(new_password, retyped_password) != 0) 
                                    {
                                        printf("\nThe re-entered password does not match with the previously entered password!\n");
                                        printf("\nDo you want to see the password that you entered?\nEnter 1 to see\n");
                                        int view_password;
                                        scanf("%d", &view_password);
                                        getchar();

                                        if (view_password == 1) 
                                        {
                                            printf("\nPassword entered: %s\n", new_password);
                                        }

                                        printf("\nRe-enter password\n");
                                        getPassword(retyped_password_new);
                                        retyped_password_new[strcspn(retyped_password_new, "\n")] = '\0';
                                        trim(retyped_password_new);

                                        if (strcmp(new_password, retyped_password_new) != 0) 
                                        {
                                            printf("\nBoth the passwords don't match\nEnter 1 to sign up again\nEnter 0 to exit\n");
                                            int choice;
                                            scanf("%d", &choice);
                                            getchar();
                                            if (choice == 0) 
                                            {
                                                printf("Exiting...\n");
                                                fclose(file);
                                                return 0;
                                            }
                                        } 
                                        else 
                                        {
                                            strcpy(info[user_found]->password, new_password);
                                            fseek(file, 0, SEEK_SET);
                                            for (int n = 0; n < index; n++) 
                                            {
                                                if (n > 0) 
                                                {
                                                    fprintf(file, "\n");
                                                }
                                                fprintf(file, "%s,%s,%s,%s",
                                                        info[n]->username,
                                                        info[n]->mobile_number,
                                                        info[n]->password,
                                                        info[n]->user_location);
                                                if (info[n]->previous_orders[0] != NULL) 
                                                {
                                                    fprintf(file, ",");
                                                    for (int o = 0; info[n]->previous_orders[o] != NULL; o++) 
                                                    {
                                                        if (o > 0) 
                                                        {
                                                            fprintf(file, ",");
                                                        }
                                                        fprintf(file, "%s", info[n]->previous_orders[o]);
                                                    }
                                                }
                                            }
                                            printf("Password changed successfully. Please login again.\n");
                                            fclose(file);
                                            return 0;
                                        }
                                    } 
                                    else 
                                    {
                                        strcpy(info[user_found]->password, new_password);
                                        fseek(file, 0, SEEK_SET);
                                        for (int n = 0; n < index; n++) 
                                        {
                                            if (n > 0) 
                                            {
                                                fprintf(file, "\n");
                                            }
                                            fprintf(file, "%s,%s,%s,%s",
                                                    info[n]->username,
                                                    info[n]->mobile_number,
                                                    info[n]->password,
                                                    info[n]->user_location);
                                            if (info[n]->previous_orders[0] != NULL) 
                                            {
                                                fprintf(file, ",");
                                                for (int o = 0; info[n]->previous_orders[o] != NULL; o++)
                                                {
                                                    if (o > 0) 
                                                    {
                                                        fprintf(file, ",");
                                                    }
                                                    fprintf(file, "%s", info[n]->previous_orders[o]);
                                                }
                                            }
                                        }
                                        printf("Password changed successfully. Please login again.\n");
                                        fclose(file);
                                        return 0;
                                    }
                                }
                            } 
                            else 
                            {
                                printf("Login Failed\n");
                                fclose(file);
                                return 0;
                            }
                        }
                    }
                }
            } 
            else 
            {
                printf("\nEnter your mobile number\n");
                fgets(new_user->mobile_number, 20, stdin);
                new_user->mobile_number[strcspn(new_user->mobile_number, "\n")] = '\0';

                if (!isValidMobileNumber(new_user->mobile_number)) 
                {
                    printf("Invalid mobile number, it must be exactly 10 digits.\n");
                    continue;
                }

                printf("\nEnter your password\n");
                getPassword(password);
                password[strcspn(password, "\n")] = '\0';
                trim(password);

                printf("\nRe-enter your password\n");
                getPassword(retyped_password);
                retyped_password[strcspn(retyped_password, "\n")] = '\0';
                trim(retyped_password);

                if (strcmp(password, retyped_password) != 0) 
                {
                    printf("\nThe re-entered password does not match with the previously entered password!\n");
                    printf("\nDo you want to see the password that you entered?\nEnter 1 to see\n");
                    int view_password;
                    scanf("%d", &view_password);
                    getchar();

                    if (view_password == 1) 
                    {
                        printf("\nPassword entered: %s\n", password);
                    }

                    printf("\nRe-enter password\n");
                    getPassword(retyped_password_new);
                    retyped_password_new[strcspn(retyped_password_new, "\n")] = '\0';
                    trim(retyped_password_new);

                    if (strcmp(password, retyped_password_new) != 0) 
                    {
                        printf("\nBoth the passwords don't match\nEnter 1 to sign up again\nEnter 0 to exit\n");
                        int choice;
                        scanf("%d", &choice);
                        getchar();
                        if (choice == 0) 
                        {
                            printf("Exiting...\n");
                            fclose(file);
                            return 0;
                        }
                    } 
                    else 
                    {
                        new_user->password = password;
                        getLocation(new_user->user_location);
                        fprintf(file, "\n");
                        fprintf(file, "%s,%s,%s,%s", new_user->username, new_user->mobile_number, new_user->password, new_user->user_location);
                        signup_again = 0;
                    }
                } 
                else 
                {
                    new_user->password = password;
                    getLocation(new_user->user_location);
                    fprintf(file, "\n");
                    fprintf(file, "%s,%s,%s,%s", new_user->username, new_user->mobile_number, new_user->password, new_user->user_location);
                    signup_again = 0;
                }
            }
        }
        fclose(file);
    }

    return 0;
}
