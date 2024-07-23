#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include "customer.h"
#include "restaurant.h"
#include "utility.h"

#ifndef ORDER_H
#define ORDER_H
/*
    This file handles all order related functions
*/

#define MAX_ORDERS 100
#define MAX_ORDER_ITEMS 10
#define ORDER_ID_SIZE 15
#define ORDER_FILENAME "./data/ordersv2.dat"
//changed the order file from orders to ordersv2

//---------------------------------------------------------------------------------//
// ------------------- Structure Definitions---------------------------------------//
//---------------------------------------------------------------------------------//
// Holds individual  item order data
typedef struct
{
    Item item;
    int quantity;
    float totalPrice;
} OrderItems;

// Holds the total order details
typedef struct
{
    char ID[15];
    Customer customer;
    char phoneNumber[10];
    Restaurant restaurant;
    OrderItems items[MAX_ORDER_ITEMS];
} Order;

//---------------------------------------------------------------------------------//
// ------------------- Method Definitions---------- -------------------------------//
//---------------------------------------------------------------------------------//
int saveOrder(const Order *order);
int readOrders(Order **orders);
int searchOrderByPhoneNumber(const char *phoneNumber, Order *orders);
void printOrder(const Order *order);
void displayOrderSummary(const Order *order);

//---------------------------------------------------------------------------------//
// ------------------- Method Implementation Starts -------------------------------//
//---------------------------------------------------------------------------------//

//
// Saves orders into file
//
int saveOrder(const Order *order)
{
    int retValue = -1;
    // printOrder(order);

    FILE *file = fopen(ORDER_FILENAME, "ab"); // open file in append mode
    if (file != NULL)
    {
        fwrite(order, sizeof(Order), 1, file); // write restaurant to file
        fclose(file);
        retValue = 0;
    }
    else
    {
        printf("Error opening file.\n");
    }

    return retValue;
}

//
// Function to read all order records from a file
//
int readOrders(Order **orders)
{
    FILE *file = fopen(ORDER_FILENAME, "rb");
    if (!file)
    {
        printf("Error opening file.\n");
        return -1;
    }

    // Allocate memory for the restaurants
    *orders = malloc(MAX_ORDERS * sizeof(Order));
    if (*orders == NULL)
    {
        printf("Memory allocation failed.\n");
        fclose(file);
        return -1;
    }

    int count = 0;
    while (fread(&(*orders)[count], sizeof(Order), 1, file) == 1)
    {
        count++;
        if (count >= MAX_ORDERS)
        {
            printf("Reached maximum number of restaurants that can be read.\n");
            break;
        }
    }

    fclose(file);
    return count;
}

//
// Search previous orders with phone number
//

int searchOrderByPhoneNumber(const char *phoneNumber, Order *orders)
{
    Order *resultOrders;
    int orderCount = readOrders(&resultOrders);
    int count = 0;

    if (orderCount == 0)
    {
        printf("Failed to read orders from file.\n");
        return 0;
    }

     for (int i = 0; i < orderCount; i++)
    {
        // printf("restaurant name %s, resultOrders[i].customer.phoneNumber: %s, Search Number: %s\n", resultOrders[i].restaurant.name, resultOrders[i].customer.phoneNumber, phoneNumber);
        if (strcmp(resultOrders[i].customer.phoneNumber, phoneNumber) == 0)
        {
            // printf("Phone number matches.\n");
            orders[count] = resultOrders[i];
            // printf("Assigned pointers.\n");
            // (*orders)[count] = resultOrders[i];
            count++;
        }
    }

    // printf("free memory.\n");
    // free(resultOrders);


    return count;
}

//
// Function to print an order
//
void printOrder(const Order *order)
{

    printf("  Order ID: %s\n", &order->ID);
    printf("  Customer Name: %s\n", &order->customer.name);
    printf("  Customer Phone: %s\n", &order->customer.phoneNumber);
    printf("  Restaurant Name: %s\n", &order->restaurant.name);

    printf("Order Items:\n");
    for (int i = 0; i < MAX_ORDER_ITEMS; i++)
    {
        if (order->items[i].item.ID[0] != 'I')
        {
            break;
        }

        if (order->items[i].quantity > 0)
        {
            printf("  Item Name: %s - ", order->items[i].item.name);
            printf("  Item Price: %.2f - ", order->items[i].item.price);
            printf("  Quantity: %d - ", order->items[i].quantity);
            printf("  Total Price: %.2f - ", order->items[i].totalPrice);
            printf("\n");
        }
    }
}

//
// Display order summary
//
void displayOrderSummary(const Order *order)
{

    if (!order)
    {
        printf("Order not found.\n");
        return;
    }

    printRed("--------------------------------------------------------------------------------------------------------\n");
    printRed("                                              ORDER DETAILS \n");
    printRed("--------------------------------------------------------------------------------------------------------\n\n");
    printf(" Customer Phone Number : %s                                         Customer Name: %s \n", order->customer.phoneNumber, order->customer.name);
    printf(" Restaurant Id : %s                                              Restuaurant Name: %s \n", order->restaurant.ID, order->restaurant.name);
    printf(" Order Id : %s                                                   \n\n\n", order->ID);

    printf("----------------------------------------------------------------------------------------------------------------------------\n");
    printf("| %-5s| %-7s | %-40s | %-20s | %-12s | %-6s | %-10s |\n", "SNo", "ID", "Item Name", "Allergen", "Price", "Quantity", "Item Cost");
    printf("----------------------------------------------------------------------------------------------------------------------------\n");
    int i = 0;
    float totalPrice = 0.0f;
    while (strlen(order->items[i].item.ID) > 0)
    {

        if (order->items[i].item.ID[0] != 'I')
        {
            break;
        }
        printf("| %-5d| %-7s | %-40s | %-20s | %-12f | %-6d | %-10f | \n",
               (i + 1),
               order->items[i].item.ID,
               order->items[i].item.name,
               strlen(order->items[i].item.allergen) > 0 ? order->items[i].item.allergen : "N/A",
               order->items[i].item.price,
               order->items[i].quantity,
               order->items[i].totalPrice);
        totalPrice += order->items[i].totalPrice;
        i++;
    }
    printf("--------------------------------------------------------------------------------------------------------\n");
    printf(" *** Total Order Amount :");
    printf("%.2f\n\n", totalPrice);
}
#endif