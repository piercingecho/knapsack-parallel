# Knapsack Problem in Parallel

This is a survey into parallel C libraries using the knapsack problem.

### Instructions to Run Program

In the terminal, run:
`gcc knapsack_sequential.c -o knapsack.exe`

This creates an executable with the name knapsack.exe in the directory. Run it in the command line with the format:
`./knapsack.exe <integer1> <integer2>`

where integer1 is the weight limit of the knapsack and integer2 is the number of items to read from the file.

Currently, the maximum number the program allows is 64 items. 
Feel free to change the line `#define MAX_POSSIBLE_ITEMS_IN_FILE 64` in `knapsack_sequential.c` to update this.

### Instructions for Data File
Please reference the file with example data, data.txt for formatting instructions.
Each item should take 3 lines total, with the following format:

```
<Name (string)>
<Weight (integer)>
<Value (integer)>
```

such as

```
diamond ring
5
100
```

Feel free to change the line `#define FILE_NAME "data.txt"` in `knapsack_sequential.c` to a different file name.