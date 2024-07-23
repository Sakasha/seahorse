#include <stdio.h>
#include <conio.h>
#include <unistd.h>
#include "restaurant.h"
#include "utility.h"
#include "apriori.h"
#include "track.h"
#include "dijkstra.h"

#ifndef DISPLAY_HELPER_H
#define DISPLAY_HELPER_H

/*
    A set of helper methods to manage the display of menus

*/
#define MAX_ALLERGENS 10

//---------------------------------------------------------------------------------//
// ------------------- Method Definitions---------- -------------------------------//
//---------------------------------------------------------------------------------//
int printAndCollectLoginOrRegistrationChoice();
int collectUsername(char *username);
int collectUserPassword(char *password);
int printAndCollectUserCredentials(char *username);
int printAndCollectRatingOrOrderingChoice();
int printAndCollectPetOrFoodOrGroceryChoice();
void collectItemOrders(Restaurant *restaurant, OrderItems *items, int *itemCount, Itemset *itemsets, Order *historyOrders, int orderCount);
Restaurant *filterSort(Restaurant *restaurants, int count, char *userLocation);
int collectAndUpdateRestaurantRatingBasedOnPreviousOrder(char *phoneNumber);
void displayUserOrderHistory(int count, Order *orders);
int displayAndCollectAllergens(char **allergens);
Item *filterItemsBasedOnAllergens(Item *itemList, int itemCount, char **allergens, int allergenCount, int *filteredCount);
//---------------------------------------------------------------------------------//
// ------------------- Method Implementation Starts -------------------------------//
//---------------------------------------------------------------------------------//

//
//  Method to collect user choice of login or registrations
//
int printAndCollectLoginOrRegistrationChoice()
{
    int choice = -1;
    while (1)
    {
        printf("\nPlease select one of the following:\n");
        printf("1. To login\n");
        printf("2. To register as new user\n");
        printf("9. To exit\n");
        printf(":");
        scanf("%d", &choice);
        fflush(stdin);

        if ((choice > 0 && choice < 3) || (choice == 9))
        {
            break;
        }
        else
        {
            printf("\n\n");
            printf("Invalid Choice.  Please try again!\n\n");
            choice = -1;
        }
    }

    return choice;
}

//
//  Method to collect user name with required validations.  Use for valdiating credentials
//
int collectUsername(char *username)
{
    int choice = 0;

    while (1)
    {
        printf("Enter username (Phone #): ");
        scanf("%s", username);
        printf("\n ");

        if (isValidUsername(username) != 0)
        {
            printf("Invalid username. It must be exactly 10 digits.\n");
            continue;
        }

        if (!isUsernameRegistered(username))
        {
            while (1)
            {
                printf("Username not registered. Please register first.\n\n");
                printf("Please enter your choice.\n");
                printf("1. To input username again\n");
                printf("8. To go back to main menu\n");
                printf(":");
                scanf("%d", &choice);
                fflush(stdin);
                printf("\n ");

                if ((choice == 1) || (choice == 8))
                {
                    break;
                }
                else
                {
                    printf("Invalid choice.  Try again!\n\n");
                }
            }
        }
        else
        {
            choice = 0;
            break;
        }

        if (choice == 8)
        {
            break;
        }
    }
    return choice;
}

//
/// @brief
/// @param password
//
void myGetPass(char *password)
{
    int i = 0;
    char ch;

    printf("###Enter your password (max %d characters): ", MAX_PASSWORD_LENGTH - 1);

    while (1)
    {
        ch = _getch();

        if (ch == 13) // Enter key
            break;
        if (ch == 8) // Backspace
        {
            if (i > 0)
            {
                printf("\b \b"); // erase character from console
                i--;
            }
        }
        else if (i < MAX_PASSWORD_LENGTH - 1)
        {
            password[i++] = ch;
            printf("*"); // print asterisk instead of the actual character
        }
    }

    password[i] = '\0'; // Null-terminate the password string
    printf("\n");
}

//
//  Method to collect user password with required validations.  Use for valdiating credentials
//
int collectUserPassword(char *password)
{
    int choice = 0;

    while (1)
    {

        myGetPass(password);

        if (isValidPassword(password) != 0)
        {
            printf("Invalid password. It must be at most 20 characters long.\n");

            while (1)
            {
                printf("Please enter your choice.\n");
                printf("1. To input password again\n");
                printf("8. To go back to main menu\n");
                printf(":");
                scanf("%d", &choice);
                fflush(stdin);
                printf("\n ");

                if ((choice == 1) || (choice == 8))
                {
                    break;
                }
                else
                {
                    printf("Invalid choice.  Try again!\n\n");
                }
            }

            if (choice == 8)
            {
                break;
            }
        }
        else
        {
            choice = 0;
            break;
        }
    }
    return choice;
}

