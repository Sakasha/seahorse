#include <stdio.h>  //For scanf and printf
#include <limits.h> //For INT_MAX
#include <stdlib.h> //For srand and malloc
#include <string.h> //For strcmp and other string functions
#include <ctype.h>  //For isspace
#include <time.h>   //srand(time(NULL))
#include "delivery-partner.h"

#ifndef _DIJKSTRA_
#define _DIJKSTRA_

/*
    This file handles idijkstra algorithm for calculating distance
    the location details and restaurant names are hard coded
*/

#define V 20        // Number of vertices (locations)
#define INF INT_MAX // Infinite distance value

//---------------------------------------------------------------------------------//
// ------------------- Structure Definitions---------------------------------------//
//---------------------------------------------------------------------------------//

//---------------------------------------------------------------------------------//
// ------------------- Method Definitions---------- -------------------------------//
//---------------------------------------------------------------------------------//

void toLowerCase(char *str);
int getIndex(char *vertex, char *vertices[]);
int minDistance(int dist[], int visited[]);
float dijkstra(long distances[V][V], char *src, char *dest, char *vertices[]);
void trims(char str[]);
float dijk(char *restaurantName, char *userLocation);

//---------------------------------------------------------------------------------//
// ------------------- Method Implementation Starts -------------------------------//
//---------------------------------------------------------------------------------//

//
// Function to convert a string to lowercase
//
void toLowerCase(char *str)
{
    for (int i = 0; str[i]; i++)
    {
        str[i] = tolower(str[i]);
    }
}


//
// Function to find the index of a vertex in the graph given its name (case-insensitive)
//
int getIndex(char *vertex, char *vertices[])
{
    char lower_vertex[50];
    strcpy(lower_vertex, vertex);
    toLowerCase(lower_vertex); // Converts the inputted vertex to lower case

    for (int i = 0; i < V; i++)
    {
        char lower_stored_vertex[50];
        strcpy(lower_stored_vertex, vertices[i]);
        toLowerCase(lower_stored_vertex); // Converts the vertices stored to lower case

        if (strcmp(lower_vertex, lower_stored_vertex) == 0) // Checks if the inputted restaurant is present
            return i;
    }
    return -1; // Vertex not found
}

//
// Function to find the vertex with minimum distance value
//
int minDistance(int dist[], int visited[])
{
    int min = INF, min_index;
    for (int v = 0; v < V; v++)
    {
        if (visited[v] == 0 && dist[v] <= min) // Checks if it hasn't been visited and has min distance
        {
            min = dist[v];
            min_index = v;
        }
    }
    return min_index;
}
//
// Function to implement Dijkstra's algorithm
//
float dijkstra(long distances[V][V], char *src, char *dest, char *vertices[])
{
    int dist[V];    // The output array. dist[i] will hold the shortest distance from src to i
    int visited[V]; // visited[i] will be true if vertex i is included in shortest path tree or shortest distance from src to i is finalized

    int srcIndex = getIndex(src, vertices);
    int destIndex = getIndex(dest, vertices);

    // Initialize all distances as INFINITE and visited[] as false
    for (int i = 0; i < V; i++)
    {
        dist[i] = INF;
        visited[i] = 0;
    }

    dist[srcIndex] = 0; // Distance of source vertex from itself is always 0

    // Find shortest path for all vertices
    for (int count = 0; count < V - 1; count++)
    {
        // Pick the minimum distance vertex from the set of vertices not yet processed.
        // u is always equal to src in the first iteration.
        int u = minDistance(dist, visited);

        // Mark the picked vertex as visited
        visited[u] = 1;

        // Update dist value of the adjacent vertices of the picked vertex.
        for (int v = 0; v < V; v++)
        {
            // Update dist[v] only if it's not in visited, there is an edge from u to v,
            // and the total weight of path from src to v through u is smaller than current value of dist[v]
            if (!visited[v] && distances[u][v] != INF && dist[u] != INF && dist[u] + distances[u][v] < dist[v])
                dist[v] = dist[u] + distances[u][v];
        }
    }

    // Return the shortest path from source to destination
    return dist[destIndex];
}

