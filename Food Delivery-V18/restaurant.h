#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include "Utility.h"
#include "customer.h"
#include "utility.h"
#include "dijkstra.h"

#ifndef RESTAURANT_H
#define RESTAURANT_H

/*
    This file handles all restaurant related functions
*/

#define MAX_ITEMS 20        // the maximum number of items per restaurant
#define MAX_RESTAURANTS 100 // the maximum number of restaurants
#define RESTAURANT_FILENAME "./data/restaurants.dat"

//---------------------------------------------------------------------------------//
// ------------------- Structure Definitions---------------------------------------//
//---------------------------------------------------------------------------------//

// Holds individual menu item data
typedef struct Item
{
    char name[50];
    char ID[7];
    char allergen[50];
    bool availability;
    float price;
} Item;

// Holds individual restaurant data
typedef struct
{
    char name[50];
    char ID[7];
    char cuisine[20];
    char time[20];
    float rating;
    bool veg; // Added veg field
    Item items[MAX_ITEMS];
    char location[100];
    float distance;
} Restaurant;

//---------------------------------------------------------------------------------//
// ------------------- Method Definitions---------- -------------------------------//
//---------------------------------------------------------------------------------//
bool isIDUnique(const char *ID);
void generateUniqueID(char *ID, int size, char startLetter);
int addRestaurant(Restaurant *newRestaurant);
int getRestaurant(const char *ID, Restaurant *restaurant);
float getItemPrice(const char *restaurantID, const char *itemID);
int isItemAvailable(const char *restaurantID, const char *itemID);
Restaurant *filterRestaurantsByCuisine(Restaurant *restaurants, int count, const char *cuisine, int *filteredCount);
Restaurant *filterRestaurantsByVeg(Restaurant *restaurants, int count, bool isVeg, int *filteredCount);
void sortRestaurants(Restaurant *restaurants, int count, int (*compare)(const void *, const void *));
void sortItems(Item *items, int count, int sortOrder);
int compareByRating(const void *a, const void *b);
int compareByDistance(const void *a, const void *b);
int compareByPrice(const void *a, const void *b);
void displayRestaurants(Restaurant *restaurants, int count, char *userLocation);
int displayRestaurantItems(const Restaurant *restaurant);
int updateRestaurant(const char *ID, Restaurant *updatedRestaurant);
void removeRestaurant(const char *ID);

//---------------------------------------------------------------------------------//
// ------------------- Method Implementation Starts -------------------------------//
//---------------------------------------------------------------------------------//

//
// Function to check if a generated ID already exists in the restaurant db file
//
bool isIDUnique(const char *ID)
{
    FILE *file = fopen(RESTAURANT_FILENAME, "rb");
    if (file != NULL)
    {
        Restaurant restaurant;
        while (fread(&restaurant, sizeof(Restaurant), 1, file) == 1)
        {
            if (strcmp(restaurant.ID, ID) == 0)
            {
                fclose(file);
                return false;
            }
            for (int i = 0; i < MAX_ITEMS; i++)
            {
                if (strcmp(restaurant.items[i].ID, ID) == 0)
                {
                    fclose(file);
                    return false;
                }
            }
        }
        fclose(file);
    }
    return true;
}

//
// Function to generate a unique restaurant ID
//
void generateUniqueID(char *ID, int size, char startLetter)
{
    do
    {
        strcpy(ID, generateRandomID(size, startLetter));
    } while (!isIDUnique(ID));
}

//
// Function to add a new restaurant to the file
//
int addRestaurant(Restaurant *newRestaurant)
{
    int retValue = 1;

    FILE *file = fopen(RESTAURANT_FILENAME, "ab"); // open file in append mode
    if (file != NULL)
    {
        fwrite(newRestaurant, sizeof(Restaurant), 1, file); // write restaurant to file
        fclose(file);
        retValue = 0;
    }
    else
    {
        printf("Error opening %s file.\n", RESTAURANT_FILENAME);
    }

    return retValue;
}

//
// Function to retrieve a restaurant from the file based on ID
//
int getRestaurant(const char *ID, Restaurant *restaurant)
{
    FILE *file = fopen(RESTAURANT_FILENAME, "rb"); // open file in read mode
    if (file != NULL)
    {
        while (fread(restaurant, sizeof(Restaurant), 1, file) == 1)
        {
            if (strcmp(restaurant->ID, ID) == 0)
            { // find matching restaurant
                fclose(file);
                return 1;
            }
        }
        fclose(file);
    }
    else
    {
        printf("Error opening file.\n");
    }
    return -1;
}