//
//  Method to collect user name and password and validate the same
//
int printAndCollectUserCredentials(char *username)
{
    int choice = 0;
    char password[MAX_PASSWORD_LENGTH];

    while (1)
    {
        choice = collectUsername(username);

        if (choice == 8)
        {
            break;
        }

        choice = collectUserPassword(password);
        if (choice == 8)
        {
            break;
        }

        if ((validateCredentials(username, password) == 0))
        {
            printCentered("\n***Login successful!****\n\n");
            choice = 0;
            break;
        }
        else
        {
            while (1)
            {
                printf("Invalid username or password.  Please try again.\n\n");
                printf("Please enter your choice.\n");
                printf("1. To input username/passowrd again\n");
                printf("8. To go back to main menu\n");
                printf(":");
                scanf("%d", &choice);
                fflush(stdin);
                printf("\n ");

                if ((choice == 1) || (choice == 8))
                {
                    break;
                }
                else
                {
                    printf("Invalid choice.  Try again!\n\n");
                }
            }

            if (choice == 8)
            {
                break;
            }
        }
    }

    return choice;
}

int printAndCollectRatingOrOrderingChoice()
{
    int choice = 0;

    while (1)
    {
        printf("Please enter your choice.\n");
        printf("1. To update rating for previous order\n");
        printf("2. To place new order\n");
        printf("8. To go back to main menu\n");
        printf(":");
        scanf("%d", &choice);
        fflush(stdin);
        printf("\n ");

        if ((choice == 1) || (choice == 2) || (choice == 8))
        {
            break;
        }
        else
        {
            printf("Invalid choice.  Try again!\n\n");
        }
    }
}

int printAndCollectPetOrFoodOrGroceryChoice()
{
    int choice = 0;

    while (1)
    {
        printf("Please enter your choice.\n");
        printf("1. To place pet related orders\n");
        printf("2. To place food related orders\n");
        printf("3. To place grocery related orders\n");
        printf("8. To go back to main menu\n");
        printf(":");
        scanf("%d", &choice);
        fflush(stdin);
        printf("\n ");

        if ((choice == 1) || (choice == 2) || (choice == 3) || (choice == 8))
        {
            break;
        }
        else
        {
            printf("Invalid choice.  Try again!\n\n");
        }
    }
}

