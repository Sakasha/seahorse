#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#ifndef GROCERY_H
#define GROCERY_H



#define MAX_STORES 20
#define MAX_ITEMS 20
#define MAX_NAME_LEN 50

typedef struct {
    char name[MAX_NAME_LEN];
    float cost;
    int stock;
} Item;

typedef struct {
    char name[MAX_NAME_LEN];
    char location[MAX_NAME_LEN];
    Item items[MAX_ITEMS];
    int itemCount;
    float distance;
} Store;

typedef struct {
    char itemName[MAX_NAME_LEN];
    float cost;
    int quantity;
} CartItem;

void toLowerCase(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);
    }
}

void readFile(const char *filename, Store stores[], int *storeCount) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Unable to open file");
        exit(1);
    }

    char line[1024];
    *storeCount = 0;

    while (fgets(line, sizeof(line), file)) {
        char *token = strtok(line, ",");
        if (!token) continue;
        strcpy(stores[*storeCount].name, token);

        token = strtok(NULL, ",");
        if (!token) continue;
        strcpy(stores[*storeCount].location, token);

        stores[*storeCount].distance = 0.0;

        stores[*storeCount].itemCount = 0;

        while ((token = strtok(NULL, ",")) != NULL) {
            if (stores[*storeCount].itemCount >= MAX_ITEMS) {
                printf("Exceeded maximum number of items for store %s\n", stores[*storeCount].name);
                break;
            }

            strcpy(stores[*storeCount].items[stores[*storeCount].itemCount].name, token);

            token = strtok(NULL, ",");
            if (!token) break;
            stores[*storeCount].items[stores[*storeCount].itemCount].cost = atof(token);

            token = strtok(NULL, ",");
            if (!token) break;
            stores[*storeCount].items[stores[*storeCount].itemCount].stock = atoi(token);

            stores[*storeCount].itemCount++;
        }

        (*storeCount)++;
        if (*storeCount >= MAX_STORES) {
            printf("Exceeded maximum number of stores.\n");
            break;
        }
    }

    fclose(file);
}

void writeFile(const char *filename, Store stores[], int storeCount) {
    FILE *file = fopen(filename, "w");
    if (!file) {
        perror("Unable to open file");
        exit(1);
    }

    for (int i = 0; i < storeCount; i++) {
        fprintf(file, "%s,%s", stores[i].name, stores[i].location);
        for (int j = 0; j < stores[i].itemCount; j++) {
            fprintf(file, ",%s,%.2f,%d", stores[i].items[j].name, stores[i].items[j].cost, stores[i].items[j].stock);
        }
        fprintf(file, "\n");
    }

    fclose(file);
}

void displayStoresByLocation(Store stores[], int storeCount, const char *location, int matchingStores[], int *matchingCount) {
    printf("Shops and items in location: %s\n", location);
    printf("===============================================\n");
    *matchingCount = 0;
    for (int i = 0; i < storeCount; i++) {
        if (strcasecmp(stores[i].location, location) == 0) {
            matchingStores[*matchingCount] = i;
            (*matchingCount)++;
            printf("%d. %s\n", *matchingCount, stores[i].name);
        }
    }
}

void displayStoreItems(Store store) {
    printf("Shop: %s\n", store.name);
    printf("------------------------------------------\n");
    printf("| No. | Item Name            | Cost (₹) |\n");
    printf("------------------------------------------\n");
    for (int i = 0; i < store.itemCount; i++) {
        printf("| %-3d | %-20.20s | ₹%7.2f  |\n", i + 1, store.items[i].name, store.items[i].cost);
    }
    printf("------------------------------------------\n");
}

void displayCart(CartItem cart[], int cartCount) {
    printf("Your cart:\n");
    printf("-------------------------------------------------------------------------------\n");
    printf("| No. | Item Name            | Quantity | Cost per Item (₹) | Total Cost (₹) |\n");
    printf("-------------------------------------------------------------------------------\n");
    float totalCost = 0;
    for (int i = 0; i < cartCount; i++) {
        float itemTotal = cart[i].cost * cart[i].quantity;
        totalCost += itemTotal;
        printf("| %-3d | %-20.20s | %8d | ₹%15.2f | ₹%14.2f |\n", i + 1, cart[i].itemName, cart[i].quantity, cart[i].cost, itemTotal);
    }
    printf("-------------------------------------------------------------------------------\n");
    printf("Total Cost: ₹%.2f\n", totalCost);
}

void generateBill(CartItem cart[], int cartCount, Store *store) {
    float totalCost = 0;
    for (int i = 0; i < cartCount; i++) {
        totalCost += cart[i].cost * cart[i].quantity;
    }
    float gst = totalCost * 0.18;
    float deliveryCharge = totalCost * 0.05;
    float finalAmount = totalCost + gst + deliveryCharge;
    float travelTime = store->distance / 40 * 60;  // in minutes

    printf("\nBill Summary:\n");
    printf("===============================================\n");
    printf("Total Cost: ₹%.2f\n", totalCost);
    printf("GST (18%%): ₹%.2f\n", gst);
    printf("Delivery Charges (5%%): ₹%.2f\n", deliveryCharge);
    printf("Final Amount: ₹%.2f\n", finalAmount);
    printf("===============================================\n");
    printf("Distance to store: %.1f km\n", store->distance);
    printf("Estimated delivery time: %.1f minutes\n", travelTime);
    printf("===============================================\n");

    // Decrement the stock based on the final cart
    for (int i = 0; i < cartCount; i++) {
        for (int j = 0; j < store->itemCount; j++) {
            if (strcmp(store->items[j].name, cart[i].itemName) == 0) {
                store->items[j].stock -= cart[i].quantity;
                break;
            }
        }
    }
}

