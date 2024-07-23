#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>


#ifndef PET_H
#define PET_H

#define MAX_ITEMS 5
#define MAX_SHOPS 10
#define MAX_NAME_LENGTH 50
#define MAX_CART_ITEMS 50

#define GST_RATE 0.18
#define DELIVERY_CHARGE_RATE 0.05
#define DELIVERY_SPEED 40.0  // Speed in km/hr

typedef struct {
    char name[MAX_NAME_LENGTH];
    float cost;
    int quantity;
} CartItem;

typedef struct {
    char name[MAX_NAME_LENGTH];
    float cost;
    int stock;  // Keeping this for internal use, but it won't be displayed
} PetItem;

typedef struct {
    char shopName[MAX_NAME_LENGTH];
    char location[MAX_NAME_LENGTH];
    PetItem items[MAX_ITEMS];
} Shop;

// Define distances for locations
typedef struct {
    char location[MAX_NAME_LENGTH];
    float distance;  // in km
} LocationDistance;

LocationDistance locationDistances[] = {
    {"Anna Nagar", 8.0},
    {"Sholinganallur", 7.0},
    {"Adyar", 5.0},
    {"Kalavakkam", 6.0},
    {"Neelankarai", 9.0}
};

void petToLowerCase(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);
    }
}

float getDistance(const char *location) {
    for (int i = 0; i < sizeof(locationDistances) / sizeof(LocationDistance); i++) {
        if (strcasecmp(locationDistances[i].location, location) == 0) {
            return locationDistances[i].distance;
        }
    }
    return -1.0;  // Location not found
}

void readDataFromFile(const char *filename, Shop shops[], int *numShops) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file.\n");
        exit(1);
    }

    char line[256];
    *numShops = 0;

    while (fgets(line, sizeof(line), file) && *numShops < MAX_SHOPS) {
        Shop *shop = &shops[*numShops];
        char *token = strtok(line, ",");
        if (token == NULL) continue;
        strcpy(shop->shopName, token);

        token = strtok(NULL, ",");
        if (token == NULL) continue;
        strcpy(shop->location, token);

        for (int i = 0; i < MAX_ITEMS; i++) {
            token = strtok(NULL, ",");
            if (token == NULL) continue;
            strcpy(shop->items[i].name, token);

            token = strtok(NULL, ",");
            if (token == NULL) continue;
            shop->items[i].cost = atof(token);

            token = strtok(NULL, ",");
            if (token == NULL) continue;
            shop->items[i].stock = atoi(token);
        }
        (*numShops)++;
    }

    fclose(file);
}

void displayShopsByLocation(Shop shops[], int numShops, const char *location) {
    char lowerLocation[MAX_NAME_LENGTH];
    strcpy(lowerLocation, location);
    toLowerCase(lowerLocation);

    printf("\nShops and items in location: %s\n", location);
    printf("=========================================\n");

    for (int i = 0; i < numShops; i++) {
        char shopLocation[MAX_NAME_LENGTH];
        strcpy(shopLocation, shops[i].location);
        toLowerCase(shopLocation);

        if (strcmp(shopLocation, lowerLocation) == 0) {
            printf("Shop: %s\n", shops[i].shopName);
            printf("-------------------------------------------------------\n");
            printf("| %-30s | %-15s |\n", "Item Name", "Cost (INR)");
            printf("-------------------------------------------------------\n");

            for (int j = 0; j < MAX_ITEMS; j++) {
                if (strlen(shops[i].items[j].name) > 0) {
                    printf("| %-30s | %-13.2f |\n", shops[i].items[j].name, shops[i].items[j].cost);
                }
            }
            printf("-------------------------------------------------------\n");
        }
    }
}

