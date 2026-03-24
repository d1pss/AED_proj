#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>

#include "tarefas.h"

#ifndef ARRAY_MANAMEGENT
#define ARRAY_MANAMEGENT

/****************************************************************************************************
 * @brief Calculates the boundaries and dimensions of the mini-map, ensuring it remains within the 
 *        bounds of the main map, based on the start position and step range.
 * 
 * @param info Struct storing the mini-map boundary and dimension information.
 * @param problem Struct storing the current problem's mini map information.
 ****************************************************************************************************/
void Get_mini_map_info(M_map_info* info, Problem* problem);

/****************************************************************************************************
 * @brief Allocates memory for the 2D arrays that store the energy values of the mini-map and the 
 *        boolean array that tracks visited cells. Initializes the visited array to false.
 * 
 * @param info Struct storing the mini-map boundary and dimension information.
 * @param problem Struct storing the current problem's mini map information.
 ****************************************************************************************************/
void alloc_mini_maps(M_map_info* info, Problem* problem);

/****************************************************************************************************
 * @brief Initializes the 2D boolean array of the mini-map, setting all values to false, indicating 
 *        that no cell has been visited initially.
 * 
 * @param info Struct storing the mini-map boundary and dimension information.
 * @param problem Struct storing the current problem's mini map information.
 ****************************************************************************************************/
void init_bool_array(M_map_info* info, Problem* problem);

/****************************************************************************************************
 * @brief Reads energy values from the input file and populates the mini-map with only the values 
 *        within the computed bounds of the mini-map.
 * 
 * @param problem Struct storing the current problem's mini map information.
 * @param info Struct storing the mini-map boundary and dimension information.
 * @param file_in Input file pointer used to read map data.
 ****************************************************************************************************/
void fill_up_mini_map(Problem* problem, M_map_info* info, FILE* file_in);

/****************************************************************************************************
 * @brief Frees the memory allocated for the 2D arrays that store the energy values and the visited 
 *        status of each cell in the mini-map.
 * 
 * @param info Struct storing the mini-map boundary and dimension information.
 * @param problem Struct storing the current problem's mini map information.
 ****************************************************************************************************/
void free_mini_maps(M_map_info* info, Problem* problem);





/****************************************************************************************************
 * @brief Reads the energy values within a diamond-shaped area of the mini-map, based on the center 
 *        coordinates and number of steps. Stores only positive energy values that haven't been visited.
 * 
 * @param problem Struct storing the current problem's mini map information.
 * @param info Struct storing the mini-map boundary and dimension information.
 * @param array_positive_energys Array to store the positive energy values found in the mini-map.
 * @param max_size_array Integer array tracking the number of elements added to array_positive_energys.
 * @param steps Number of steps defining the radius of the diamond-shaped area.
 * @param center_srow Row index of the center point.
 * @param center_scol Column index of the center point.
 ****************************************************************************************************/
void read_diamond_and_save_energys(Problem* problem, M_map_info* info, int* array_positive_energys, int max_size_array[1], int steps, int center_srow, int center_scol);

/****************************************************************************************************
 * @brief Calculates the sum of the highest energy values within a diamond-shaped area of the mini-map.
 *        Uses the number of steps to determine how many energy values to sum. 
 * 
 * It applies Quickselect to partially sort the array. Quickselect efficiently finds the k largest 
 * energy values without fully sorting the array, ensuring that only the k largest values are summed.
 * 
 * @param problem Struct storing the current problem's mini map information.
 * @param info Struct storing the mini-map boundary and dimension information.
 * @param array_positive_energys Array to store the positive energy values found in the mini-map.
 *                               But we alloc it out of this function to save memory.
 * @param steps Number of steps defining the radius of the diamond-shaped area and the number of energies to sum.
 * @param curr_row Current row index to calculate energy sum from.
 * @param curr_col Current column index to calculate energy sum from.
 * @return (int) Returns the sum of the best energy values within the diamond-shaped area.
 ****************************************************************************************************/
int get_sum_max_energy(Problem* problem, M_map_info* info, int* array_positive_energys, int steps, int curr_row, int curr_col);





/****************************************************************************************************
 * @brief Allocates memory and initializes the stack, which is a 2D array of (2*k+2) positions. 
 *        Each position holds two integers representing coordinates of cells to be analyzed. 
 *        It also initializes the stack with default values where the first element is set to -1 
 *        to indicate that the position is empty.
 * 
 * @param stack Pointer to the stack (2D array) that will store coordinates to be processed.
 * @param problem Struct storing the current problem's mini map information.
 * @return (int**) Returns the pointer to the initialized stack.
 ****************************************************************************************************/