char getValidatedInput(char *prompt) {
    char input;
    while (1) {
        printf("%s", prompt);
        scanf(" %c", &input);
        input = tolower(input);
        if (input == 'y' || input == 'n') {
            return input;
        } else {
            printf("Invalid input! Please enter 'y' or 'n'.\n");
        }
    }
}

void setStoreDistances(Store stores[], int storeCount) {
    for (int i = 0; i < storeCount; i++) {
        if (strcmp(stores[i].name, "The Green Grocer") == 0) stores[i].distance = 3.0;
        else if (strcmp(stores[i].name, "Garden Fresh Foods") == 0) stores[i].distance = 4.0;
        else if (strcmp(stores[i].name, "Harvest Haven Grocery") == 0) stores[i].distance = 6.0;
        else if (strcmp(stores[i].name, "Nature's Pantry") == 0) stores[i].distance = 7.0;
        else if (strcmp(stores[i].name, "Farm to Fork Market") == 0) stores[i].distance = 8.0;
        else if (strcmp(stores[i].name, "The Organic Oasis") == 0) stores[i].distance = 9.0;
        else if (strcmp(stores[i].name, "Market Fresh Provisions") == 0) stores[i].distance = 10.0;
        else if (strcmp(stores[i].name, "Urban Harvest Grocery") == 0) stores[i].distance = 11.0;
        else if (strcmp(stores[i].name, "Healthy Harvest Marketplace") == 0) stores[i].distance = 6.0;
        else if (strcmp(stores[i].name, "Pure Produce Market") == 0) stores[i].distance = 7.0;
    }
}

int gMain() {
    Store stores[MAX_STORES];
    int storeCount;

    readFile("Groce.txt", stores, &storeCount);

    // Set distances for stores
    setStoreDistances(stores, storeCount);

    char userLocation[MAX_NAME_LEN];
    printf("Enter your location: ");
    // Use fgets instead of scanf to read multi-word input
    fgets(userLocation, sizeof(userLocation), stdin);
    // Remove trailing newline character if present
    size_t len = strlen(userLocation);
    if (len > 0 && userLocation[len - 1] == '\n') {
        userLocation[len - 1] = '\0';
    }
    toLowerCase(userLocation);

    int matchingStores[MAX_STORES];
    int matchingCount = 0;
    displayStoresByLocation(stores, storeCount, userLocation, matchingStores, &matchingCount);

    if (matchingCount == 0) {
        printf("No stores found in your location.\n");
        return 1;
    }

    int storeChoice;
    printf("Enter the store number: ");
    scanf("%d", &storeChoice);
    storeChoice--;

    if (storeChoice < 0 || storeChoice >= matchingCount) {
        printf("Invalid store choice!\n");
        return 1;
    }

    Store *selectedStore = &stores[matchingStores[storeChoice]];
    displayStoreItems(*selectedStore);

    CartItem cart[MAX_ITEMS];
    int cartCount = 0;
    char addItem = 'y';

    while (addItem == 'y') {
        int itemNumber, quantity;
        printf("Enter the item number to add to cart: ");
        scanf("%d", &itemNumber);
        itemNumber--;

        if (itemNumber < 0 || itemNumber >= selectedStore->itemCount) {
            printf("Invalid item number!\n");
            continue;
        }

        printf("Enter the quantity: ");
        scanf("%d", &quantity);

        if (quantity > selectedStore->items[itemNumber].stock) {
            printf("Error: Quantity exceeds stock! Available stock: %d\n", selectedStore->items[itemNumber].stock);
            continue;
        }

        cart[cartCount].cost = selectedStore->items[itemNumber].cost;
        strcpy(cart[cartCount].itemName, selectedStore->items[itemNumber].name);
        cart[cartCount].quantity = quantity;
        cartCount++;

        addItem = getValidatedInput("Do you want to add more items? (y/n): ");
    }

    displayCart(cart, cartCount);

    char editCart = getValidatedInput("Do you want to edit the cart? (y/n): ");

    while (editCart == 'y') {
        int editChoice;
        printf("Enter the number of the item to edit/remove: ");
        scanf("%d", &editChoice);
        editChoice--;

        if (editChoice < 0 || editChoice >= cartCount) {
            printf("Invalid item number!\n");
            continue;
        }

        int newQuantity;
        printf("Enter the new quantity (0 to remove the item): ");
        scanf("%d", &newQuantity);

        // Find the item number in the store based on the cart item name
        int itemNumber = -1;
        for (int i = 0; i < selectedStore->itemCount; i++) {
            if (strcmp(selectedStore->items[i].name, cart[editChoice].itemName) == 0) {
                itemNumber = i;
                break;
            }
        }

        selectedStore->items[itemNumber].stock += cart[editChoice].quantity;  // Restock the original quantity

        if (newQuantity == 0) {
            for (int i = editChoice; i < cartCount - 1; i++) {
                cart[i] = cart[i + 1];
            }
            cartCount--;
        } else {
            if (newQuantity > selectedStore->items[itemNumber].stock) {
                printf("Error: Quantity exceeds stock! Available stock: %d\n", selectedStore->items[itemNumber].stock);
                continue;
            }
            cart[editChoice].quantity = newQuantity;
            selectedStore->items[itemNumber].stock -= newQuantity;  // Update with the new quantity
        }

        displayCart(cart, cartCount);
        editCart = getValidatedInput("Do you want to edit more items? (y/n): ");
    }

    generateBill(cart, cartCount, selectedStore);

    writeFile("Groce.txt", stores, storeCount);  // Save the updated stock to the file

    return 0;
}
#endif