#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LENGTH 1024
#define MAX_ITEMS 10
#define MAX_ALLERGENS 10

// Structure to hold information about each restaurant
typedef struct {
    char name[50];
    char location[50];
    float rating;
    char best_seller[100];
    char restaurant_type[10]; // V or NV
    char items[MAX_ITEMS][100];
    float prices[MAX_ITEMS];
    char item_types[MAX_ITEMS][10]; // V or NV
} Restaurant;

// Structure to hold item and price together for sorting
typedef struct {
    char item[100];
    float price;
    char type[10];
} MenuItem;

// Function to print stars based on the average rating
void printStars(float averageRating) {
    int fullStars = (int)averageRating;
    int halfStar = (averageRating - fullStars) >= 0.5 ? 1 : 0;

    for (int i = 0; i < fullStars; i++) {
        printf("★");
    }
    if (halfStar) {
        printf("☆");
    }
    for (int i = fullStars + halfStar; i < 5; i++) {
        printf("☆");
    }
    printf(" (%.1f)\n", averageRating);
}

// Function to convert string to lowercase
void toLowerCase(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);
    }
}

// Function to read and parse the file data
int readRestaurants(Restaurant restaurants[], const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return -1;
    }

    char line[MAX_LINE_LENGTH];
    int restaurantCount = 0;

    while (fgets(line, sizeof(line), file)) {
        Restaurant restaurant;
        char *token = strtok(line, ",");

        strcpy(restaurant.name, token);
        token = strtok(NULL, ",");
        strcpy(restaurant.location, token);
        token = strtok(NULL, ",");
        restaurant.rating = atof(token);
        token = strtok(NULL, ",");
        strcpy(restaurant.best_seller, token);
        token = strtok(NULL, ",");
        strcpy(restaurant.restaurant_type, token);

        int itemCount = 0;
        while (token != NULL && itemCount < MAX_ITEMS) {
            token = strtok(NULL, ",");
            if (token == NULL) break;
            strcpy(restaurant.items[itemCount], token);

            token = strtok(NULL, ",");
            if (token == NULL) break;
            restaurant.prices[itemCount] = atof(token);

            token = strtok(NULL, ",");
            if (token == NULL) break;
            strcpy(restaurant.item_types[itemCount], token);

            itemCount++;
        }

        restaurants[restaurantCount] = restaurant;
        restaurantCount++;
    }

    fclose(file);
    return restaurantCount;
}

// Function to check if the entered restaurant exists
int restaurantExists(char *input_restaurant, Restaurant restaurants[], int restaurant_count) {
    char restaurant_name_lower[50];
    for (int i = 0; i < restaurant_count; i++) {
        strcpy(restaurant_name_lower, restaurants[i].name);
        toLowerCase(restaurant_name_lower);
        if (strcmp(input_restaurant, restaurant_name_lower) == 0) {
            return i;
        }
    }
    return -1;
}

// Function to check if the entered allergens are valid
int validAllergens(char allergens[][3], int allergen_count) {
    const char valid_allergens[MAX_ALLERGENS][3] = {"m", "p", "e"};
    for (int i = 0; i < allergen_count; i++) {
        int valid = 0;
        for (int j = 0; j < 3; j++) { // Only 3 valid allergens
            if (strcmp(allergens[i], valid_allergens[j]) == 0) {
                valid = 1;
                break;
            }
        }
        if (!valid) {
            return 0;
        }
    }
    return 1;
}

// Function to sort menu items by price
void sortMenuItemsByPrice(MenuItem menu[], int count, int sort_order) {
    for (int i = 0; i < count - 1; i++) {
        for (int j = 0; j < count - i - 1; j++) {
            if ((sort_order == 1 && menu[j].price > menu[j + 1].price) || 
                (sort_order == 2 && menu[j].price < menu[j + 1].price)) {
                MenuItem temp = menu[j];
                menu[j] = menu[j + 1];
                menu[j + 1] = temp;
            }
        }
    }
}

// Function to remove allergen information from item names
void removeAllergenInfo(char *item) {
    char *paren = strchr(item, '(');
    if (paren) {
        *paren = '\0';  // Truncate the string at the '(' character
    }
}

// Function to display the menu in a tabular format with outline border
void displayMenu(MenuItem menu[], int menu_count) {
    printf("\nMenu:\n");

    printf("+--------+------------------------------------------------+------------+--------+\n");
    printf("| %-6s | %-48s | %-10s | %-6s |\n", "Number", "Item", "Cost", "Type");
    printf("+--------+------------------------------------------------+------------+--------+\n");

    for (int i = 0; i < menu_count; i++) {
        printf("| %-6d | %-48s | ₹%-9.2f | %-6s |\n", i + 1, menu[i].item, menu[i].price, menu[i].type);
    }

    printf("+--------+------------------------------------------------+------------+--------+\n");
}

