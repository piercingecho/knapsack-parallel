// Compile instructions:
// gcc -fopenmp knapsack_openmp.c -o exe_name.exe

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>
#include "knapsack_sequential.c"
    //has #include "Item.h"

#define NUM_THREADS 8

int brute_force_combinations_parallel(struct Item items[], int num_items, int weight_limit, int num_threads);

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


    int best_combo = brute_force_combinations_parallel(items, num_items, weight_limit, NUM_THREADS);


    if(WORDY_OUTPUT)
    {
        print_best_combo_output(best_combo, items, num_items);
    }

    return 0;
}

int brute_force_combinations_parallel(struct Item items[], int num_items, int weight_limit, int num_threads)
{
    /*
    * Goes through each combination value for the knapsack, up to 2^num_items. Each thread will be assigned
    * an equal portion of the work.
    * 
    * Arguments:
    *   items: the items considered for the knapsack problem.
    *   num_items: the length of the list items.
    *   weight_limit: max weight our knapsack can carry.
    *   num_threads: the number of threads to solve the problem in parallel.
    * 
    * Returns:
    *   The ID of the best knapsack combination, from the ones tried.
    */

    // final variables
    int best_combo_id = -1;
    int best_combo_value = -1;
    
    // for stop condition
    int max_combo_value = round(pow(2,num_items)); // handles floating point difficulties
    int chunk_size = max_combo_value / NUM_THREADS;
    
    if(WORDY_OUTPUT)
    {
        printf("Total Combinations To Test: %d\n", max_combo_value);
    }


    /* Create a set of threads to complete problem. Share the best version, keep local bests so we don't have to mutex until the end. */
    #pragma omp parallel \
        shared(best_combo_id, best_combo_value, chunk_size) \
        num_threads(NUM_THREADS)
 	{
        // for threads
        int tid;

        // best values. Must initialize value in case we don't find a better solution.
        int thread_best_combo_id; 
        int thread_best_value = -1; 

        // for iterating
        int curr_combo_id;
        int start_combo_id; // test <chunk_size> combinations
        int end_combo_id;
        int curr_combo_value;
        int curr_combo_weight;



		tid = omp_get_thread_num();
        
        omp_set_num_threads(NUM_THREADS);
        start_combo_id = tid * chunk_size;
        end_combo_id = (tid + 1) * chunk_size;
   		for (curr_combo_id=start_combo_id; 
             curr_combo_id < end_combo_id; 
             curr_combo_id++)
		{
            // first calculate the best one from its assigned values
            curr_combo_weight = calculate_combination_weight(items, num_items, curr_combo_id);
            curr_combo_value = calculate_combination_value(items, num_items, curr_combo_id);
            
            if(WORDY_OUTPUT)
            {
                printf("Thread %d: Combo %d\n", tid, curr_combo_id);
            }

            if(curr_combo_weight <= weight_limit && curr_combo_value > thread_best_value)
            {
                thread_best_combo_id = curr_combo_id;
                thread_best_value = curr_combo_value;
            }

		}


        // Lock variables from being accessed
        #pragma omp critical (global_best_combo_lock)
        {
            /*
            We already know the best combo is good for the weight limit. So we just check value.
            This works with the edge case of no combination worked by the thread being valid because we initialize thread_best_value to -1.
            */
            if(thread_best_value > best_combo_value)
            {
                best_combo_id = thread_best_combo_id;
                best_combo_value = curr_combo_value;
            }
            else if(thread_best_value == best_combo_value && thread_best_combo_id < best_combo_id)
            {
                best_combo_id = thread_best_combo_id;
            }
        }

        // end lock

   	}   /* end of parallel region */

    return best_combo_id;
}
