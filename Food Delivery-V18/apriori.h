#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "restaurant.h"
#include "utility.h"
#include "order.h"

#ifndef APRIORI_H
#define APRIORI_H

/*
    This file handles apriori algorithm for identifying the recommendated items
*/

#define MAX_TRANSACTIONS 100
#define MAX_ITEMS_PER_TRANSACTION 10
#define MAX_ITEM_LENGTH 50
#define MIN_SUPPORT 4

//---------------------------------------------------------------------------------//
// ------------------- Structure Definitions---------------------------------------//
//---------------------------------------------------------------------------------//

// Define an Item struct to hold item details
typedef struct
{
    char item[MAX_ITEM_LENGTH];
} ItemA;

// Define a Transaction struct to hold items in a transaction
typedef struct
{
    ItemA items[MAX_ITEMS_PER_TRANSACTION];
    int itemCount;
} Transaction;

// Define an Itemset struct to hold items in an itemset and its support count
typedef struct
{
    ItemA items[MAX_ITEMS_PER_TRANSACTION];
    int itemCount;
    int supportCount;
} Itemset;

//---------------------------------------------------------------------------------//
// ------------------- Method Definitions---------- -------------------------------//
//---------------------------------------------------------------------------------//
void gettemsAcrossRestaurantOrders(Order *orders, int orderCount, char *mostOrderItem);
void findMostOrderedItemForRestaurant(Order *orders, int orderCount, const char *restaurantID, char *mostOrderItem);
void findMostCommonCompanionItem(Order *orders, int orderCount, const char *restaurantID, const char *itemName, char *mostOrderItem);
void getPair(char *item, Itemset *itemsets, char *companion, int transactionCount, int depth);
void generateDataset(Transaction *transactions, int transactionCount, Order *orders);

int isSubset(Itemset subset, Transaction transaction);
void countSupport(Itemset *itemsets, int itemsetCount, Transaction *transactions, int transactionCount);
int canJoin(Itemset a, Itemset b);
Itemset joinItemsets(Itemset a, Itemset b);
void generateCandidates(Itemset *itemsets, int itemsetCount, Itemset *candidates, int *candidateCount);
void pruneCandidates(Itemset *candidates, int *candidateCount);
void printItemset(Itemset itemset);
void apriori(Transaction *transactions, int transactionCount, Itemset *itemsets);
void printTransactions(Transaction *transactions, int transactionCount);
void getTopItem(const Itemset *inputItemsets, char *itemName, ItemA *companianItems, int level);

//---------------------------------------------------------------------------------//
// ------------------- Method Implementation Starts -------------------------------//
//---------------------------------------------------------------------------------//

//
// Function to generate a sample dataset
//
void generateDataset(Transaction *transactions, int transactionCount, Order *orders)
{
    for (int i = 0; i < transactionCount; i++)
    {
        // printOrder(&orders[i]);
        int count = 0;
        for (int j = 0; j < MAX_ORDER_ITEMS; j++)
        {
            if (orders[i].items[j].quantity > 0 && orders[i].items[j].quantity < 30 && orders[i].items[j].totalPrice > 1 && strlen(trim(orders[i].items[j].item.name)) > 0)
            {
                strcpy(transactions[i].items[count].item, orders[i].items[j].item.name);
                // printf("###  generate data set : %s\n", transactions[i].items[count].item);
                count++;
            }
        }
        transactions[i].itemCount = count;
    }
}
//
// Function to check if an itemset is a subset of a transaction
//
int isSubset(Itemset subset, Transaction transaction)
{
    // printf("# subset.itemcount : %d - transaction.itemCount : %d\n", subset.itemCount, transaction.itemCount);

    int matchCount = 0;
    for (int i = 0; i < subset.itemCount; i++)
    {
        // printf("# subset.items[i].item %s - ", subset.items[i].item);
        for (int j = 0; j < transaction.itemCount; j++)
        {
            // printf("transaction.items[j].item : %s - ", transaction.items[j].item);
            if (strcmp(subset.items[i].item, transaction.items[j].item) == 0)
            {
                matchCount++;
                // printf("matchCount : %d", matchCount);
                break;
            }
        }
        // printf("-----------\n");
    }
    return (matchCount == subset.itemCount);
}

//
// Function to count the support of each itemset
//
void countSupport(Itemset *itemsets, int itemsetCount, Transaction *transactions, int transactionCount)
{
    for (int i = 0; i < itemsetCount; i++)
    {
        itemsets[i].supportCount = 0;
        for (int j = 0; j < transactionCount; j++)
        {
            if (isSubset(itemsets[i], transactions[j]))
            {
                itemsets[i].supportCount++;
            }
        }
    }
}

