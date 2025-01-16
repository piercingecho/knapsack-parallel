#define MAX_POSSIBLE_ITEMS_IN_FILE 64
#define MAX_ITEM_NAME_LENGTH 64
#define WORDY_OUTPUT 1
#define MAX_INTEGER_LINE_SIZE 128

#include "Item.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>


#define min(a,b)             \
({                           \
    __typeof__ (a) _a = (a); \
    __typeof__ (b) _b = (b); \
    _a < _b ? _a : _b;       \
})

#define CHECK_BIT(var,pos) ((var>>pos) & 1)



struct Item* read_knapsack_file(char filename[], int num_items_to_read, struct Item items[]);
void print_best_combo_output(int best_combo, struct Item items[], int num_items);
int calculate_combination_weight(struct Item items[], int items_length, int combination_id);
int calculate_combination_value(struct Item items[], int items_length, int combination_id);

struct Item* read_knapsack_file(char filename[], int num_items_to_read, struct Item items[])
{
    /*
    * Reads a file that contains data for the knapsack problem.
    *
    * Arguments:
    *   filename: the file to read. We assume stable user input in the following format:
            Item 1 Name
            item_1_weight
            item_1_value
            Item 2 Name
            item_2_weight
            item_2_value
            ...
    *   num_items_to_read: the number of items to read from the file. This MUST be lower
    *     than the number of items in the file.
    *   items: the data structure we store all items in. This should be a pre-initialized
    *     array with length equal to num_items_to_read.
    * 
    * Returns:
    *   A pointer back to items. Fills the empty array items with file data.
    */

    FILE *f = fopen(filename, "r");
    if(f == NULL) {
        printf("Unable to open file.\n");
        exit(1);
    }

    for(int i=0; i<num_items_to_read; i++)
    {
        char item_name[MAX_ITEM_NAME_LENGTH];
        int item_weight;
        int item_value;

        char temp_item_weight[MAX_INTEGER_LINE_SIZE];
        char temp_item_value[MAX_INTEGER_LINE_SIZE];

        //get item name
        fgets(item_name, MAX_ITEM_NAME_LENGTH, f); //read input until newline, discard the newline character.
        item_name[strcspn(item_name, "\n")] = 0; //remove trailing newline

        //get item weight
        fgets(temp_item_weight, MAX_INTEGER_LINE_SIZE, f);
        temp_item_weight[strcspn(temp_item_weight, "\n")] = 0; //remove trailing newline
        char *endpointer;
        item_weight = (int) strtol(temp_item_weight, &endpointer, 10); // convert to integer

        //get item value
        fgets(temp_item_value, MAX_INTEGER_LINE_SIZE, f);
        temp_item_value[strcspn(temp_item_value, "\n")] = 0; //remove trailing newline
        item_value = (int) strtol(temp_item_value, &endpointer, 10); // convert to integer

        // create item
        struct Item new_item = {"", item_weight, item_value};
        strncpy(new_item.name, item_name, MAX_ITEM_NAME_LENGTH);
        items[i] = new_item;
    }

    fclose(f);
    return items;

}

void print_best_combo_output(int best_combo, struct Item items[], int num_items)
{
    /*
    * Prints output that gives the best combination. Ignores WORDY_OUTPUT definition,
    * so guard this function with an if statement before calling it.
    * 
    * Arguments:
    *   best_combo: ID of the best knapsack combination.
    *   items: all items considered in the knapsack.
    *   num_items: the length of items array.
    * 
    * Returns:
    *   stdout describing the best combination.
    * 
    */

    printf("===\n");
    printf("%d\n", best_combo);

    // print the name of each item we have
    for(int i=0; i<num_items; i++)
    {
        if(CHECK_BIT(best_combo, i))
        {
            printf("%s\n", items[i].name);
        }
    }

    printf("%d\n", calculate_combination_weight(items, num_items, best_combo));
    printf("%d", calculate_combination_value(items, num_items, best_combo));
}

int calculate_combination_weight(struct Item items[], int items_length, int combination_id)
{
    int curr_weight = 0;
    for(int i=0; i<items_length; i++)
    {
        //if we have the specified item, then add its weight.
        if(CHECK_BIT(combination_id, i))
        {
            curr_weight += items[i].weight;
        }
    }

    return curr_weight;
}

int calculate_combination_value(struct Item items[], int items_length, int combination_id)
{
    int curr_value = 0;
    for(int i=0; i<items_length; i++)
    {
        //if we have the specified item, then add its value.
        if(CHECK_BIT(combination_id, i))
        {
            curr_value += items[i].value;
        }
    }

    return curr_value;
}