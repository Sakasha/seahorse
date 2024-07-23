	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include <time.h>
	#include "../utility.h"
	#include "../restaurant.h"
	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include <stdbool.h>

	#define MAX_LINE_LENGTH 600
	//#define MAX_ITEMS 12


	// Function to print restaurant data
	void printInputRestaurantData(const Restaurant *restaurant)
	{
		printf("Restaurant Name: %s\n", restaurant->name);
		printf("Restaurant Location: %s\n",restaurant->location);
		printf("Restaurant ID: %s\n", restaurant->ID);
		//printf("Cuisine: %s\n", restaurant->cuisine);
		printf("Opening Hours: %s\n", restaurant->time);
		printf("Rating: %.2f\n", restaurant->rating);
		printf("Veg: %s\n", restaurant->veg ? "Yes" : "No");
		printf("Menu Items:\n");
		for (int i = 0; i < MAX_ITEMS; i++)
		{
		   
			const Item *item = &restaurant->items[i];

			 if(item->price < 1 ) {
				break;
			 }
			printf("  Name: %s, ID: %s, Allergen: %s, Availability: %s, Price: %.2f\n",
				   item->name, item->ID, item->allergen, item->availability ? "Yes" : "No", item->price);
		}
		printf("\n");
	}


	// Function to generate random float number
	float randomFloat(float min, float max)
	{
		return ((float)rand() / RAND_MAX) * (max - min) + min;
	}

	void split_line(char *line, Restaurant *restaurant, int *item_index)
	{

		// Retaurant Id is not part of the input from data file, so add it manually
		generateID(restaurant->ID, 7, 'R');

		// Split the line read from the file with comma as delimiter
		char *token = strtok(line, ",");

		// 1st  item should be "restaurant name" in the line read from the file
		if (token != NULL)
		{
			strncpy(restaurant->name, token, sizeof(restaurant->name) - 1);
			restaurant->name[sizeof(restaurant->name) - 1] = '\0';
		}

		// 2nd item should be "restaurant location" in the line read from the file
		token = strtok(NULL, ",");
		if (token != NULL)
		{
			strncpy(restaurant->location, token, sizeof(restaurant->location) - 1);
			restaurant->location[sizeof(restaurant->location) - 1] = '\0';
		}


		// 3rd item should be "restaurant rating" in the line read from the file
		token = strtok(NULL, ",");
		if (token != NULL)
		{
			restaurant->rating = strtof(token, NULL);
		}

		// 4th item should be "veg or non-veg choice" in the line read from the file
		token = strtok(NULL, ",");
		if (token != NULL)
		{
			printf("### token : %s, strcmp(token, 'V') : %d \n", token, strcmp(token, "V"));

			if( strcmp(token, "V") == 0 ) {
				restaurant->veg = true;
			} else {
				restaurant->veg = false;
			}
		}


		
		token = strtok(NULL, ",");
		while (token != NULL)
		{

			// Item Id is not part of the input from data file, so add it manually
			generateID(restaurant->items[*item_index].ID, 7, 'I');
			restaurant->items[*item_index].availability = true;


			// strcpy(restaurant->items[*item_index].allergen, "none");

			// Extract allergen information from item string
			char *allergen_start = strchr(token, '(');
			char *allergen_end = strchr(token, ')');


			if (allergen_start != NULL && allergen_end != NULL)
			{
				strncpy(restaurant->items[*item_index].allergen, allergen_start + 1, allergen_end - allergen_start - 1);
				restaurant->items[*item_index].allergen[allergen_end - allergen_start - 1] = '\0';

				// Remove allergen information from item string
				*allergen_start = '\0';
				
			}
			else
			{
				strcpy(restaurant->items[*item_index].allergen, "None");
			}

			// Price is not part of the input data read from file, so randomly generate and assign
			float price = randomFloat(5.0, 20.0);
			restaurant->items[*item_index].price = price > 0 ? price : 5.0f;

			strncpy(restaurant->items[*item_index].name, token, sizeof(restaurant->items[*item_index].name) - 1);
			restaurant->items[*item_index].name[sizeof(restaurant->items[*item_index].name) - 1] = '\0';

			(*item_index)++;
			token = strtok(NULL, ",");
		}
	}

	int main()
	{

		srand(time(NULL));

		FILE *inputFile = fopen("./restaurant-input-data.txt", "r");
		FILE *outputFile = fopen("./../data/restaurants.dat", "wb");
		if (inputFile == NULL || outputFile == NULL)
		{
			perror("Error opening file");
			return EXIT_FAILURE;
		}

		char line[MAX_LINE_LENGTH];

		int count = 1;

		while (fgets(line, sizeof(line), inputFile))
		{
			Restaurant restaurant;
			int item_index = 0;
			// Remove newline character if present
			line[strcspn(line, "\n")] = '\0';

			split_line(line, &restaurant, &item_index);
			fwrite(&restaurant, sizeof(Restaurant), 1, outputFile);
			printf("### %d ###\n", count);
			count++;
			printInputRestaurantData(&restaurant);
		}

		fclose(inputFile);
		fclose(outputFile);

		printf("Data has been successfully written to output.bin\n");

		return 0;
	}