int collectFoodOrders(char *phoneNumber)
{

    // declarationscls

    Order order;
    Customer customer;
    Restaurant *restaurants = NULL; // Declare a pointer to hold restaurant list
    OrderItems items[MAX_ITEMS];
    int itemCount, count, selectedRestaurant = -1;

    // generate order id, this will be usedlater while saving the order in file
    generateID((order.ID), ORDER_ID_SIZE, 'O');

    // Search the customer db to fetch the details.
    // If not found, register the customer and save the same in customer db
    if (searchCustomerByPhoneNumber(phoneNumber, &customer) != 0)
    {
        printf("\nLooks like you are not registered, let us register you quickly.\n\n");
        return 8;
    }

    order.customer = customer;

    // Fetch the restaurant list, along with number of restaurants in the list
    // going forward we will use the list of restaurant held in memory
    // Not the best approach, Need to change later
    count = readRestaurants(&restaurants);

    // Sort the restaurants
    sortRestaurants(restaurants, count, compareByRating);

    // -------------------- Use the following code to integrate ----------------------------
    //
    // In the next few lines Read the order history, and
    // run the apriori algorithm to build the reference table
    // which we will use for fetchning the recommendations
    Order *historyOrders;

    // read the order history from the orders db
    int orderCount = readOrders(&historyOrders);

    Transaction transactions[MAX_TRANSACTIONS];
    Itemset itemsets[MAX_ITEMS_PER_TRANSACTION * MAX_ITEMS_PER_TRANSACTION];
    char topItem[100];
    char mostOrderItem[50];

    // Convert the order transaction history in to a format that is easy
    // for us to handle in apriori algorithm
    generateDataset(transactions, orderCount, historyOrders);

    // Run the algorith and build the reference table with itemsets
    apriori(transactions, orderCount, itemsets);

    // Debug to print the apriori reference table
    int x = 0;
    while (1)
    {
        if (itemsets[x].itemCount <= 0)
        {
            break;
        }
        printItemset(itemsets[x]);
        x++;
    }
    // Get the top order item.
    getTopItem(itemsets, topItem, NULL, 1);

    printf("\n\n###########################################################.\n\n");
    printf("\n **** We recommend the following resturants which are most rated. ****\n\n");

    // Display the recommended restuarant based on rating
    displayRestaurants(restaurants, 1, customer.addresses[0].city);

    // printf("\n We recommend %s for you today\n", &mostOrderItem);

    // Display the top item based on apriori reference table build earlier
    if (strlen(topItem) != 0)
    {
        printf("\n *** We recommend %s for you today\n", topItem);
    }
    else if (strlen(mostOrderItem) != 0)
    {
        printf("\n We recommend %s for you today\n", mostOrderItem);
    }

    printf("\n###########################################################.\n\n");

    // Display it to user for selection
    // displayRestaurants(restaurants, count, order.customer.addresses->city);
    displayRestaurants(restaurants, count, order.customer.addresses->city);

    // Allow user to filter, sort, or select
    restaurants = filterSort(restaurants, count, order.customer.addresses->city);

    // Collect user choice, if not a valid choice, make them select again
    while (1)
    {
        printf("\nSelect the restaurant (enter the 'SNo' to select the resturant): ");
        scanf("%d", &selectedRestaurant);
        fflush(stdin);

        if (selectedRestaurant > count)
        {
            printf("\nInvalid Choice.  Try again...");
        }
        else
        {
            break;
        }
    }

    // Save the resturant details in the order
    order.restaurant = restaurants[(selectedRestaurant - 1)];

    // Display confirmation message on the restaurant selection
    printf("\nYour Select is : %s.  Good Choice...\n", restaurants[(selectedRestaurant - 1)].name);

    // -------------------------------  Integration point ---------------------------- //
    // For a given restaurant fetch the most ordered item
    char mostOrderItemInRestaurant[50];
    findMostOrderedItemForRestaurant(historyOrders, orderCount, restaurants[(selectedRestaurant - 1)].ID, mostOrderItemInRestaurant);

    if (strlen(mostOrderItemInRestaurant) != 0)
    {
        printf("\n\n###########################################################.\n\n");
        printf("\n **** We recommend the following item in %s . ****\n\n", restaurants[(selectedRestaurant - 1)].name);

        printf("\n We recommend %s for you today\n", mostOrderItemInRestaurant);

        printf("\n###########################################################.\n\n");
    }

    // Call the function to collect user item selections
    collectItemOrders(&restaurants[(selectedRestaurant - 1)], order.items, &itemCount, itemsets, historyOrders, orderCount);

    // Display order summary
    displayOrderSummary(&order);

    char placeOrder = 'n';

    while (1)
    {
        printf("\nProceed to order placement? (y/n):");
        scanf(" %c", &placeOrder);
        fflush(stdin);

        if (placeOrder == 'y')
        {
            // Save order in file
            saveOrder(&order);
            printf("\n\n *****  Items Ordered.  Thanks for your order  ****\n\n");
            printf("\n############ Track your Order #################\n");
            trackOrder(&restaurants[(selectedRestaurant - 1)].name, &restaurants[(selectedRestaurant - 1)].location, customer.addresses[0].city, order.ID);

            break;
        }
        else
        {
            printf("\n ***** Cancelling the order.  Hope to see you soon. ***** \n");
            break;
        }
    }

    return 0;
}

//
// Utlity method to display and filter items based on allergens
//