// Function to display restaurant information
void displayRestaurantInfo(Restaurant restaurant) {
    printf("\nRestaurant: %s\n", restaurant.name);
    printf("Location  : %s\n", restaurant.location);
    printf("Rating    : ");
    printStars(restaurant.rating);
    printf("Best Seller: %s\n", restaurant.best_seller);
    //printf("Type      : %s\n", restaurant.restaurant_type);
}

int main() {
    Restaurant restaurants[50];
    int restaurant_count = readRestaurants(restaurants, "RestaurantDatabase.txt");

    if (restaurant_count < 0) {
        printf("Failed to load restaurants.\n");
        return 1;
    }

    char input_restaurant[50];
    char allergens_input[100];
    char allergens[MAX_ALLERGENS][3]; // Allow up to 10 allergens
    int allergen_count = 0;
    int restaurant_index;
    int sort_order;
    int veg_preference;

    // Get valid restaurant name
    do {
        printf("\nEnter the restaurant name: ");
        fgets(input_restaurant, sizeof(input_restaurant), stdin);
        input_restaurant[strcspn(input_restaurant, "\n")] = '\0'; // Remove trailing newline
        toLowerCase(input_restaurant);

        restaurant_index = restaurantExists(input_restaurant, restaurants, restaurant_count);
        if (restaurant_index == -1) {
            printf("Restaurant not found. Please try again.\n");
        }
    } while (restaurant_index == -1);

    // Get valid allergens
    do {
        printf("\nThe allergens you can avoid are egg, milk and peanuts.\nEnter m for milk\nEnter e for egg\nEnter p for peanuts\nEnter the allergen to avoid : ");
        fgets(allergens_input, sizeof(allergens_input), stdin);
        allergens_input[strcspn(allergens_input, "\n")] = '\0'; // Remove trailing newline
        toLowerCase(allergens_input);

        if (strcmp(allergens_input, "none") == 0) {
            break;
        }

        char *token = strtok(allergens_input, " ,");
        allergen_count = 0;
        while (token != NULL && allergen_count < MAX_ALLERGENS) {
            strcpy(allergens[allergen_count], token);
            allergen_count++;
            token = strtok(NULL, " ,");
        }

        if (!validAllergens(allergens, allergen_count)) {
            printf("Invalid allergens entered. Please try again.\n");
        }
    } while (!validAllergens(allergens, allergen_count));

    // Get sort order for price
    do {
        printf("\nEnter 1 to sort by price (low to high)\nEnter 2 to sort by price (high to low):\nEnter your choice : ");
        if (scanf("%d", &sort_order) != 1) {
            while (getchar() != '\n'); // Clear invalid input
            sort_order = 0; // Reset sort_order to ensure loop continues
        }
    } while (sort_order != 1 && sort_order != 2);

    // Get vegetarian preference
    do {
        printf("\nEnter 1 for Vegetarian items\nEnter 2 for Non-Vegetarian items\nEnter 3 for both\nEnter your choice : ");
        if (scanf("%d", &veg_preference) != 1) {
            while (getchar() != '\n'); // Clear invalid input
            veg_preference = 0; // Reset veg_preference to ensure loop continues
        }
    } while (veg_preference != 1 && veg_preference != 2 && veg_preference != 3);

    // Extract and filter menu items based on allergens and vegetarian preference
    MenuItem menu[MAX_ITEMS];
    int menu_count = 0;
    for (int i = 0; i < MAX_ITEMS && restaurants[restaurant_index].prices[i] != 0; i++) {
        int contains_allergen = 0;
        for (int j = 0; j < allergen_count; j++) {
            if (strstr(restaurants[restaurant_index].items[i], allergens[j]) != NULL) {
                contains_allergen = 1;
                break;
            }
        }
        if (!contains_allergen) {
            if ((veg_preference == 1 && strcmp(restaurants[restaurant_index].item_types[i], "V") == 0) ||
                (veg_preference == 2 && strcmp(restaurants[restaurant_index].item_types[i], "NV") == 0) ||
                veg_preference == 3) {
                strcpy(menu[menu_count].item, restaurants[restaurant_index].items[i]);
                menu[menu_count].price = restaurants[restaurant_index].prices[i];
                strcpy(menu[menu_count].type, restaurants[restaurant_index].item_types[i]);
                removeAllergenInfo(menu[menu_count].item);
                menu_count++;
            }
        }
    }

    // Sort menu by price
    sortMenuItemsByPrice(menu, menu_count, sort_order);

    // Display restaurant info and sorted menu
    displayRestaurantInfo(restaurants[restaurant_index]);
    displayMenu(menu, menu_count);

    return 0;
}
