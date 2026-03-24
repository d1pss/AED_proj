#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>

#ifndef TAREFAS
#define TAREFAS

typedef struct _Problem{
    int rows, columns, ob_energy, srows, scolumns, steps, init_energy;
    int** mini_map;
    bool** visited; 
}Problem;

typedef struct m_map_info{
    int mini_rows, mini_columns, mini_srow, mini_scol, n_out_of_bounds_srow, 
        n_out_of_bounds_erow, n_out_of_bounds_ecolumn, n_out_of_bounds_scolumn, 
        mini_map_scolumn, mini_map_ecolumn, mini_map_srow, mini_map_erow;
}M_map_info;

/****************************************************************************************************
 * @brief Reads and processes multiple problems from an input file, validates each problem, and 
 *        solves it using the `tasks` function. For invalid problems, it skips processing and writes 
 *        the header to the output file.
 * 
 * The function loops through all problems in the input file, extracts their details, checks for 
 * validity, and either allocates resources to solve the problem or skips invalid ones. After solving, 
 * results are written to the output file, and memory is freed.
 * 
 * @param file_in Input file pointer for reading problems.
 * @param file_out Output file pointer for writing results.
 ****************************************************************************************************/
void solve_problems(FILE* file_in, FILE* file_out);


/****************************************************************************************************
 * @brief Solves the current problem using a branch-and-bound Depth First Search (DFS) algorithm to 
 *        find a solution. It handles two cases:
 *        1. Task 1: Finds a path where the accumulated energy matches or overcomes the objective energy.
 *        2. Task 2: Finds the path that maximizes energy within a limited number of steps.
 * 
 * The function allocates memory for stacks and paths, adjusts initial conditions based on the problem 
 * type, calls `recursive_BB_DFS` to explore paths, and writes the result to the output file, including 
 * the solution path if one is found. It cleans up all allocated memory before returning.
 * 
 * @param problem Struct containing problem details.
 * @param info Struct with mini-map information.
 * @param file_out Output file pointer for writing results.
 ****************************************************************************************************/
void tasks(Problem* problem, M_map_info* info, FILE* file_out);


/****************************************************************************************************
 * @brief This function implements a branch-and-bound Depth First Search (DFS) algorithm. It 
 *        recursively explores all possible paths within a given mini-map, attempting to find the 
 *        optimal path that either meets a specific energy objective (Task 1) or maximizes the energy 
 *        collected (Task 2). It leverages bounding techniques to prune unnecessary branches based on 
 *        the potential future energy gain from the current state.
 * 
 * In Task 1, the algorithm tries to find a path that exactly reaches or overcomes the specified objective energy. 
 * In Task 2, it tracks the best energy found so far and aims to maximize the total energy. 
 * Pruning occurs when the current energy and the estimated maximum energy that can be collected 
 * from future steps are insufficient to reach the objective or further exploration is unnecessary.
 * 
 * The function utilizes auxiliary structures like a stack and arrays to track visited nodes, 
 * store the current path, and explore adjacent nodes in a specific order based on energy potential.
 * 
 * @param problem Is a struct that stores the current problem information, including the mini-map, 
 *                visited status of cells, and energy values.
 * @param info Is a struct that stores the mini-map dimensions and other map-related data.
 * @param stack A pointer to the stack array that stores coordinates of cells to be processed next.
 * @param aux_way A pointer to the auxiliary path array, storing the current path being explored.
 * @param correct_way A pointer to the array that stores the best/optimal path found so far.
 * @param stack_pos A pointer to the position of the first free slot in the stack.
 * @param curr_row The row coordinate of the current cell being processed.
 * @param curr_col The column coordinate of the current cell being processed.
 * @param curr_energy The total energy accumulated so far, not including the current cell's energy.
 * @param curr_step The number of steps remaining for exploration.
 * @param ob_energy In Task 1, this holds the target energy to be reached. In Task 2, it holds 
 *                  the best energy found so far.
 * @param way_pos The position of the first free slot in the current path (aux_way).
 * @return (int) In Task 1, returns the energy if the objective is met, otherwise -1. In Task 2, 
 *               always returns -1, and the best energy is stored in the ob_energy.
 ****************************************************************************************************/
int recursive_BB_DFS(Problem* problem, M_map_info* info, int** stack, int** aux_way, int** correct_way, int stack_pos[1], int curr_row, int curr_col, int curr_energy, int curr_step, int ob_energy[1], int way_pos[1]);

#endif