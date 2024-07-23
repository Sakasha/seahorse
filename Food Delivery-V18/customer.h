#include <stdio.h>
#include <string.h>
#include "login.h"
#include "utility.h"

#ifndef CUSTOMER_H
#define CUSTOMER_H

/*
    This file handles all customer related functions
*/

#define MAX_CUSTOMERS 300
#define MAX_ADDRESSES_FOR_CUSTOMERS 10
#define CUSTOMER_FILENAME "./data/customers.dat"

//---------------------------------------------------------------------------------//
// ------------------- Structure Definitions---------------------------------------//
//---------------------------------------------------------------------------------//

typedef struct
{
    int houseNumber;
    char streetName[100];
    char city[100];
    char postcode[7];
    char type[50];
    double latitude;
    double longitude;
} Address;

// Define the Customer struct
typedef struct
{
    char name[100];
    char phoneNumber[11]; // 10 digits + 1 for null terminator
    char emailId[100];
    Address addresses[MAX_ADDRESSES_FOR_CUSTOMERS]; // Array to hold multiple addresses
    int addressesCount;                             // Number of addresses for the customer
} Customer;

//---------------------------------------------------------------------------------//
// ------------------- Method Definitions------------------------------------------//
//---------------------------------------------------------------------------------//

int searchCustomerByPhoneNumber(const char *phoneNumber, Customer *result);
int saveCustomerToFile(Customer *customer);
int readCustomersFromFile(Customer *customers);
void collectAddressDetails(Address *address);
int collectCustomerDetails(Customer *customer);
void printCustomer(const Customer *customer);

//---------------------------------------------------------------------------------//
// ------------------- Method Implementation Starts -------------------------------//
//---------------------------------------------------------------------------------//

//
// Function to search for a customer record by phone number in the file
//
int searchCustomerByPhoneNumber(const char *phoneNumber, Customer *result)
{
    FILE *file = fopen(CUSTOMER_FILENAME, "rb"); // Open file in read binary mode
    if (file == NULL)
    {
        perror("Error opening file");
        printf("Error opening %s file\n", CUSTOMER_FILENAME);
        return -1;
    }
    int found = 0;
    Customer customer;
    while (fread(&customer, sizeof(Customer), 1, file) == 1)
    {
        if (strcmp(customer.phoneNumber, phoneNumber) == 0)
        {
            *result = customer; // Copy the found record to the result
            found = 1;
            break;
        }
    }
    fclose(file);
    return found ? 0 : -1; // Return 0 if found, -1 otherwise
}

//
// Function to save a customer record to a file
//
int saveCustomerToFile(Customer *customer)
{

    int retValue = -1;
    Customer searchResult;

    if (searchCustomerByPhoneNumber(customer->phoneNumber, &searchResult) == 0)
    {
        printf("\n ### customer with phonenumber %s already registered.\n", customer->phoneNumber);
        return retValue;
    }

    FILE *file = fopen(CUSTOMER_FILENAME, "a+b"); // Open file in append binary mode
    if (file == NULL)
    {
        perror("Error opening file\n");
        return retValue;
    }
    fwrite(customer, sizeof(Customer), 1, file); // Write the customer record to the file
    fclose(file);

    retValue = 0;

    return retValue;
}

//
// Function to read all customer records from a file
//
int readCustomersFromFile(Customer *customers)
{
    FILE *file = fopen(CUSTOMER_FILENAME, "rb"); // Open file in read binary mode
    if (file == NULL)
    {
        perror("Error opening file");
        return 0;
    }
    int count = 0;
    while (fread(&customers[count], sizeof(Customer), 1, file) == 1)
    {
        count++;
    }
    fclose(file);
    return count;
}

//
// Utility method to collect a customer addresses
//
void collectAddressDetails(Address *address)
{
    printf("Enter house number: ");
    scanf("%d", &address->houseNumber);
    printf("Enter street name: ");
    scanf(" %[^\n]", address->streetName); // %[^\n] reads input until newline is encountered
    printf("Enter city: ");
    scanf(" %[^\n]", address->city);
    printf("Enter postcode: ");
    scanf(" %6s", address->postcode); // %6s ensures that at most 6 characters are read
    printf("Enter address type (e.g., Home, Work): ");
    scanf(" %[^\n]", address->type);
    //printf("Enter latitude: ");
    //scanf("%lf", &address->latitude);
    address->latitude = 0.0;
    //printf("Enter longitude: ");
    //scanf("%lf", &address->longitude);
    address->longitude = 0.0;
}

//
// a utility method to collect customer details as part of registration
//
int collectCustomerDetails(Customer *customer)
{
    int retValue = -1;

    printf("Enter customer name: ");
    scanf(" %[^\n]", customer->name);

    while (1)
    {
        printf("Enter phone number: ");
        scanf(" %10s", customer->phoneNumber); // %10s ensures that at most 10 characters are read      

        if (isValidUsername(customer->phoneNumber) != 0)
        {
            printRed("Invalid phone number.  It has to 10 digits, only numbers");
            continue;
        }
        else
        {
            break;
        }
    }
    printf("Enter email ID: ");
    scanf(" %[^\n]", customer->emailId);

    printf("Enter number of addresses: ");
    scanf("%d", &customer->addressesCount);
    if (customer->addressesCount > MAX_ADDRESSES_FOR_CUSTOMERS)
    {
        printf("Number of addresses exceeds maximum limit. Setting to %d.\n", MAX_ADDRESSES_FOR_CUSTOMERS);
        customer->addressesCount = MAX_ADDRESSES_FOR_CUSTOMERS;
    }

    for (int i = 0; i < customer->addressesCount; i++)
    {
        printf("\nEnter details for address %d:\n", i + 1);
        collectAddressDetails(&customer->addresses[i]);
    }

    // Save the customer in customer db
    retValue = saveCustomerToFile(customer);

    return retValue;
}

//
// Utility method to print a given customer
//
void printCustomer(const Customer *customer)
{

    printf("Name: %s\n", customer->name);
    printf("Phone Number: %s\n", customer->phoneNumber);
    printf("Email ID: %s\n", customer->emailId);
    printf("Addresses:\n");
    for (int j = 0; j < customer->addressesCount; ++j)
    {
        printf("Address %d:\n", j + 1);
        printf("House Number: %d\n", customer->addresses[j].houseNumber);
        printf("Street Name: %s\n", customer->addresses[j].streetName);
        printf("City: %s\n", customer->addresses[j].city);
        printf("Postcode: %s\n", customer->addresses[j].postcode);
    }
    printf("\n");
}

#endif