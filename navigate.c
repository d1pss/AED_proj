/******************************************************************************************
 * @file navigate.c
 * @author Aline Cardoso Nº 109443
 *         Rodrigo Oliveira Nº 109317  
 * 
 ******************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdbool.h>

#include "tarefas.h"
#include "file_management.h"
#include "array_management.h"

/******************************************************************************************
 * @brief Finds paths for a given map in two difrent ways.
 * 
 ******************************************************************************************/
int main (int argc, char* argv[]){
    if(argc != 2)
    {//check if we only recive 2 arguments if we recive extra or less exit the program
        return 0;
    }
    FILE *file_in = NULL, *file_out = NULL;
    char *input_file_name = argv[1];

    file_in = file_in_open(input_file_name);
    
    file_out = file_out_open(input_file_name, file_in);
    
    solve_problems(file_in, file_out);

    fclose(file_in);
    fclose(file_out);

    return 0; 
}