int** alloc_and_initialize_stack(int** stack, Problem* problem);


/****************************************************************************************************
 * @brief Removes the last element added to the stack (LIFO Last In, First Out) by copying its 
 *        coordinates to 'curr_cell' and setting the stack position to -1 to mark it as empty.
 * 
 * @param stack Pointer to the stack that stores coordinates to be processed.
 * @param curr_cell An array of 2 elements where the coordinates removed from the stack are stored.
 * @param stack_pos The position of the first free slot in the stack.
 * @return (int) Returns the updated stack_pos, which is decremented by 1 to reflect the 
 *               removal of the last element.
 ****************************************************************************************************/
int remove_first_in_stack(int** stack, int curr_cell[2], int stack_pos);


/****************************************************************************************************
 * @brief Adds a new set of coordinates to the first free position in the stack (LIFO Last In, First Out).
 * 
 * @param stack Pointer to the stack that stores coordinates to be processed.
 * @param cords_to_add An array of 2 elements containing the coordinates to add to the stack.
 * @param stack_pos The position of the first free slot in the stack.
 * @return (int) Returns the updated stack_pos, which is incremented by 1 to reflect the 
 *               addition of a new element.
 ****************************************************************************************************/
int add_in_stack(int** stack, int* cords_to_add, int stack_pos);


/****************************************************************************************************
 * @brief Adds a pair of coordinates (x, y) to the first available slot in the LC_array. 
 *        This array temporarily holds coordinates before they are added to the stack.
 * 
 * @param LC_array A 2D array that temporarily stores up to 4 sets of coordinates.
 * @param x The x-coordinate (row index) to be added to the array.
 * @param y The y-coordinate (column index) to be added to the array.
 ****************************************************************************************************/
void add_to_LC_array(int LC_array[][2], int x, int y);


/****************************************************************************************************
 * @brief After sorting the LC_array, this function adds its contents to the stack in reverse order. 
 *        This ensures that the best coordinates (the first in LC_array) are added last, so they 
 *        are processed first (since the stack follows LIFO behavior).
 * 
 * @param LC_array A 2D array that temporarily stores up to 4 sets of coordinates that are now sorted.
 * @param stack The stack that stores coordinates to be processed.
 * @param stack_pos The position of the first free slot in the stack.
 * @return (int) Returns the updated stack position after adding all elements of LC_array.
 ****************************************************************************************************/
int remove_from_LC_array_and_add_to_stack(int LC_array[][2], int** stack, int stack_pos);


/****************************************************************************************************
 * @brief Sorts the LC_array by evaluating the total potential energy at each coordinate, calculated 
 *        as the cell's energy plus the maximum energy that can be collected from neighboring cells.
 *        The array is sorted from highest to lowest potential energy. It also removes paths where 
 *        the energy is insufficient to reach the objective or if the current energy is non-positive.
 * 
 * @param LC_array A 2D array that temporarily stores up to 4 sets of coordinates to be sorted.
 * @param ob_energy Objective energy or best energy found so far (depends on the task).
 * @param curr_step The number of steps available to reach each coordinate in LC_array.
 * @param curr_energy The current accumulated energy before reaching the coordinates in LC_array.
 * @param problem Struct storing the current problem's mini map information.
 * @param info Struct storing the mini-map boundary and dimension information.
 * @return (int) Returns the number of valid elements remaining in LC_array after sorting.
 ****************************************************************************************************/
int sort_LC_array(int LC_array[][2], int* ob_energy, int curr_step, int curr_energy, Problem* problem, M_map_info* info);


/****************************************************************************************************
 * @brief Swaps the coordinates at positions i and j in the LC_array. It also swaps the associated 
 *        maximum energy values in the max_energys array to maintain synchronization.
 * 
 * @param LC_array Array of coordinates to be swapped.
 * @param max_energys Array of maximum energy values corresponding to each coordinate in LC_array.
 * @param i Index of the first coordinate to swap.
 * @param j Index of the second coordinate to swap.
 ****************************************************************************************************/
void swap_LC_array_and_max_energys(int LC_array[][2], int max_energys[4], int i, int j);


/****************************************************************************************************
 * @brief Checks whether the given coordinates (x, y) are within the bounds of the mini-map. 
 * 
 * @param x The x-coordinate (row index) to check.
 * @param y The y-coordinate (column index) to check.
 * @param info Struct storing the mini-map boundary and dimension information.
 * @return (int) Returns 1 if the coordinates are in bounds, otherwise returns 0.
 ****************************************************************************************************/
int check_inbound_cords(int x, int y, M_map_info* info);