//
// Read all restaurants
//
int readRestaurants(Restaurant **restaurants)
{
    FILE *file = fopen(RESTAURANT_FILENAME, "rb");
    if (!file)
    {
        printf("Error opening %s file.\n", RESTAURANT_FILENAME);
        return -1;
    }

    // Allocate memory for the restaurants
    *restaurants = malloc(MAX_RESTAURANTS * sizeof(Restaurant));
    if (*restaurants == NULL)
    {
        printRed("Memory allocation failed.\n");
        fclose(file);
        return -1;
    }

    int count = 0;
    while (fread(&(*restaurants)[count], sizeof(Restaurant), 1, file) == 1)
    {
        count++;
        if (count >= MAX_RESTAURANTS)
        {
            printf("Reached maximum number of restaurants that can be read - %d\n", count);
            break;
        }
    }

    fclose(file);
    return count;
}

//
// fetches the price of an item with resaurant id and item id
//
float getItemPrice(const char *restaurantID, const char *itemID)
{
    FILE *file = fopen(RESTAURANT_FILENAME, "rb");
    if (!file)
    {
        printf("Error opening restaurant file.\n");
        return -1;
    }
    Restaurant restaurant;
    while (fread(&restaurant, sizeof(Restaurant), 1, file) == 1)
    {
        if (strcmp(restaurant.ID, restaurantID) == 0)
        {
            for (int i = 0; i < MAX_ITEMS; i++)
            {
                if (strcmp(restaurant.items[i].ID, itemID) == 0)
                {
                    fclose(file);
                    return restaurant.items[i].price;
                }
            }
        }
    }

    fclose(file);
    printf("Item not found.\n");
    return -1;
}

//
// Function to check if an item is available
//
int isItemAvailable(const char *restaurantID, const char *itemID)
{
    Restaurant restaurant;
    getRestaurant(restaurantID, &restaurant);

    if (!&restaurant)
    {
        return 0;
    }

    for (int i = 0; i < MAX_ITEMS; i++)
    {
        if (strcmp(restaurant.items[i].ID, itemID) == 0)
        {
            int availability = restaurant.items[i].availability ? 1 : 0;
            return availability;
        }
    }
    return 0;
}

//
// Utility method used by qsort for compare by restaurant rating
//
int compareByRating(const void *a, const void *b)
{
    const Restaurant *restaurantA = (const Restaurant *)a;
    const Restaurant *restaurantB = (const Restaurant *)b;
    if (restaurantA->rating < restaurantB->rating)
        return 1;
    if (restaurantA->rating > restaurantB->rating)
        return -1;
    return 0;
}

//
// Utility method used by qsort for compare by restaurant distance from customer location
//
int compareByDistance(const void *a, const void *b)
{
    const Restaurant *restaurantA = (const Restaurant *)a;
    const Restaurant *restaurantB = (const Restaurant *)b;
    if (restaurantA->distance < restaurantB->distance)
        return 1 * -1;
    if (restaurantA->distance > restaurantB->distance)
        return -1 * -1;
    return 0;
}

//
// Utility method used by qsort for compare by items
//
int compareByPrice(const void *a, const void *b)
{
    const Item *itemA = (const Item *)a;
    const Item *itemB = (const Item *)b;
    if (itemA->price < itemB->price)
        return 1;
    if (itemA->price > itemB->price)
        return -1;
    return 0;
}

//
// Utility to sort the restaurants
//
void sortRestaurants(Restaurant *restaurants, int count, int (*compare)(const void *, const void *))
{
    qsort(restaurants, count, sizeof(Restaurant), compare);
}

//
// Utility to sort the items
// for a change uses bubble sort
//
void sortItems(Item *items, int count, int sortOrder)
{
    for (int i = 0; i < count - 1; i++)
    {
        for (int j = 0; j < count - i - 1; j++)
        {
            if ((sortOrder == 1 && items[j].price > items[j + 1].price) ||
                (sortOrder == 2 && items[j].price < items[j + 1].price))
            {
                Item temp = items[j];
                items[j] = items[j + 1];
                items[j + 1] = temp;
            }
        }
    }
}

//
// Utililty to filter restaurant by veg
//
Restaurant *filterRestaurantsByVeg(Restaurant *restaurants, int count, bool isVeg, int *filteredCount)
{
    Restaurant *filteredRestaurants = (Restaurant *)malloc(MAX_RESTAURANTS * sizeof(Restaurant));
    if (!filteredRestaurants)
    {
        printf("Memory allocation error.\n");
        *filteredCount = 0;
        return NULL;
    }

    *filteredCount = 0;
    for (int i = 0; i < count; i++)
    {
        if (restaurants[i].veg == isVeg)
        {
            filteredRestaurants[*filteredCount] = restaurants[i];
            (*filteredCount)++;
        }
    }

    return filteredRestaurants;
}

