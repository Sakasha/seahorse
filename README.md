# Food Delivery System <br> -Seahorse

## Description
This project is a terminal-based food delivery system implemented in C programming Language. It allows users to browse a menu, sort, filter, place orders within a command-line interface. This Project Utilizes special algorithms such as Dijkstra's and Apriori Algorithm.

## Prerequisite Libraries
-  <stdio.h>
-  <stdlib.h>
-  <string.h>
-  <time.h>
-  <stdbool.h>
-  <limits.h>
-  <ctype.h>
-  <openssl/sha.h>
-  <errno.h>
-  <unistd.h>
    

## Modules
- login.h
- display-helper.h
- customer.h
- apriori.h
- dijkstra.h
- delivery-partner.h
- grocery.h
- order.h
- pet.h
- restaurant.h
- track.h
- utility.h
<br>
### order-manager.c
Order manager is the file which consist of the main function. To use the application one must run order-manager.Order-manager calls all other modules to present the final integrated food delivery system

### login
 The login module consists of functions to create user, register users, Verification of user credentials,logging in and Hashing passwords.

 ### display-helper
 The display-helper module consists of various display functions to present the operations (input and output) in a neat and organized manner, such as to collect orders, collect login details, displaying restaurant list, displaying items of a restaurant, filter and Sort methods

 ### customer
 The customer module consists of all customer related actions, such as reading and writing customer data into the repective file and to collect customer details to do so.

 ### apriori
 The apriori module consists of the functions to implement Apriori's algorithm which is a assosication mining algorithm. It is used for suggesting the most common companion item for the choosen item.

 ### dijkstra
 The dijkstra module consistsof the functions to implement Dijkstra's Algorithm which is used to calculate the shortest path.

 ### delivery-partner
The delivery partner module is to provide details on the person who will be delivering the food.

### grocery
The grocery module consist of the functions used in the grocery domain. Such as to add, view and select grocery items.

### pet
The pet module consist of the functions used in the pet food domain. Such as to add, view and select Pet food items.

 ### restaurant
 The restaurant module consists of all restaurant related actions, such as generating restaurant IDs, comparison of distance, rating and price, availabilty check, sort and filter functions.

 ### track
 This module tracks the delivery partner and displays the estimated time for delivery based on dijkstra's algorithm.

 ### utility
 The utility module consists of miscellaneous functions such as to print in red, blue, random alphanumeric generator, trim, captcha and lowercase conversion.


## Running the Code
The compiler used to compile the Food delivery System was **GCC C++/C compiler from mingw-w64 via MSYS2** to create programs that run on Windows
<br><br>
The command to compile the file
<br>

```
gcc order-manager.c -o order-manager.exe -lssl -lcrypto
```
## Version
The compiled code is of 18th version 

## Authors
  - Sathvika Kamali Shankar - 3122235001119
  - Shivaanee S.K. - 3122235001123
  - Shrika S.P. Thota - 3122235001125