/****************************************************************************************************
 * @brief Frees all dynamically allocated memory for the stack. It iterates through all positions 
 *        in the stack and frees each one individually to prevent memory leaks.
 * 
 * @param stack Pointer to the stack that stores coordinates to be processed.
 * @param problem Struct storing the current problem's mini map information.
 ****************************************************************************************************/
void free_stack(int** stack, Problem* problem);





/****************************************************************************************************
 * @brief Allocates memory for a 2D array (way) that holds the coordinates of cells visited 
 *        during the traversal. The array has 'size' positions, each capable of storing 
 *        a pair of integers (coordinates).
 * 
 * @param way Pointer to the 2D array that will be allocated and initialized.
 * @param size The size is the number of steps we can take, defined by problem->steps.
 * @return (int**) Returns a pointer to the allocated way array.
 ****************************************************************************************************/
int** alloc_and_init_way(int** way, int size);


/****************************************************************************************************
 * @brief Frees the memory allocated for both the auxiliary way and the correct way arrays. 
 *        This ensures that all dynamically allocated positions are freed, preventing memory leaks.
 * 
 * @param aux_way Pointer to the auxiliary way array that needs to be freed.
 * @param correct_way Pointer to the correct way array that needs to be freed.
 * @param size The size is the number of steps we can take, defined by problem->steps.
 ****************************************************************************************************/
void free_ways(int** aux_way, int** correct_way, int size);


/****************************************************************************************************
 * @brief Removes the last point added to the auxiliary way array. This operates in LIFO order 
 *        (Last In, First Out) by setting the last inserted coordinates to -1, indicating 
 *        that this position is now empty.
 * 
 * @param way Pointer to the auxiliary way array from which the coordinates are removed.
 * @param way_pos The position of the first free slot in the auxiliary way.
 * @return (int) Returns the updated way_pos, which is decremented by 1 to reflect the 
 *               removal of the last element.
 ****************************************************************************************************/
int remove_cords_from_aux_way(int** way, int way_pos);


/****************************************************************************************************
 * @brief Adds the current coordinates to the first free position set by the way_pos in the auxiliary
 *        way array. 
 * 
 * @param way Pointer to the auxiliary way array where the coordinates will be added.
 * @param way_pos The position of the first free slot in the auxiliary way.
 * @param curr_cell An array of 2 elements containing the current coordinates to be added 
 *                  to the auxiliary way.
 * @return (int) Returns the updated way_pos, which is incremented by 1 to reflect the 
 *               addition of a new element.
 ****************************************************************************************************/
int add_cords_to_aux_way(int** way, int way_pos, int* curr_cell);


/****************************************************************************************************
 * @brief Copies all coordinates from the auxiliary way array to the correct way array. 
 * 
 * @param aux_way Pointer to the auxiliary way array containing the coordinates to be copied.
 * @param correct_way Pointer to the correct way array where the coordinates will be copied.
 * @param size The size is the number of steps we can take, defined by problem->steps.
 ****************************************************************************************************/
void copy_way(int** aux_way, int** correct_way, int size);





/****************************************************************************************************
 * @brief Swaps the integers located at the memory addresses pointed to by 'a' and 'b'.
 * 
 * @param a Pointer to the first integer to be swapped.
 * @param b Pointer to the second integer to be swapped.
 ****************************************************************************************************/
void swap(int* a, int* b);


/****************************************************************************************************
 * @brief Partitions the array using the first element as the pivot. This function rearranges the
 *        elements in the array such that all elements less than or equal to the pivot are on the
 *        left side and all elements greater than the pivot are on the right side. The pivot is
 *        then placed in its correct position in the sorted order.
 * 
 * @param arr The array to be partitioned.
 * @param low The starting index of the portion of the array to partition.
 * @param high The ending index of the portion of the array to partition.
 * @return (int) Returns the index of the pivot element after partitioning.
 ****************************************************************************************************/
int partition(int arr[], int low, int high);


/****************************************************************************************************
 * @brief Performs the Quickselect algorithm, which partially sorts the array to find the k largest 
 *        elements. It selects a pivot element, partitions the array around the pivot, and then 
 *        recursively narrows down the search range based on the position of the pivot relative to 
 *        'k'. After execution, the k largest elements will be located at the end of the array, 
 *        but the array itself is not fully sorted.
 * 
 * @param arr The array to be partially sorted.
 * @param low The starting index of the portion of the array to consider.
 * @param high The ending index of the portion of the array to consider.
 * @param k The number of largest elements to find. It should be a 1-based index, meaning that k = 1 
 *          corresponds to the largest element.
 ****************************************************************************************************/
void quickSelect(int arr[], int low, int high, int k);


#endif