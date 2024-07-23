#include <stdio.h>  //For scanf and printf
#include <limits.h> //For INT_MAX
#include <stdlib.h> //For srand and malloc
#include <string.h> //For strcmp and other string functions
#include <ctype.h>  //For isspace
#include <time.h>   //srand(time(NULL))

#ifndef DELIVERY_H
#define DELIVERY_H


#define DP_FILENAME "./data/delivery-partner-db.txt"


//---------------------------------------------------------------------------------//
// ------------------- Structure Definitions---------------------------------------//
//---------------------------------------------------------------------------------//

// Structure to represent a delivery partner
typedef struct
{
    char name[50];
    char phone_number[20];
    char from_location[50];
    char to_location[50];
    char user_location[50];
    char orderID[10];
    char instructions[1000];
} DeliveryPartner;


//---------------------------------------------------------------------------------//
// ------------------- Method Definitions---------- -------------------------------//
//---------------------------------------------------------------------------------//

void generate_order_id(char *order_id, size_t length);
void handleDeliveryPartner( DeliveryPartner **info, char *res_loc, char *user_loc, int flag, char *orderId, float total_distance);

//---------------------------------------------------------------------------------//
// ------------------- Method Implementation Starts -------------------------------//
//---------------------------------------------------------------------------------//
void generate_order_id(char *order_id, size_t length) 
{
    const char characters[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    size_t num_characters = sizeof(characters) - 1;

    for (size_t i = 0; i < length; ++i) 
    {
        order_id[i] = characters[rand() % num_characters];
    }
    order_id[length] = '\0'; // Null-terminate the string
}

void handleDeliveryPartner( DeliveryPartner **info, char *res_loc, char *user_loc, int flag, char *orderId, float total_distance)
{

    int instruction_choice;
    if (flag != -1) 
    {
        printf("\nDelivery Partner details:\n");
        printf("Name : %s\n", info[flag]->name);
        printf("Mobile number : %s\n", info[flag]->phone_number);
        printf("From location : %s\n", info[flag]->from_location);
        printf("To location : %s\n", res_loc);

        // Calculate time taken
        float speed = 40.0; // Speed in km/h
        float time_in_hours = total_distance / speed;
        float time_in_minutes = time_in_hours * 60;
        printf("\nDistance from your location: %.1f", total_distance);
        printf("\nTime taken: %.2f hours or %.2f minutes\n", time_in_hours, time_in_minutes);
    } 
  

    FILE *file = fopen(DP_FILENAME, "r+");
    if (flag != -1)
    {
        strcpy(info[flag]->to_location, res_loc);
        printf("\nDo you want to give instructions to the delivery partner?\nEnter 1 to give instructions.\nEnter any other character otherwise\n");
        scanf("%d", &instruction_choice);
        if (instruction_choice == 1)
        {
            int c;
            while ((c = getchar()) != '\n' && c != EOF)
                ;
            printf("\nEnter your instructions\n");
            fgets(info[flag]->instructions, 1000, stdin);
        }
        else
        {
            strcpy(info[flag]->instructions, "\n");
        }
    }

    fseek(stdin,0,SEEK_END);

    if (file == NULL)
    {
        printf("Error opening file.\n");
        return 1;
    }


    srand(time(NULL)); // Seed the random number generator

    size_t length = 6;
    char order_id[length + 1]; // +1 for the null-terminator

    // generate_order_id(order_id, length);
    printf("Generated Order ID: %s\n", orderId);
    strcpy(info[flag]->orderID, order_id);
    strcpy(info[flag]->user_location, user_loc);
    // Read the contents of the file into memory
    char file_content[1000];
    char modified_content[1000] = ""; // Initialize as empty string
    int line_number = 0;

    // Loop through each line in the file
    while (fgets(file_content, sizeof(file_content), file))
    {
        if (line_number != flag)
        {
            strcat(modified_content, file_content);
        }
        else
        {
            char new_entry[1000];
            snprintf(new_entry, sizeof(new_entry), "%s,%s,%s,%s,%s,%s,%s", info[flag]->name, info[flag]->phone_number, info[flag]->from_location, info[flag]->to_location, info[flag]->user_location, info[flag]->orderID, info[flag]->instructions);
            strcat(modified_content, new_entry);
        }
        line_number++;
    }

    // Write the modified content back to the file
    freopen(DP_FILENAME, "w", file);
    fputs(modified_content, file);
    fclose(file);
}
#endif