//
// Display menu to filter and sort
//
Restaurant *filterSort(Restaurant *restaurants, int count, char *userLocation)
{
    int optSelect, choice, exiting = 1;
    Restaurant *returnRestaurants;
    returnRestaurants = restaurants;
    while (exiting)
    {

        printf("\nwhat would you like to do\n[1] %20s\n[2] %20s\n[3] %20s\n", "Sort Restaurants", "Filter Restaurants", "Select Restaurants");
        scanf("%d", &optSelect);

        switch (optSelect)
        {
        case 1:
        {
            // assignDistance(restaurants, count, userLocation);

            printf("Choose sort criteria:\n[1] Rating (Highest to Lowest)\n[2] Distance (Lowest to Highest)\n");
            scanf("%d", &choice);

            switch (choice)
            {
            case 1:
                sortRestaurants(restaurants, count, compareByRating);
                returnRestaurants = restaurants;
                break;
            case 2:
                // ort the array of restaurants based on distance from the user location
                sortRestaurants(restaurants, count, compareByDistance);
                returnRestaurants = restaurants;
                break;
            }
            displayRestaurants(restaurants, count, userLocation);
            break;
        }

        case 2:
        {
            printf("\nChoose filter criteria:\n[1] Filter by Veg/Non-Veg\n");
            //printf("\nChoose filter criteria:\n[1] Filter by Veg/Non-Veg\n[2]Filter by Cuisine\n");
            scanf("%d", &choice);

            switch (choice)
            {
            case 1:
            {
                bool isVeg;
                int filteredCount;
                printf("[1] Vegetarian Restaurants\n[0] NonVegetarian Restaurants\n");
                scanf("%d", &isVeg);
                returnRestaurants = filterRestaurantsByVeg(restaurants, count, isVeg, &filteredCount);
                displayRestaurants(returnRestaurants, filteredCount, userLocation);
                break;
            }
            // case 2:
            // {
            //     char cuisine[20];
            //     printf("Enter the cuisine to filter by: ");
            //     scanf("%s", cuisine);
            //     int filteredCount;
            //     returnRestaurants = filterRestaurantsByCuisine(restaurants, count, cuisine, &filteredCount);
            //     displayRestaurants(returnRestaurants, filteredCount, userLocation);
            //     break;
            // }
            }
            break;
        }

        case 3:
        {
            exiting = 0;

            break;
        }
        default:
            printf("Invalid choice.\n");
            break;
        }
    }

    return returnRestaurants;
}
//
// Display order history
void displayUserOrderHistory(int count, Order *orders)
{
    printf("---------------------------------------------------------------------------------------------------\n");
    printf("| %-5s| %-40s | %-20s | %-30s | %-5s |\n", "SNo", "Restaurant", "Order id", "Item Name", "Quantity");
    printf("---------------------------------------------------------------------------------------------------\n");

    for (int i = 0; i < count; i++)
    {

        if (orders[i].ID[0] != 'O')
        {
            continue;
        }
        printf("%-5d %-40s %-20s\n", (i + 1), orders[i].restaurant.name, orders[i].ID);
        for (int j = 0; j < MAX_ORDER_ITEMS; j++)
        {
            if (orders[i].items[j].item.ID[0] == 'I' && orders[i].items[j].quantity > 0)
            {
                printf("%-85s %-30s   %-5d\n", "", orders[i].items[j].item.name, orders[i].items[j].quantity);
            }
        }

        printf("\n");
    }
}
//
//
// Collect and update rating of a restaurant
//
int collectAndUpdateRestaurantRatingBasedOnPreviousOrder(char *phoneNumber)
{
    int count = -1;
    int choice = -1;
    float rating = 0.0;
    Order orders[MAX_ORDERS];
    Restaurant restaurant;

    count = searchOrderByPhoneNumber(phoneNumber, &orders);

    // for (int i = 0; i < count; i++)
    // {
    //     printf("filtered: restaurant name %s, orders[i].customer.phoneNumber: %s, Orders ID: %s\n", orders[i].restaurant.name, orders[i].customer.phoneNumber, orders[i].ID);
    // }

    // printf(" order count %d\n", count);

    if (count < 0)
    {
        printf(" NO previous order available for user id %s\n", phoneNumber);
        return 8;
    }

    while (1)
    {
        printf(" Select the order you want to rate  from the following list:\n");

        displayUserOrderHistory(count, orders);
        scanf("%d", &choice);
        fflush(stdin);

        if (choice > count)
        {
            printf("Invalid choice for order selection.  Please try again!\n");
            continue;
        }
        else
        {
            break;
        }
    }

    int found = getRestaurant(orders[choice - 1].restaurant.ID, &restaurant);

    if (found == 1)
    {

        while (1)
        {
            printf("Enter your rating (1-5) - with 1 being worst and 5 being best: \n");
            scanf("%f", &rating);
            fflush(stdin);

            if (rating < 0 || rating > 5)
            {
                printf("Invalid choice for rating.  Allowed values are between 1 and 5 only.  Please try again!\n");
                continue;
            }
            else
            {
                break;
            }
        }

        restaurant.rating = rating;

        printf(" %s, %s, %f", restaurant.ID, restaurant.name, restaurant.rating);

        removeRestaurant(restaurant.ID);

        if (addRestaurant(&restaurant) == 0)
        {
            printf("rating update success \n");
        }
        else
        {
            printf("Error in updating rating \n");
            return -1;
        }
    }
    return 0;
}