//
// Function to check if two itemsets can be joined to form a larger itemset
//
int canJoin(Itemset a, Itemset b)
{
    if (a.itemCount != b.itemCount)
        return 0;
    for (int i = 0; i < a.itemCount - 1; i++)
    {
        if (strcmp(a.items[i].item, b.items[i].item) != 0)
        {
            return 0;
        }
    }
    return strcmp(a.items[a.itemCount - 1].item, b.items[b.itemCount - 1].item) < 0;
}

//
// Function to join two itemsets to form a larger itemset
//
Itemset joinItemsets(Itemset a, Itemset b)
{
    Itemset result;
    result.itemCount = a.itemCount + 1;
    for (int i = 0; i < a.itemCount; i++)
    {
        strcpy(result.items[i].item, a.items[i].item);
    }
    strcpy(result.items[a.itemCount].item, b.items[b.itemCount - 1].item);
    return result;
}

//
// Function to generate candidate itemsets by joining existing itemsets
//
void generateCandidates(Itemset *itemsets, int itemsetCount, Itemset *candidates, int *candidateCount)
{
    *candidateCount = 0;
    for (int i = 0; i < itemsetCount; i++)
    {
        for (int j = i + 1; j < itemsetCount; j++)
        {
            if (canJoin(itemsets[i], itemsets[j]))
            {
                Itemset candidate = joinItemsets(itemsets[i], itemsets[j]);
                candidates[*candidateCount] = candidate;
                (*candidateCount)++;
            }
        }
    }
}

//
// Function to prune itemsets that do not meet the minimum support
//
void pruneCandidates(Itemset *candidates, int *candidateCount)
{
    int newCount = 0;
    for (int i = 0; i < *candidateCount; i++)
    {
        if (candidates[i].supportCount >= MIN_SUPPORT)
        {
            candidates[newCount++] = candidates[i];
        }
    }
    *candidateCount = newCount;
}

//
// Function to print an itemset with its support count
//
void printItemset(Itemset itemset)
{
    printf("{ ");
    for (int i = 0; i < itemset.itemCount; i++)
    {
        printf("%s ", itemset.items[i].item);
    }
    printf("} : %d\n", itemset.supportCount);
}

//
// Function implementing the Apriori algorithm
//
void apriori(Transaction *transactions, int transactionCount, Itemset *itemsets)
{

    int itemsetCount = 0;

    // Generate initial itemsets (1-item itemsets)
    for (int i = 0; i < transactionCount; i++)
    {
        for (int j = 0; j < transactions[i].itemCount; j++)
        {
            int found = 0;
            for (int k = 0; k < itemsetCount; k++)
            {
                if (strcmp(itemsets[k].items[0].item, transactions[i].items[j].item) == 0)
                {
                    found = 1;
                    break;
                }
            }
            if (!found)
            {
                itemsets[itemsetCount].itemCount = 1;
                strcpy(itemsets[itemsetCount].items[0].item, transactions[i].items[j].item);
                itemsetCount++;
            }
        }
    }

    // Iterate while there are itemsets to process and itemsets can have up to 4 items
    int iteration = 1;
    while (itemsetCount > 0 && iteration <= 4)
    {

        // Count support for current itemsets
        countSupport(itemsets, itemsetCount, transactions, transactionCount);

        // Prune itemsets that do not meet minimum support
        pruneCandidates(itemsets, &itemsetCount);

        // Print the frequent itemsets
        for (int i = 0; i < itemsetCount; i++)
        {
            printItemset(itemsets[i]);
        }

        // Generate candidate itemsets for the next iteration
        Itemset candidates[MAX_ITEMS_PER_TRANSACTION * MAX_ITEMS_PER_TRANSACTION];
        int candidateCount;
        generateCandidates(itemsets, itemsetCount, candidates, &candidateCount);

        // Count support for candidate itemsets
        countSupport(candidates, candidateCount, transactions, transactionCount);

        // Prune candidate itemsets that do not meet minimum support
        pruneCandidates(candidates, &candidateCount);

        // Update itemsets for the next iteration
        itemsetCount = candidateCount;
        memcpy(itemsets, candidates, candidateCount * sizeof(Itemset));

        // Increment the iteration counter
        iteration++;
    }

    // printf(" #######  itemsetCount:  %d", itemsetCount);
}

