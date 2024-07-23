#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include <stdlib.h>
#include <time.h>

#include "splash.h"
#include "login.h"
#include "customer.h"
#include "order.h"
#include "restaurant.h"
#include "utility.h"
#include "display-helper.h"
#include "dijkstra.h"
#include "apriori.h"
#include "pet.h"
#include "grocery.h"

//---------------------------------------------------------------------------------//
// ------------------- Method Definitions---------- -------------------------------//
//---------------------------------------------------------------------------------//

//---------------------------------------------------------------------------------//
// ------------------- Method Implementation Starts -------------------------------//
//---------------------------------------------------------------------------------//

//
// The main
//
int main()
{
    int userChoice = -1;
    char username[MAX_USERNAME_LENGTH];
    Customer customer;

    // Start by displaying splash screen
    printSplashScreen();

    // Run the display login in while loop
    while (1)
    {

        // Get user choice
        // 1 for login
        // 2 for registration
        // 8 for repeat from start
        // 9 for exit
        userChoice = printAndCollectLoginOrRegistrationChoice();

        // if choice is 9, exit the app with a bye
        if (userChoice == 9)
        {
            printBye();
            return 0;
        }

        // if the choice is 8, it means the user want go back to previous menu, so continue
        else if (userChoice == 8)
        {
            continue;
        }

        // if choice is 1, then user want to login with user id and passwod, so launch that
        else if (userChoice == 1)
        {
            captcha();
            
            int menuChoice = printAndCollectUserCredentials(&username);

            // if choice is 8, then user wants to go to previous menu, so do that
            if (menuChoice == 8)
            {
                continue;
            }

            searchCustomerByPhoneNumber(&username, &customer);

            printf(" Hello %s \n\n", customer.name);
        }

        // if choice is 2, the user wants to register, so first create customer record and then user cred
        else if (userChoice == 2)
        {

            int menuChoice = collectCustomerDetails(&customer);

            // if choice is great than 8, then it is error, so terminate
            if (menuChoice > 8)
            {
                printRed("Error in creating customer record.  Failing!");
                break;
            }

            menuChoice = registerUser(&(customer.phoneNumber));

            // if choice is great than 8, then it is error, , so terminate
            if (menuChoice > 8)
            {
                printRed("Error in creating credentials.  Failing!\n");
                break;
            }
        }

        //
        // Show previous order option only if the user logs in.  He registering, skip the rating menu
        if (userChoice == 1)
        {
            int menuChoice = printAndCollectRatingOrOrderingChoice();

            // if choice is great than 8, then it is error, so terminate
            if (menuChoice > 8)
            {
                printRed("Error in creating customer record.  Failing!\n");
                continue;
            }
            // if choice is 1, need to update the prevous order rating
            else if (menuChoice == 1)
            {
                collectAndUpdateRestaurantRatingBasedOnPreviousOrder(&username);
            }

            // if choice is 1, need to place order.  this will be choice even when a new user is registered
            // so change the user choice to 2 (which is the new user registration option),
            // in the subsequent flow, we will direct user to order choice menu
            else if (menuChoice == 2)
            {
                userChoice = 2;
            }

            // printf("Success continue.....\n#### menuChoice : %d, userCHoice : %d \n\n", menuChoice, userChoice);
        }

        // if user is newly register, then show them the order choice menu
        if (userChoice == 2)
        {
            int menuChoice = printAndCollectPetOrFoodOrGroceryChoice();
            // printf("Success continue.....\n#### menuChoice : %d, userCHoice : %d \n\n", menuChoice, userChoice);

            if (menuChoice == 8)
            {
                continue;
            }
            else if (menuChoice == 1)
            {
                int petChoice = petMain(customer.addresses[0].city);

                printf("\n############ Track your Order #################\n");
                char orderId[16];
                generateID(orderId, ORDER_ID_SIZE, 'O');
                processOrder(NULL, 1, orderId);
            }
            else if (menuChoice == 2)
            {
                int orderChoice = collectFoodOrders(username);
            }
            else
            {
                int groceryCHoice = groceryMain(customer.addresses[0].city);

                printf("\n############ Track your Order #################\n");
                char orderId[16];
                generateID(orderId, ORDER_ID_SIZE, 'O');
                processOrder(NULL, 1, orderId);
            }
        }
    }

    return 0;
}