//
//  Method to collects  orders of items in loop
//
void collectItemOrders(Restaurant *restaurant, OrderItems *items, int *itemCount, Itemset *itemsets, Order *historyOrders, int orderCount)
{

    // Declarations
    *itemCount = 0;
    char choice = 'y';
    int count, selectedItem = -1, optSelect = -1;
    // Item *orignalItems = restaurant->items;

    Item *orignalItems = (Item *)malloc(MAX_ITEMS * sizeof(Item));
    if (!orignalItems)
    {
        printf("Memory allocation error.\n");
        return NULL;
    }

    memcpy(orignalItems, restaurant->items, (MAX_ITEMS * sizeof(Item)));

    // DEBUG Statement
    // printf("\n #### Value of Choice : %c\n", choice);

    while (choice == 'y')
    {
        // If max allowed count is reached, exist the loop with message
        if (*itemCount >= MAX_ITEMS)
        {
            printf("Maximum item limit reached.\n");
            break;
        }

        // Display the items in a table
        count = displayRestaurantItems(restaurant);
        //----------------------------------------------------------------
        // Collect options to sort items or select items

        while (1)
        {
            printf("\nwhat would you like to do\n[1] %20s\n[2] %20s\n[3] %s\n[4] %s\n[5] %s\n:", "Sort by price (low to high)", "Sort by price (high to low)", "Filter by Allergens", "Remove Filter by Allergens", "Select items");
            scanf("%d", &optSelect);
            fflush(stdin);

            if (optSelect < 0 || optSelect > 5)
            {
                printf("\nInvalid Choice.  Try again...");
            }
            else
            {
                if (optSelect == 1 || optSelect == 2)
                {
                    sortItems(restaurant->items, count, optSelect);
                    count = displayRestaurantItems(restaurant);
                }
                else if (optSelect == 3)
                {

                    char allergens[MAX_ALLERGENS][3]; // Allow up to 10 allergens
                    int allergenCount = displayAndCollectAllergens(allergens);
                    // printf("### AlLergen count : %d\n", allergenCount);

                    int filteredCount = 0;
                    Item *filteredItems = filterItemsBasedOnAllergens(restaurant->items, count, allergens, allergenCount, &filteredCount);
                    memcpy(restaurant->items, filteredItems, (MAX_ITEMS * sizeof(Item)));
                    count = displayRestaurantItems(restaurant);
                }
                else if (optSelect == 4)
                {
                    memcpy(restaurant->items, orignalItems, (MAX_ITEMS * sizeof(Item)));
                    count = displayRestaurantItems(restaurant);
                }
                else
                {
                    break;
                }
            }
        }

        //----------------------------------------------------------------
        // Collect the user choice as S.No
        // Ensure that there is no invalid entry
        while (1)
        {
            printf("\nSelect the item (enter the 'SNo' to select the item): ");
            scanf("%d", &selectedItem);
            fflush(stdin);

            if (selectedItem > count)
            {
                printf("\nInvalid Choice.  Try again...");
            }
            else
            {
                break;
            }
        }

        // Check if available, if not throw error and continue
        int availability = restaurant->items[(selectedItem - 1)].availability ? 1 : 0;

        // Check if the item is available, if not throw a message and move on to for next item
        if (!availability)
        {
            printf("Item %s is not available.\n", restaurant->items[(selectedItem - 1)].name);
        }
        // If item is available them collect other details
        else
        {
            // -------------------------------  Integration point ---------------------------- //
            char mostOrderItem[50] = {'\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0'};
            findMostCommonCompanionItem(historyOrders, orderCount, restaurant->ID, restaurant->items[(selectedItem - 1)].name, mostOrderItem);

            // -------------------------------  Integration point ---------------------------- //

            char companion[100] = {'\0'};
            getPair(itemsets[0].items[0].item, itemsets, companion, orderCount, (itemCount + 2));

            if (strlen(companion) > 0 && companion[0] != NULL && companion[0] != '\0')
            {
                printf("\n\n###########################################################.\n\n");
                printf("\n **** We recommend the following item in %s  to go along with %s. ****\n\n", companion, restaurant->items[(selectedItem - 1)].name);
                printf("\n###########################################################.\n\n");
            }
            else if (strlen(mostOrderItem) != 0 && companion[0] != NULL && companion[0] != '\0')
            {
                printf("\n\n###########################################################.\n\n");
                printf("\n **** We recommend the following item in %s  to go along with %s. ****\n\n", mostOrderItem, restaurant->items[(selectedItem - 1)].name);
                printf("\n###########################################################.\n\n");
            }

            // printf("\n We recommend %s for you today\n", mostOrderItem);

            items[*itemCount].item = restaurant->items[(selectedItem - 1)];

            printf("Enter quantity: ");
            scanf("%d", &items[*itemCount].quantity);
            fflush(stdin);

            items[*itemCount].totalPrice = restaurant->items[(selectedItem - 1)].price * items[*itemCount].quantity;
            (*itemCount)++;
        }

        choice = 'x';

        // Collect user choice to continue or not
        printf("\n\n Do you want to add another item? (y/n): ");
        scanf(" %c", &choice);
        fflush(stdin);

        // printf("\n #### Value of Choice : %c\n", choice);
    } // End of while loop to collect orders

    // DEBUG Statement
    // printf("\n ############   Exiting the get item orders loop .\n");
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///------------------------------------------OTHERS CODE FOR INTEGRATION ----------------------------------------///
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int displayAndCollectAllergens(char **allergens)
{

    int choice = -1, allergenCount = 0;

    while (1)
    {
        printf("Enter allergen to avoid: \n[1]%20s\n[2]%20s\n[3]%20s\n[4]%20s\n", "Milk", "Egg", "Peanut", "Done");
        scanf("%d", &choice);
        fflush(stdin);

        if (choice < 0 || choice > 4)
        {
            printf("Invalid choice.  Try again\n");
            continue;
        }
        else
        {
            if (choice == 1)
            {
                if (isValidAllergenEntry(allergens, allergenCount, "Milk") == 0)
                {
                    allergens[allergenCount] = "Milk";
                    allergenCount++;
                }
                else
                {
                    printf(" %s already in the avoid allegen list\n", "Milk");
                }
            }
            else if (choice == 2)
            {
                if (isValidAllergenEntry(allergens, allergenCount, "Egg") == 0)
                {
                    allergens[allergenCount] = "Egg";
                    allergenCount++;
                }
                else
                {
                    printf(" %s already in the avoid allegen list\n", "Egg");
                }
            }
            else if (choice == 3)
            {
                if (isValidAllergenEntry(allergens, allergenCount, "Peanut") == 0)
                {
                    allergens[allergenCount] = "Peanut";
                    allergenCount++;
                }
                else
                {
                    printf(" %s already in the avoid allegen list\n", "Peanut");
                }
            }
            else
            {
                break;
            }
        }
    }

    return allergenCount;
}

int isValidAllergenEntry(char **allergen, int count, char *choice)
{
    int retVal = 0;

    for (int i = 0; i < count; i++)
    {
        if ((strcmp(allergen[i], choice) == 0))
        {
            retVal = -1;
            break;
        }
    }

    return retVal;
}

Item *filterItemsBasedOnAllergens(Item *itemList, int itemCount, char **allergens, int allergenCount, int *filteredCount)
{
    // printf("\n### Item count %d\n", itemCount);
    // for (int x = 0; x < itemCount; x++)
    // {
    //     printf("### item[%d].name : %s \n", x, itemList[x].name);
    // }

    Item *filteredItems = (Item *)malloc(MAX_ITEMS * sizeof(Item));
    if (!filteredItems)
    {
        printf("Memory allocation error.\n");
        *filteredCount = 0;
        return NULL;
    }
    for (int i = 0; i < itemCount; i++)
    {

        if (isAllergensPresent(itemList[i], allergens, allergenCount) == 0)
        {
            (filteredItems[*filteredCount]) = itemList[i];
            (*filteredCount)++;
        }
    }

    return filteredItems;
}

int isAllergensPresent(Item *item, char **allergens, int allergenCount)
{
    int retVal = 0;
    // printf("### item.name : %s, item.allergen : %s,  allergens: %s\n", item->name, item->allergen, allergens[0]);

    for (int i = 0; i < allergenCount; i++)
    {
        // printf("## item->allergen : %s, allergens : %s, result  %d\n", item->allergen, allergens[i], strcmp(item->allergen, allergens[i]));
        if (strcmp(item->allergen, allergens[i]) == 0)
        {
            retVal = -1;
            break;
        }
    }

    return retVal;
}
#endif