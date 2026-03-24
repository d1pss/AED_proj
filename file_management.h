#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>

#ifndef FILE_MANAMEGENT
#define FILE_MANAMEGENT


    /****************************************************************************************************
     * @brief Allocates memmory to the struct.
     * 
     * @param problem Pointer to the struct with the current problem information.
     * @return (Problem*) The pointer to the struct with the memmory allocated to it self.
     ****************************************************************************************************/
    Problem* allocate_struct_problem(Problem* problem);

    /****************************************************************************************************
     * @brief Allocates memmory to the struct.
     * 
     * @param info Pointer to the struct with the current problem mini map information.
     * @return (M_map_info*) The pointer to the struct with the memmory allocated to it self.
     ****************************************************************************************************/
    M_map_info* allocate_struct_m_map_info(M_map_info* info);

    /****************************************************************************************************
     * @brief Checks if the file name ends in .1maps and opens file in.
     * 
     * @param input_file_name Name of the input file.
     * @return (FILE*) The pointer to the file in.
     ****************************************************************************************************/
    FILE* file_in_open(char* input_file_name);

    /****************************************************************************************************
     * @brief Creates the output file name based on the input file name and opens file out.
     * 
     * @param input_file_name Name of the input file.
     * @param file_in Input file pointer.
     * @return (FILE*) The pointer to the file out.
     ****************************************************************************************************/
    FILE* file_out_open(char* input_file_name, FILE* file_in);

#endif