void displayItemsByAnimal(Shop shops[], int numShops, const char *location, int animalIndex, CartItem cart[], int *cartSize) {
    char lowerLocation[MAX_NAME_LENGTH];
    strcpy(lowerLocation, location);
    toLowerCase(lowerLocation);

    char *animalTypes[] = {"Fish", "Hamster", "Dog", "Cat", "Bird"};

    printf("\nItems for animal: %s\n", animalTypes[animalIndex]);
    printf("=========================================\n");

    char animalSuffix[4];  // For storing the animal suffix like "(C)"
    snprintf(animalSuffix, sizeof(animalSuffix), "(%c)", toupper(animalTypes[animalIndex][0]));

    for (int i = 0; i < numShops; i++) {
        char shopLocation[MAX_NAME_LENGTH];
        strcpy(shopLocation, shops[i].location);
        toLowerCase(shopLocation);

        if (strcmp(shopLocation, lowerLocation) == 0) {
            printf("Shop: %s\n", shops[i].shopName);
            printf("-------------------------------------------------------\n");
            printf("| %-30s | %-15s |\n", "Item Name", "Cost (INR)");
            printf("-------------------------------------------------------\n");

            for (int j = 0; j < MAX_ITEMS; j++) {
                if (strstr(shops[i].items[j].name, animalSuffix) != NULL) {
                    printf("| %-30s | %-13.2f |\n", shops[i].items[j].name, shops[i].items[j].cost);
                    printf("-------------------------------------------------------\n");

                    // Ask user if they want to add item to cart
                    int quantity;
                    printf("Enter quantity for %s: ", shops[i].items[j].name);
                    while (scanf("%d", &quantity) != 1 || quantity <= 0) {
                        printf("Invalid quantity. Enter a positive number for %s: ", shops[i].items[j].name);
                        while (getchar() != '\n');  // Clear the input buffer
                    }

                    // Add the item to the cart
                    CartItem newItem;
                    strcpy(newItem.name, shops[i].items[j].name);
                    newItem.cost = shops[i].items[j].cost;
                    newItem.quantity = quantity;
                    cart[*cartSize] = newItem;
                    (*cartSize)++;

                    return;  // Return after adding the item to allow for another animal choice
                }
            }
        }
    }
}

void displayCart(CartItem cart[], int cartSize) {
    if (cartSize == 0) {
        printf("Your cart is empty.\n");
        return;
    }

    printf("\nYour cart:\n");
    printf("----------------------------------------------------------------------------------------------------\n");
    printf("| %-4s | %-30s | %-10s | %-15s | %-15s |\n", "No.", "Item Name", "Quantity", "Cost per Item ()", "Total Cost ()");
    printf("----------------------------------------------------------------------------------------------------\n");

    for (int i = 0; i < cartSize; i++) {
        printf("| %-4d | %-30s | %-10d | %-13.2f | %-13.2f |\n", i + 1, cart[i].name, cart[i].quantity, cart[i].cost, cart[i].cost * cart[i].quantity);
    }

    printf("----------------------------------------------------------------------------------------------------\n");
}

void editCart(CartItem cart[], int *cartSize) {
    char choice;
    do {
        displayCart(cart, *cartSize);

        printf("\nDo you want to edit the cart? (y/n): ");
        scanf(" %c", &choice);

        while (tolower(choice) != 'y' && tolower(choice) != 'n') {
            printf("Invalid choice. Please enter 'y' or 'n': ");
            scanf(" %c", &choice);
        }

        if (tolower(choice) == 'y') {
            int itemNumber;
            printf("Enter the number of the item to edit: ");
            while (scanf("%d", &itemNumber) != 1 || itemNumber < 1 || itemNumber > *cartSize) {
                printf("Invalid item number. Enter a number between 1 and %d: ", *cartSize);
                while (getchar() != '\n');  // Clear the input buffer
            }

            int newQuantity;
            printf("Enter new quantity for %s (0 to remove): ", cart[itemNumber - 1].name);
            while (scanf("%d", &newQuantity) != 1 || newQuantity < 0) {
                printf("Invalid quantity. Enter a non-negative number for %s: ", cart[itemNumber - 1].name);
                while (getchar() != '\n');  // Clear the input buffer
            }

            if (newQuantity == 0) {
                // Remove the item from the cart
                for (int i = itemNumber - 1; i < *cartSize - 1; i++) {
                    cart[i] = cart[i + 1];
                }
                (*cartSize)--;
            } else {
                cart[itemNumber - 1].quantity = newQuantity;
            }
        }

    } while (tolower(choice) == 'y');
}

void calculateAndDisplayBill(CartItem cart[], int cartSize, float distance) {
    float totalCost = 0;
    for (int i = 0; i < cartSize; i++) {
        totalCost += cart[i].cost * cart[i].quantity;
    }

    float gst = totalCost * GST_RATE;
    float deliveryCharges = totalCost * DELIVERY_CHARGE_RATE;
    float finalAmount = totalCost + gst + deliveryCharges;

    float estimatedTime = distance / DELIVERY_SPEED * 60; // Time in minutes

    printf("\nBill Summary:\n");
    printf("-------------------------------------------------------\n");
    printf("Total Cost:            %.2f\n", totalCost);
    printf("GST (18%%):             %.2f\n", gst);
    printf("Delivery Charges (5%%): %.2f\n", deliveryCharges);
    printf("-------------------------------------------------------\n");
    printf("Final Amount:          %.2f\n", finalAmount);
    printf("-------------------------------------------------------\n");
    printf("Distance:              %.2f km\n", distance);
    printf("Estimated Delivery Time: %.2f minutes\n", estimatedTime);
}