//
// Utility method to print the transactons
//
void printTransactions(Transaction *transactions, int transactionCount)
{
    printf(" ## Transaction count = %d\n", transactionCount);
    for (int i = 0; i < transactionCount; i++)
    {
        for (int j = 0; j < transactions[i].itemCount; j++)
        {
            printf("Transactions[%d].items[%d] = %s \n", i, j, transactions[i].items[j]);
        }
    }
}

//
// Get the top items
//
void getTopItem(const Itemset *inputItemsets, char *itemName, ItemA *companianItems, int level)
{
    int i = 0, maxCount = -1;
    char compItem[200];
    while (1)
    {
        if (inputItemsets[i].itemCount < 1)
        {
            break;
        }
        if (level == 1 && inputItemsets[i].itemCount == level)
        {

            if (inputItemsets[i].supportCount > maxCount)
            {
                maxCount = inputItemsets[i].supportCount;
                strcpy(itemName, inputItemsets[i].items[0].item);
            }
        }
        else if (level > 1 && inputItemsets[i].itemCount == level)
        {
            bool found = false;
            for (int j = 0; j < level - 1; j++)
            {
                for (int k = 0; k < inputItemsets[i].itemCount; k++)
                {
                    if (strcmp(inputItemsets[i].items[k].item, companianItems[j].item) == 0)
                    {
                        found = true;
                    }
                    else
                    {
                        if (inputItemsets[i].supportCount > maxCount)
                        {
                            maxCount = inputItemsets[i].supportCount;
                            strcat(compItem, inputItemsets[i].items[k].item);
                            strcat(compItem, "  ");
                        }
                    }
                }
            }

            if (found)
            {
                strcpy(itemName, compItem);
            }
        }

        i++;
    }
}

//
// A method to identify the companian items
//
void getPair(char *item, Itemset *itemsets, char *companion, int transactionCount, int depth)
{
    // added companion to function after reccomendation threw garbage values
    // companion[0] = '\0';

    for (int i = 0; i < transactionCount; i++)
    {
        if (itemsets[i].itemCount < 1 || itemsets[i].itemCount > 10)
        {
            break;
        }
        // printf("## item : %s \n\n", itemsets[i].items[0].item);
        if (itemsets[i].itemCount > 1 && itemsets[i].itemCount <= depth && strcmp(item, itemsets[i].items[0].item) == 0)
        {
            for (int j = 1; j < itemsets[i].itemCount; j++)
            {
                if (j > 1)
                {
                    strcat(companion, ", ");
                }
                strcat(companion, itemsets[i].items[j].item);
            }
            // printf("\n");
            break;
        }
    }

    // printf("## Companion : %s \n\n", companion);
}

//
// Helper function to find the most ordered item for a given restaurant
//
void findMostOrderedItemForRestaurant(Order *orders, int orderCount, const char *restaurantID, char *mostOrderItem)
{
    int itemCount[MAX_ITEMS] = {0};
    char itemNames[MAX_ITEMS][50] = {0};
    int uniqueItemCount = 0;

    for (int i = 0; i < orderCount; i++)
    {
        if (strcmp(orders[i].restaurant.ID, restaurantID) == 0)
        {
            for (int j = 0; j < MAX_ITEMS; j++)
            {
                if (orders[i].items[j].quantity > 0 && orders[i].items[j].quantity < 30 && orders[i].items[j].totalPrice > 1 && strlen(trim(orders[i].items[j].item.name)) > 0)
                {
                    //  printf("## Item name : %s\n", orders[i].items[j].item.name);

                    bool found = false;
                    for (int k = 0; k < uniqueItemCount; k++)
                    {
                        //  printf("### Item Name in unique list %s, item name in the order %s, item count is %d, index is %d\n", itemNames[k], orders[i].items[j].item.name, itemCount[k], k);

                        if (strcmp(itemNames[k], orders[i].items[j].item.name) == 0)
                        {
                            itemCount[k] += orders[i].items[j].quantity;
                            found = true;
                            break;
                        }
                    }

                    if (!found)
                    {
                        strcpy(itemNames[uniqueItemCount], orders[i].items[j].item.name);
                        itemCount[uniqueItemCount] = orders[i].items[j].quantity;
                        uniqueItemCount++;
                    }
                }
            }
        }
    }

    int maxIndex = 0;
    for (int i = 1; i < uniqueItemCount; i++)
    {
        if (itemCount[i] > itemCount[maxIndex])
        {
            maxIndex = i;
        }
    }

    // printf("Most ordered item for restaurant %s: %s, Count: %d\n", restaurantID, itemNames[maxIndex], itemCount[maxIndex]);
    strcpy(mostOrderItem, itemNames[maxIndex]);
}