//
// Utililty to filter restaurant by cuisine
//
Restaurant *filterRestaurantsByCuisine(Restaurant *restaurants, int count, const char *cuisine, int *filteredCount)
{
    Restaurant *filteredRestaurants = (Restaurant *)malloc(MAX_RESTAURANTS * sizeof(Restaurant));
    if (!filteredRestaurants)
    {
        printf("Memory allocation error.\n");
        *filteredCount = 0;
        return NULL;
    }

    *filteredCount = 0;
    for (int i = 0; i < count; i++)
    {
        if (strcmp(convertToLowercase(restaurants[i].cuisine), convertToLowercase(cuisine)) == 0)
        {
            filteredRestaurants[*filteredCount] = restaurants[i];
            (*filteredCount)++;
        }
    }

    if (*filteredCount == 0)
    {
        printf("No restaurants found with the cuisine: %s\n", cuisine);
    }

    return filteredRestaurants;
}

//
//  Displays all restaurants
//
void displayRestaurants(Restaurant *restaurants, int count, char *userLocation)
{

    printf("-------------------------------------------------------------------------------------------------------------\n");
    //printf("| %-5s | %-5s | %-30s | %-15s | %-20s | %-6s | %-3s |\n", "SNo", "ID", "Restaurant Name", "Cuisine", "Distance", "Rating", "Veg");
    printf("| %-5s | %-5s | %-30s | %-20s | %-6s | %-3s |\n", "SNo", "ID", "Restaurant Name","Distance", "Rating", "Veg");
    printf("-------------------------------------------------------------------------------------------------------------\n");
    for (int i = 0; i < count; i++)
    {
        restaurants[i].distance = dijk(restaurants[i].name, userLocation);

       // printf("| %-5d | %-5s | %-30s | %-15s | %-20.2f | %-6.1f | %-3s |\n",
        printf("| %-5d | %-5s | %-30s | %-20.2f | %-6.1f | %-3s |\n",
               i + 1,
               restaurants[i].ID,
               restaurants[i].name,
               //restaurants[i].cuisine,
               restaurants[i].distance,
               restaurants[i].rating > 0 ? restaurants[i].rating : 0.0,
               restaurants[i].veg ? "Yes" : "No");
    }
    printf("------------------------------------------------------------------------------------------------------------\n");
}

//
//  Displays all items in a given restaurants
//
int displayRestaurantItems(const Restaurant *restaurant)
{
    if (!restaurant)
    {
        printf("Restaurant not found.\n");
        return 0;
    }

    printf("--------------------------------------------------------------------------------------------------------\n");
    printf("| %-5s| %-7s | %-40s | %-20s | %-12s | %-6s |\n", "SNo", "ID", "Item Name", "Allergen", "Availability", "Price");
    printf("--------------------------------------------------------------------------------------------------------\n");
    int i = 0;
    while (strlen(restaurant->items[i].ID) > 0)
    {
        printf("| %-5d| %-7s | %-40s | %-20s | %-12s | %-5.2f |\n",
               (i + 1),
               restaurant->items[i].ID,
               restaurant->items[i].name,
               strlen(restaurant->items[i].allergen) > 0 ? restaurant->items[i].allergen : "N/A",
               restaurant->items[i].availability ? "Yes" : "No",
               restaurant->items[i].price);
        i++;
    }
    printf("--------------------------------------------------------------------------------------------------------\n");

    return i;
}

//
// UPd Function to update an existing restaurant in the file based on ID
//
int updateRestaurant(const char *ID, Restaurant *updatedRestaurant)
{
    FILE *file = fopen(RESTAURANT_FILENAME, "r+b"); // open file in read/write mode
    if (file != NULL)
    {
        Restaurant restaurant;
        while (fread(&restaurant, sizeof(Restaurant), 1, file) == 1)
        {
            if (strcmp(restaurant.ID, ID) == 0)
            {                                                           // find matching restaurant
                fseek(file, sizeof(Restaurant), SEEK_CUR);              // move file pointer back
                fwrite(updatedRestaurant, sizeof(Restaurant), 1, file); // overwrite existing restaurant
                fclose(file);
                printf("Restaurant updated successfully.\n");
                return 0;
            }
        }
        fclose(file);
        printf("Restaurant with ID %s not found.\n", ID);
    }
    else
    {
        printf("Error opening file.\n");
    }
    return -1;
}

//
// Function to remove a restaurant from the file based on ID
//
void removeRestaurant(const char *ID)
{
    FILE *file = fopen(RESTAURANT_FILENAME, "rb"); // open file in read mode
    FILE *tempFile = fopen("temp.dat", "wb");      // open temporary file in write mode
    if (file != NULL && tempFile != NULL)
    {
        Restaurant restaurant;
        while (fread(&restaurant, sizeof(Restaurant), 1, file) == 1)
        {
            if (strcmp(restaurant.ID, ID) != 0)
            { // copy all except the one to be deleted
                fwrite(&restaurant, sizeof(Restaurant), 1, tempFile);
            }
        }
        fclose(file);
        fclose(tempFile);
        remove(RESTAURANT_FILENAME);
        rename("temp.dat", RESTAURANT_FILENAME);
        printf("Restaurant successfully.\n");
    }
    else
    {
        printf("Error opening file.\n");
    }
}

#endif