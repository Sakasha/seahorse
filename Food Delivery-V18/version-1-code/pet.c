#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define MAX_ITEMS 5
#define MAX_SHOPS 10
#define MAX_NAME_LENGTH 50
#define DELIVERY_SPEED 40.0  // in km/hr

typedef struct {
    char name[MAX_NAME_LENGTH];
    float cost;
    int stock;  // Keeping this for internal use, but it won't be displayed
} Item;

typedef struct {
    char shopName[MAX_NAME_LENGTH];
    char location[MAX_NAME_LENGTH];
    Item items[MAX_ITEMS];
} Shop;

// Define distances for locations
typedef struct {
    char location[MAX_NAME_LENGTH];
    float distance;  // in km
} LocationDistance;

LocationDistance locationDistances[] = {
    {"Anna Nagar", 4.0},
    {"Sholinganallur", 6.0},
    {"Adyar", 5.0},
    {"Kalavakkam", 7.0},
    {"Neelankarai", 5.0}
};

void toLowerCase(char *str) {
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

    float distance = getDistance(location);
    if (distance < 0) {
        printf("Location not found.\n");
        return;
    }

    float deliveryTime = (2 * distance) / DELIVERY_SPEED;  // To pick up and deliver

    printf("Shops and items in location: %s\n", location);
    printf("=========================================\n");
    printf("Estimated delivery time: %.2f hours\n", deliveryTime);
    printf("=========================================\n");

    for (int i = 0; i < numShops; i++) {
        char shopLocation[MAX_NAME_LENGTH];
        strcpy(shopLocation, shops[i].location);
        toLowerCase(shopLocation);

        if (strcmp(shopLocation, lowerLocation) == 0) {
            printf("Shop: %s\n", shops[i].shopName);
            printf("-------------------------------------------------------\n");
            printf("| %-30s | %-15s |\n", "Item Name", "Cost (₹)");
            printf("-------------------------------------------------------\n");

            for (int j = 0; j < MAX_ITEMS; j++) {
                if (strlen(shops[i].items[j].name) > 0) {
                    printf("| %-30s | ₹%-13.2f |\n", shops[i].items[j].name, shops[i].items[j].cost);
                }
            }
            printf("-------------------------------------------------------\n");
        }
    }
}

void displayItemsByAnimal(Shop shops[], int numShops, const char *location, const char *animal) {
    char lowerLocation[MAX_NAME_LENGTH];
    strcpy(lowerLocation, location);
    toLowerCase(lowerLocation);

    float distance = getDistance(location);
    if (distance < 0) {
        printf("Location not found.\n");
        return;
    }

    float deliveryTime = (2 * distance) / DELIVERY_SPEED;  // To pick up and deliver

    char animalSuffix[4];  // For storing the animal suffix like "(C)"
    snprintf(animalSuffix, sizeof(animalSuffix), "(%c)", toupper(animal[0]));

    printf("Items for animal: %s\n", animal);
    printf("=========================================\n");
    printf("Estimated delivery time: %.2f hours\n", deliveryTime);
    printf("=========================================\n");

    for (int i = 0; i < numShops; i++) {
        char shopLocation[MAX_NAME_LENGTH];
        strcpy(shopLocation, shops[i].location);
        toLowerCase(shopLocation);

        if (strcmp(shopLocation, lowerLocation) == 0) {
            printf("Shop: %s\n", shops[i].shopName);
            printf("-------------------------------------------------------\n");
            printf("| %-30s | %-15s |\n", "Item Name", "Cost (₹)");
            printf("-------------------------------------------------------\n");

            for (int j = 0; j < MAX_ITEMS; j++) {
                if (strstr(shops[i].items[j].name, animalSuffix) != NULL) {
                    printf("| %-30s | ₹%-13.2f |\n", shops[i].items[j].name, shops[i].items[j].cost);
                }
            }
            printf("-------------------------------------------------------\n");
        }
    }
}

int main() {
    Shop shops[MAX_SHOPS];
    int numShops;
    char userLocation[MAX_NAME_LENGTH];
    char animalType[MAX_NAME_LENGTH];

    readDataFromFile("PetFood.txt", shops, &numShops);

    printf("Enter your location: ");
    scanf(" %[^\n]", userLocation);  // Use %[^\n] to read the entire line including spaces
    displayShopsByLocation(shops, numShops, userLocation);

    printf("Enter animal type (Fish, Hamster, Dog, Cat, Bird): ");
    scanf("%s", animalType);
    toLowerCase(animalType);
    displayItemsByAnimal(shops, numShops, userLocation, animalType);

    return 0;
}