//
// Helper function to find the most common companion item for a given item in a given restaurant
//
void findMostCommonCompanionItem(Order *orders, int orderCount, const char *restaurantID, const char *itemName, char *mostOrderItem)
{
    int companionCount[MAX_ITEMS] = {0};
    char companionNames[MAX_ITEMS][50] = {0};
    int uniqueCompanionCount = 0;

    char restaurantName[300];

    for (int i = 0; i < orderCount; i++)
    {
        if (strcmp(orders[i].restaurant.ID, restaurantID) == 0)
        {
            strcpy(restaurantName, orders[i].restaurant.name);
            bool itemFound = false;
            for (int j = 0; j < MAX_ITEMS; j++)
            {
                if (orders[i].items[j].quantity > 0 && orders[i].items[j].quantity < 30 && strcmp(orders[i].items[j].item.name, itemName) == 0 && orders[i].items[j].totalPrice > 1 && strlen(trim(orders[i].items[j].item.name)) > 0)
                {
                    itemFound = true;
                    break;
                }
            }
            if (itemFound)
            {
                for (int j = 0; j < MAX_ITEMS; j++)
                {
                    if (orders[i].items[j].quantity > 0 && orders[i].items[j].quantity < 30 && strcmp(orders[i].items[j].item.name, itemName) != 0 && orders[i].items[j].totalPrice > 1 && strlen(trim(orders[i].items[j].item.name)) > 0)
                    {
                        bool found = false;
                        for (int k = 0; k < uniqueCompanionCount; k++)
                        {
                            if (strcmp(companionNames[k], orders[i].items[j].item.name) == 0)
                            {
                                companionCount[k] += orders[i].items[j].quantity;
                                found = true;
                                break;
                            }
                        }
                        if (!found)
                        {
                            strcpy(companionNames[uniqueCompanionCount], orders[i].items[j].item.name);
                            companionCount[uniqueCompanionCount] = orders[i].items[j].quantity;
                            uniqueCompanionCount++;
                        }
                    }
                }
            }
        }
    }

    int maxIndex = 0;
    for (int i = 1; i < uniqueCompanionCount; i++)
    {
        if (companionCount[i] > companionCount[maxIndex])
        {
            maxIndex = i;
        }
    }

    strcpy(mostOrderItem, companionNames[maxIndex]);
    if (uniqueCompanionCount > 0)
    {
        printf("\nMost common companion item for %s in restaurant %s: %s\n", itemName, restaurantName, companionNames[maxIndex]);
    }
}

//
// Helper function to find the most ordered item across restaurants
//
void gettemsAcrossRestaurantOrders(Order *orders, int orderCount, char *mostOrderItem)
{
    int itemCount[MAX_ITEMS] = {0};
    char itemNames[MAX_ITEMS][50] = {0};
    int uniqueItemCount = 0;

    for (int i = 0; i < orderCount; i++)
    {
        for (int j = 0; j < MAX_ORDER_ITEMS; j++)
        {

            if (orders[i].items[j].quantity > 0 && orders[i].items[j].totalPrice > 1 && strlen(trim(orders[i].items[j].item.name)) > 0)
            {
                // printf("## Item name : %s\n", orders[i].items[j].item.name);

                bool found = false;
                for (int k = 0; k < uniqueItemCount; k++)
                {
                    //  printf("### Item Name in unique list %s, item name in the order %s, item count is %d, index is %d\n", itemNames[k], orders[i].items[j].item.name, itemCount[k], k);

                    if (strcmp(itemNames[k], orders[i].items[j].item.name) == 0)
                    {
                        itemCount[k] += orders[i].items[j].quantity;
                        found = true;
                        break;
                    }
                }

                if (!found)
                {
                    strcpy(itemNames[uniqueItemCount], orders[i].items[j].item.name);
                    itemCount[uniqueItemCount] = orders[i].items[j].quantity;
                    uniqueItemCount++;
                }
            }
        }
    }

    int maxIndex = 0;
    for (int i = 1; i < uniqueItemCount; i++)
    {
        if (itemCount[i] > itemCount[maxIndex])
        {
            maxIndex = i;
        }
    }

    // printf("Most ordered item across restaurants: %s, Count: %d\n", itemNames[maxIndex], itemCount[maxIndex]);

    strcpy(mostOrderItem, itemNames[maxIndex]);
}
#endif
