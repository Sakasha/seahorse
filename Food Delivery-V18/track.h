#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include "dijkstra.h"
#include "delivery-partner.h"

#define DELIVERY_FILE "./data/delivery-partner-db.txt"

void processOrder(DeliveryPartner *info[], int index, char *orderId);

int trackOrder(char *restaurantName, char *restaurantLocation, char *userLocation, char *orderId)
{
    DeliveryPartner *info[100];
    int index = 0;

    djWithDP(restaurantName, restaurantLocation, userLocation, orderId);
    processOrder(info, index, orderId);

    // Free allocated memory
    for (int i = 0; i < index; i++)
    {
        free(info[i]);
    }

    return 0;
}

void processOrder(DeliveryPartner *info[], int index, char *orderId)
{
    char orderid[10];
    int check = 1;
    struct timespec ts;

    printf("\nOrder Placed\n");

    ts.tv_sec = 5;  // 5 seconds
    ts.tv_nsec = 0; // 0 nanoseconds

    nanosleep(&ts, NULL);

    printf("\nOrder picked up\n");
    ts.tv_sec = 5;  // 5 seconds
    ts.tv_nsec = 0; // 0 nanoseconds

    nanosleep(&ts, NULL);

    printf("\nOrder Delivered\n");
}