//
// Function to trim leading and trailing whitespace from a string
//
void trims(char str[])
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

//
// Core dijkstra algorithm implementation
//
float dijk(char *restaurantName, char *userLocation)
{
    char *vertices[V] = {"Sholinganallur", "Anna Nagar", "Adyar", "Kalavakkam", "Neelankarai", "Flavour Fusion", "Coastal Cravings", "Herb And Hummus", "MFC", "Pizza Mania", "Anna Bhavan", "Momo Territory", "Luna Bite", "Earthy Eats", "Burger Queen", "SereniTea", "Kalavakkam Bhavan", "Tasteful Oasis", "The Hungry Bear", "Leaf And Ladle"};
    long distances[V][V] = {
        {0, 10, 17, 16, 9, 7, 6, 9, INF, INF, INF, INF, INF, INF, INF, INF, INF, INF, INF, INF},
        {10, 0, 13, 14, 13, INF, INF, INF, 5, 8, 7, INF, INF, INF, INF, INF, INF, INF, INF, INF},
        {17, 13, 0, 12, 14, INF, INF, INF, INF, INF, INF, 9, 4, 6, INF, INF, INF, INF, INF, INF},
        {16, 15, 12, 0, 11, INF, INF, INF, INF, INF, INF, INF, INF, INF, 3, 7, 5, INF, INF, INF},
        {9, 13, 14, 11, 0, INF, INF, INF, INF, INF, INF, INF, INF, INF, INF, INF, INF, 6, 2, 9},
        {7, INF, INF, INF, INF, 0, 2, INF, INF, INF, INF, INF, INF, INF, INF, INF, INF, INF, INF, 11},
        {6, INF, INF, INF, INF, 2, 0, 1, INF, INF, INF, INF, INF, INF, INF, INF, INF, INF, INF, INF},
        {9, INF, INF, INF, INF, INF, 1, 0, 10, INF, INF, INF, INF, INF, INF, INF, INF, INF, INF, INF},
        {INF, 5, INF, INF, INF, INF, INF, 10, 0, 2, INF, INF, INF, INF, INF, INF, INF, INF, INF, INF},
        {INF, 8, INF, INF, INF, INF, INF, INF, 2, 0, 3, INF, INF, INF, INF, INF, INF, INF, INF, INF},
        {INF, 7, INF, INF, INF, INF, INF, INF, INF, 3, 0, 11, INF, INF, INF, INF, INF, INF, INF, INF},
        {INF, INF, 9, INF, INF, INF, INF, INF, INF, INF, 11, 0, 5, INF, INF, INF, INF, INF, INF, INF},
        {INF, INF, 4, INF, INF, INF, INF, INF, INF, INF, INF, 5, 0, 4, INF, INF, INF, INF, INF, INF},
        {INF, INF, 6, INF, INF, INF, INF, INF, INF, INF, INF, INF, 4, 0, 12, INF, INF, INF, INF, INF},
        {INF, INF, INF, 3, INF, INF, INF, INF, INF, INF, INF, INF, INF, 12, 0, 3, INF, INF, INF, INF},
        {INF, INF, INF, 7, INF, INF, INF, INF, INF, INF, INF, INF, INF, INF, 3, 0, 5, INF, INF, INF},
        {INF, INF, INF, 5, INF, INF, INF, INF, INF, INF, INF, INF, INF, INF, INF, 5, 0, 9, INF, INF},
        {INF, INF, INF, INF, 6, INF, INF, INF, INF, INF, INF, INF, INF, INF, INF, INF, 9, 0, 5, INF},
        {INF, INF, INF, INF, 2, INF, INF, INF, INF, INF, INF, INF, INF, INF, INF, INF, INF, 5, 0, 3},
        {INF, INF, INF, INF, 9, 11, INF, INF, INF, INF, INF, INF, INF, INF, INF, INF, INF, INF, 3, 0}};

    DeliveryPartner *info[100];
    int index = 0;
    char file_content1[1000];

    FILE *file = fopen(DP_FILENAME, "r"); // Opening the file
    if (file == NULL)
    {
        printf("Error opening file %s.\n", DP_FILENAME);
        return 1;
    }

    while (fgets(file_content1, sizeof(file_content1), file))
    {
        // Allocate memory for each DeliveryPartner instance
        info[index] = (DeliveryPartner *)malloc(sizeof(DeliveryPartner));
        if (info[index] == NULL)
        {
            printf("Memory allocation failed.\n");
            return 1;
        }
        // Read data from file
        sscanf(file_content1, "%[^,],%[^,],%[^,\n]\n", info[index]->name, info[index]->phone_number, info[index]->from_location);
        index++;
    }

    fclose(file);

    float min_distance = INF;
    float total_distance;
    float f;
    float distance;
    int flag = -1;
    char dp_loc[50];

    trim(restaurantName);
    toLowerCase(restaurantName); // Convert user input to lowercase
    // printf("\n### restaurantName after trimming : %s\n", restaurantName);

    trim(userLocation);
    toLowerCase(userLocation); // Convert user input to lowercase
    // printf("###To userLocation : %s\n", userLocation);

    for (int i = index - 1; i >= 0; i--)
    {
        strcpy(dp_loc, info[i]->from_location);
        trim(dp_loc);
        toLowerCase(dp_loc); // Convert stored location to lowercase
        distance = dijkstra(distances, dp_loc, restaurantName, vertices);
        if (distance < min_distance)
        {
            min_distance = distance;
            flag = i;
        }
    }

    total_distance = min_distance + dijkstra(distances, restaurantName, userLocation, vertices);

    // Free allocated memory
    for (int i = 0; i < index; i++)
    {
        free(info[i]);
    }

    return total_distance;
}

