#include <stdio.h>   // For scanf and printf
#include <stdlib.h>  // For malloc and srand
#include <string.h>  // For strcmp and other string functions including tokenisation
#include <time.h>    // For time and srand
//#include <termios.h> // For password not being displayed on the screen
#include <unistd.h>  // For password
#include <ctype.h>   // For isspace, tolower and isdigit
#include <limits.h>  // To define INT_MAX

typedef struct 
{
    char* restaurant;
    char* location;
    char* rating;
    char* best_seller;
    char** items;
} Restaurant;

Restaurant* createInfo() 
{
    Restaurant* info = (Restaurant*)malloc(sizeof(Restaurant));
    info->restaurant = (char*)malloc(50 * sizeof(char));
    info->location = (char*)malloc(20 * sizeof(char));
    info->rating = (char*)malloc(50 * sizeof(char));
    info->best_seller = (char*)malloc(50 * sizeof(char));
    info->items = (char**)malloc(100 * sizeof(char*));
    info->items[0] = NULL;
    return info;
}

void trim(char str[]) 
{
    int start = 0, end = strlen(str) - 1;

    // Trim leading whitespace
    while (isspace(str[start])) 
    {
        start++;
    }

    // Trim trailing whitespace
    while (end > start && isspace(str[end])) 
    {
        end--;
    }

    // Shift characters to the beginning of the string
    for (int i = 0; i <= end - start; i++) 
    {
        str[i] = str[start + i];
    }

    // Null-terminate the trimmed string
    str[end - start + 1] = '\0';
}

void printStars(float averageRating) 
{
    int fullStars = (int)averageRating;
    int halfStar = (averageRating - fullStars) >= 0.5 ? 1 : 0;

    for (int i = 0; i < fullStars; i++) 
    {
        printf("★");
    }
    if (halfStar) 
    {
        printf("☆");
    }
    for (int i = fullStars + halfStar; i < 5; i++) 
    {
        printf("☆");
    }
    printf(" (%.1f)\n", averageRating);
}

int main() 
{   FILE* filer = fopen("./RestaurantDatabase.txt", "w");
    fclose(filer);
    FILE* file = fopen("./RestaurantDatabase.txt", "r+");
    if (!file) 
    {
        perror("Unable to open file");
        return 1;
    }
    Restaurant** info = (Restaurant**)malloc(100 * sizeof(Restaurant*));

    char* file_content = (char*)malloc(1000 * sizeof(char));
    int index = 0;

    while (fgets(file_content, 999, file)) 
    { // Read content from file
        info[index] = createInfo();
        char items[10000];
        int no_content_read = sscanf(file_content, "%[^,],%[^,],%[^,],%[^,],%[^\n]\n", info[index]->restaurant, info[index]->location, info[index]->rating, info[index]->best_seller, items);
        if (no_content_read == 5) 
        {
            char* ITEM = strtok(items, ",");
            int item_index = 0;

            while (ITEM != NULL) 
            {
                info[index]->items[item_index++] = strdup(ITEM);
                ITEM = strtok(NULL, ",");
            }
            info[index]->items[item_index] = NULL;
        }
        index++;
    }

    char rest[50], p[50], rate[50], update_rating[50];
    float changed_rating;

    printf("Enter the restaurant\n");
    fgets(rest, 50, stdin);
    trim(rest);

    printf("\nEnter rating(Out of 5)\n");
    fgets(rate, 50, stdin);
    trim(rate);

    float new_rating, old_rating;
    for (int i = 0; i < index; i++) 
    {
        strcpy(p, info[i]->restaurant);
        trim(p);
        if (strcasecmp(p, rest) == 0) 
        {
            new_rating = strtof(rate, NULL);
            old_rating = strtof(info[i]->rating, NULL);
            changed_rating = (new_rating + old_rating) / 2;
            sprintf(update_rating, "%f", changed_rating);
            strcpy(info[i]->rating, update_rating);
            break;
        }
    }

    printf("Your rating : ");
    printStars(new_rating);
    printf("\n");
    printf("Rating before your feedback : ");
    printStars(old_rating);
    printf("\n");
    printf("Rating after your feedback : ");
    printStars(changed_rating);
    printf("\n");
    printf("Thanks for helping us!!\n");

    // Write the updated data back to the file
    freopen("RestaurantDatabase.txt", "w", file); // Reopen the file in write mode to overwrite

    for (int i = 0; i < index; i++) 
    {
        fprintf(file, "%s,%s,%s,%s", info[i]->restaurant, info[i]->location, info[i]->rating, info[i]->best_seller);
        for (int j = 0; info[i]->items[j] != NULL; j++) 
        {
            if (j == 0) 
            {
                fprintf(file, ",%s", info[i]->items[j]);
            } 
            else 
            {
                fprintf(file, ",%s", info[i]->items[j]);
            }
        }
        fprintf(file, "\n");
    }

    // Close the file and free memory
    fclose(file);
    for (int i = 0; i < index; i++) 
    {
        free(info[i]->restaurant);
        free(info[i]->location);
        free(info[i]->rating);
        free(info[i]->best_seller);
        for (int j = 0; info[i]->items[j] != NULL; j++) 
        {
            free(info[i]->items[j]);
        }
        free(info[i]->items);
        free(info[i]);
    }
    free(info);
    free(file_content);

    return 0;
}
