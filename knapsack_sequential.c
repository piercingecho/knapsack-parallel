#include "KnapsackFunctions.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#define FILE_NAME "data.txt"

int brute_force_combinations_sequential(struct Item items[], int num_items, int weight_limit, int start, int step);


int main(int argc, char* argv[]) 
{
    /*
    * Arguments:
    *   weight_limit
    *       The maximum weight the knapsack can hold. If no argument, default is given.
    *   max_num_items_available
    *       The maximum number of items to read from the file. If no argument, default is given.
    * 
    * We assume stable user input for both command line arguments and file data.
    */


    // convert arguments
    int weight_limit;
    int max_num_items_available;
    if(argc != 3)
    {
        printf("No argument. Using default values. Might get unexpected output.\n");
        weight_limit = 100;
        max_num_items_available = 64;
    }
    else
    {
        char *endpointer;
        weight_limit = (int) strtol(argv[1], &endpointer, 10);
        max_num_items_available = (int) strtol(argv[2], &endpointer, 10);
        // could check for error handling after each endpointer. We assume stable user input.
    }

    // setup to read knapsack file
    int num_items = min(max_num_items_available, MAX_POSSIBLE_ITEMS_IN_FILE);
    struct Item items[num_items];
    read_knapsack_file(FILE_NAME, num_items, items);

    int best_combo = brute_force_combinations_sequential(items, num_items, weight_limit, 0, 1);


    if(WORDY_OUTPUT)
    {
        print_best_combo_output(best_combo, items, num_items);
    }

    return 0;
}

int brute_force_combinations_sequential(struct Item items[], int num_items, int weight_limit, int start, int step)
{
    /*
    * Goes through each combination value for the knapsack, up to 2^num_items. We implement step motion 
    * so that we can evenly split this into multiple processes.
    * 
    * Arguments:
    *   items: the items considered for the knapsack problem.
    *   num_items: the length of the list items.
    *   weight_limit: max weight our knapsack can carry.
    *   start: the starting combination ID.
    *   step: the amount we should increment between each combination.
    * 
    * Returns:
    *   The ID of the best knapsack combination, from the ones tried.
    */

    int best_combo_id = -1;
    int best_combo_value = -1;

    int curr_combo_id = start;
    int max_combo_value = round(pow(2,num_items)); // handles floating point difficulties

    int curr_combo_value;
    int curr_combo_weight;
    for(curr_combo_id; curr_combo_id < max_combo_value; curr_combo_id += step)
    {
        curr_combo_weight = calculate_combination_weight(items, num_items, curr_combo_id);
        curr_combo_value = calculate_combination_value(items, num_items, curr_combo_id);
        
        if(WORDY_OUTPUT)
        {
            printf("%d ", curr_combo_id);
            for(int i=0; i<num_items; i++)
            {
                printf("%d ", CHECK_BIT(curr_combo_id, num_items - 1 - i));
            }
            printf("%d %d\n", curr_combo_weight, curr_combo_value);
        }

        // check if this is viable and the best choice
        if(curr_combo_weight <= weight_limit && curr_combo_value > best_combo_value)
        {
            best_combo_id = curr_combo_id;
            best_combo_value = curr_combo_value;
        }

    }
    return best_combo_id;
}