void djWithDP(char *restaurantName, char *restaurantLocation, char *userLocation, char *orderId)
{
    char *vertices[V] = {"Sholinganallur", "Anna Nagar", "Adyar", "Kalavakkam", "Neelankarai", "Flavour Fusion", "Coastal Cravings", "Herb And Hummus", "MFC", "Pizza Mania", "Anna Bhavan", "Momo Territory", "Luna Bite", "Earthy Eats", "Burger Queen", "SereniTea", "Kalavakkam Bhavan", "Tasteful Oasis", "The Hungry Bear", "Leaf And Ladle"};
    long distances[V][V] = {
        {0, 10, 17, 16, 9, 7, 6, 9, INF, INF, INF, INF, INF, INF, INF, INF, INF, INF, INF, INF},
        {10, 0, 13, 14, 13, INF, INF, INF, 5, 8, 7, INF, INF, INF, INF, INF, INF, INF, INF, INF},
        {17, 13, 0, 12, 14, INF, INF, INF, INF, INF, INF, 9, 4, 6, INF, INF, INF, INF, INF, INF},
        {16, 15, 12, 0, 11, INF, INF, INF, INF, INF, INF, INF, INF, INF, 3, 7, 5, INF, INF, INF},
        {9, 13, 14, 11, 0, INF, INF, INF, INF, INF, INF, INF, INF, INF, INF, INF, INF, 6, 2, 9},
        {7, INF, INF, INF, INF, 0, 2, INF, INF, INF, INF, INF, INF, INF, INF, INF, INF, INF, INF, 11},
        {6, INF, INF, INF, INF, 2, 0, 1, INF, INF, INF, INF, INF, INF, INF, INF, INF, INF, INF, INF},
        {9, INF, INF, INF, INF, INF, 1, 0, 10, INF, INF, INF, INF, INF, INF, INF, INF, INF, INF, INF},
        {INF, 5, INF, INF, INF, INF, INF, 10, 0, 2, INF, INF, INF, INF, INF, INF, INF, INF, INF, INF},
        {INF, 8, INF, INF, INF, INF, INF, INF, 2, 0, 3, INF, INF, INF, INF, INF, INF, INF, INF, INF},
        {INF, 7, INF, INF, INF, INF, INF, INF, INF, 3, 0, 11, INF, INF, INF, INF, INF, INF, INF, INF},
        {INF, INF, 9, INF, INF, INF, INF, INF, INF, INF, 11, 0, 5, INF, INF, INF, INF, INF, INF, INF},
        {INF, INF, 4, INF, INF, INF, INF, INF, INF, INF, INF, 5, 0, 4, INF, INF, INF, INF, INF, INF},
        {INF, INF, 6, INF, INF, INF, INF, INF, INF, INF, INF, INF, 4, 0, 12, INF, INF, INF, INF, INF},
        {INF, INF, INF, 3, INF, INF, INF, INF, INF, INF, INF, INF, INF, 12, 0, 3, INF, INF, INF, INF},
        {INF, INF, INF, 7, INF, INF, INF, INF, INF, INF, INF, INF, INF, INF, 3, 0, 5, INF, INF, INF},
        {INF, INF, INF, 5, INF, INF, INF, INF, INF, INF, INF, INF, INF, INF, INF, 5, 0, 9, INF, INF},
        {INF, INF, INF, INF, 6, INF, INF, INF, INF, INF, INF, INF, INF, INF, INF, INF, 9, 0, 5, INF},
        {INF, INF, INF, INF, 2, INF, INF, INF, INF, INF, INF, INF, INF, INF, INF, INF, INF, 5, 0, 3},
        {INF, INF, INF, INF, 9, 11, INF, INF, INF, INF, INF, INF, INF, INF, INF, INF, INF, INF, 3, 0}};

    DeliveryPartner *info[100];
    int index = 0;
    char file_content1[1000];

    FILE *file = fopen(DP_FILENAME, "r"); // Opening the file
    if (file == NULL)
    {
        printf("Error opening file %s.\n", DP_FILENAME);
        return 1;
    }

    while (fgets(file_content1, sizeof(file_content1), file))
    {
        // Allocate memory for each DeliveryPartner instance
        info[index] = (DeliveryPartner *)malloc(sizeof(DeliveryPartner));
        if (info[index] == NULL)
        {
            printf("Memory allocation failed.\n");
            return 1;
        }
        // Read data from file
        sscanf(file_content1, "%[^,],%[^,],%[^,\n]\n", info[index]->name, info[index]->phone_number, info[index]->from_location);
        index++;
    }

    fclose(file);

    float min_distance = INF;
    float total_distance;
    float f;
    float distance;
    int flag = -1;
    char dp_loc[50];

    trim(restaurantName);
    toLowerCase(restaurantName); // Convert user input to lowercase
    // printf("\n### restaurantName after trimming : %s\n", restaurantName);

    trim(userLocation);
    toLowerCase(userLocation); // Convert user input to lowercase

    trim(restaurantLocation);
    toLowerCase(restaurantLocation); // Convert user input to lowercase
    // printf("###To userLocation : %s\n", userLocation);

    for (int i = index - 1; i >= 0; i--)
    {
        strcpy(dp_loc, info[i]->from_location);
        trim(dp_loc);
        toLowerCase(dp_loc); // Convert stored location to lowercase
        distance = dijkstra(distances, dp_loc, restaurantName, vertices);
        if (distance < min_distance)
        {
            min_distance = distance;
            flag = i;
        }
    }

    total_distance = min_distance + dijkstra(distances, restaurantName, userLocation, vertices);

    handleDeliveryPartner(info, restaurantLocation, userLocation, flag, orderId, total_distance);

    // Free allocated memory
    for (int i = 0; i < index; i++)
    {
        free(info[i]);
    }
}

#endif