void writeDataToFile(const char *filename, Shop shops[], int numShops) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        printf("Error opening file for writing.\n");
        exit(1);
    }

    for (int i = 0; i < numShops; i++) {
        fprintf(file, "%s,%s,", shops[i].shopName, shops[i].location);
        for (int j = 0; j < MAX_ITEMS; j++) {
            fprintf(file, "%s,%.2f,%d,", shops[i].items[j].name, shops[i].items[j].cost, shops[i].items[j].stock);
        }
        fprintf(file, "\n");
    }

    fclose(file);
}

void addItemToCart(Shop shops[], int numShops, const char *location, CartItem cart[], int *cartSize) {
    int animalChoice;
    do {
        printf("Enter animal type to add items (1: Fish, 2: Hamster, 3: Dog, 4: Cat, 5: Bird, 0: Cancel): ");
        while (scanf("%d", &animalChoice) != 1 || animalChoice < 0 || animalChoice > 5) {
            printf("Invalid choice. Enter a number between 0 and 5: ");
            while (getchar() != '\n');  // Clear the input buffer
        }

        if (animalChoice > 0 && animalChoice <= 5) {
            displayItemsByAnimal(shops, numShops, location, animalChoice - 1, cart, cartSize);
        } else if (animalChoice != 0) {
            printf("Invalid choice. Please try again.\n");
        }
    } while (animalChoice != 0);
}

int petMain(char * userLocation) {
    Shop shops[MAX_SHOPS];
    int numShops;
   // char userLocation[MAX_NAME_LENGTH];
    int animalChoice;
    CartItem cart[MAX_CART_ITEMS];
    int cartSize = 0;

    readDataFromFile("./data/pet-food.txt", shops, &numShops);

    // printf("Enter your location: ");
    // scanf(" %[^\n]", userLocation);  // Use %[^\n] to read the entire line including spaces

    printf("\n Uer Location: %s\n", userLocation);

    float distance = getDistance(userLocation);
    if (distance < 0) {
        printf("Invalid location.\n");
        return 1;
    }

    displayShopsByLocation(shops, numShops, userLocation);

    do {
        printf("Enter animal type (1: Fish, 2: Hamster, 3: Dog, 4: Cat, 5: Bird, 0: Checkout): ");
        while (scanf("%d", &animalChoice) != 1 || animalChoice < 0 || animalChoice > 5) {
            printf("Invalid choice. Enter a number between 0 and 5: ");
            while (getchar() != '\n');  // Clear the input buffer
        }

        if (animalChoice > 0 && animalChoice <= 5) {
            displayItemsByAnimal(shops, numShops, userLocation, animalChoice - 1, cart, &cartSize);
        } else if (animalChoice != 0) {
            printf("Invalid choice. Please try again.\n");
        }
    } while (animalChoice != 0);

    // If the cart is empty, prompt the user to add items
    if (cartSize == 0) {
        char addItems;
        printf("Your cart is empty. Do you want to add items? (y/n): ");
        scanf(" %c", &addItems);

        while (tolower(addItems) != 'y' && tolower(addItems) != 'n') {
            printf("Invalid choice. Please enter 'y' or 'n': ");
            scanf(" %c", &addItems);
        }

        if (tolower(addItems) == 'y') {
            addItemToCart(shops, numShops, userLocation, cart, &cartSize);
        }
    }

    // Edit the final cart
    editCart(cart, &cartSize);

    // Display the final cart
    displayCart(cart, cartSize);

    // Generate and display the bill
    calculateAndDisplayBill(cart, cartSize, distance);

    // Update stock in the database file
    for (int i = 0; i < cartSize; i++) {
        for (int j = 0; j < numShops; j++) {
            for (int k = 0; k < MAX_ITEMS; k++) {
                if (strcmp(cart[i].name, shops[j].items[k].name) == 0) {
                    shops[j].items[k].stock -= cart[i].quantity;
                    break;
                }
            }
        }
    }

    // Write the updated data back to the file
    writeDataToFile("PetFood.txt", shops, numShops);

    return 0;
}
#endif