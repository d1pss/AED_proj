#include "tarefas.h" 
#include "file_management.h"
#include "array_management.h"


void solve_problems(FILE* file_in, FILE* file_out){
    M_map_info* info = NULL;
    Problem* problem = NULL;
    int no_save = 0;
    no_save++;

    problem = allocate_struct_problem(problem);    
    
    info = allocate_struct_m_map_info(info);

    //while there is still problems, solve them
    while ((fscanf(file_in,"%d", &problem->rows)) == 1){
        
        //get the information in the header of the problem
        no_save = fscanf(file_in,"%d", &problem->columns);
        no_save = fscanf(file_in,"%d", &problem->ob_energy);
        no_save = fscanf(file_in,"%d", &problem->srows);     
        no_save = fscanf(file_in,"%d", &problem->scolumns);
        no_save = fscanf(file_in,"%d", &problem->steps);
        no_save = fscanf(file_in,"%d", &problem->init_energy);

        //check for invalid problem
        if((problem->srows > problem->rows) || (problem->scolumns > problem->columns) || (problem->srows < 1) || (problem->scolumns < 1) || (problem->steps <= 0) ||
           (problem->steps > (problem->columns * problem->rows)) ||(problem->ob_energy < 1 && problem->ob_energy != -2) || (problem->init_energy < 1)){
        
            for(int i = problem->rows * problem->columns; i != 0; i--)
            {//travel trought the map so the file pointer ends in the begining of the next problem
                no_save = fscanf(file_in, "%*d");
            }
            //in this special case the problem is valid with allways no solution so in case we have a very hight k we dont process it
            if(problem->steps > (problem->columns * problem->rows)){
                fprintf(file_out, "%d %d %d %d %d %d %d -1\n\n",problem->rows, problem->columns, problem->ob_energy, problem->srows, problem->scolumns, problem->steps, problem->init_energy);
                continue;
            }

            fprintf(file_out, "%d %d %d %d %d %d %d\n\n",problem->rows, problem->columns, problem->ob_energy, problem->srows, problem->scolumns, problem->steps, problem->init_energy);
            continue;   
        }

        //get dimension of mini map
        Get_mini_map_info(info, problem);
        //alloc mini map
        alloc_mini_maps(info, problem);
        //fill up mini map
        fill_up_mini_map(problem, info, file_in);
        //solve the problem
        tasks(problem, info, file_out);
        
        
    }

    //free all allocated memory to ensure no memory leaks
    free(problem);
    free(info);
    return;
}    

int recursive_BB_DFS(Problem* problem, M_map_info* info, int** stack, int** aux_way, int** correct_way, int stack_pos[1], int curr_row, int curr_col, int curr_energy, int curr_step, int ob_energy[1], int way_pos[1]){
    int LC_array[4][2] = {{-1, 0}, {-1, 0}, {-1, 0}, {-1, 0}}, size_LC, answer = -1, next_cell[2] = {curr_row, curr_col};;

    //add the cell energy to the current energy
    curr_energy += problem->mini_map[curr_row][curr_col];

    //if there are no more steps 
    if(curr_step == 0){
        //check if that way energy matches or overcomes the objective
        if(curr_energy >= ob_energy[0]){
            //store the valid path
            copy_way(aux_way, correct_way, problem->steps);
            
            //check if its task 2
            if(problem->ob_energy == -2){
                //set the new max energy as the current energy
                ob_energy[0] = curr_energy;
                return -1;
            }
            return curr_energy;
        }
        return -1;
    }
    
    //decrement the step
    curr_step--;
    
    //check for all directions if the cords are valid and add them to the LC_array
    //up
    if(check_inbound_cords(curr_row-1, curr_col, info) && problem->visited[curr_row-1][curr_col] == false){
        if(problem->mini_map[curr_row-1][curr_col] + curr_energy > 0){
            add_to_LC_array(LC_array, curr_row-1, curr_col);
        }
    }
    //right
    if(check_inbound_cords(curr_row, curr_col+1, info) && problem->visited[curr_row][curr_col+1] == false){
        if(problem->mini_map[curr_row][curr_col+1] + curr_energy > 0){
            add_to_LC_array(LC_array, curr_row, curr_col+1);
        }
    }
    //down
    if(check_inbound_cords(curr_row+1, curr_col, info) && problem->visited[curr_row+1][curr_col] == false){
        if(problem->mini_map[curr_row+1][curr_col] + curr_energy > 0){
            add_to_LC_array(LC_array, curr_row+1, curr_col);
        }    
    }
    //left
    if(check_inbound_cords(curr_row, curr_col-1, info) && problem->visited[curr_row][curr_col-1] == false){
        if(problem->mini_map[curr_row][curr_col-1] + curr_energy > 0){
            add_to_LC_array(LC_array, curr_row, curr_col-1);
        }
    }

    //sort the LC_array puting the cords with the better potential energy first and removing cords that even with the max potincial dont reach the objective
    size_LC = sort_LC_array(LC_array, ob_energy, curr_step, curr_energy, problem, info);

    //add the LC_array to the queue
    stack_pos[0] = remove_from_LC_array_and_add_to_stack(LC_array, stack, stack_pos[0]);

    for(int i = 0; i < size_LC; i++){
        //pop the next element
        stack_pos[0] = remove_first_in_stack(stack, next_cell, stack_pos[0]);
        //add the element to the current way
        way_pos[0] = add_cords_to_aux_way(aux_way, way_pos[0], next_cell);
        //mark it as visited
        problem->visited[next_cell[0]][next_cell[1]] = true;

        //call the function again
        answer = recursive_BB_DFS(problem, info, stack, aux_way, correct_way, stack_pos, next_cell[0], next_cell[1], curr_energy, curr_step, ob_energy, way_pos);
        
        //remove the point from the current way
        way_pos[0] = remove_cords_from_aux_way(aux_way, way_pos[0]);
        //mark it as non visited
        problem->visited[next_cell[0]][next_cell[1]] = false;
        //if in case of task 1 we have found the answer exit the function by breaking out of all for loops
        if(answer != -1){
            break;
        }
    }

    return answer;
}

void tasks(Problem* problem, M_map_info* info, FILE* file_out){
    int** stack = NULL, **aux_way = NULL, **correct_way = NULL;
    //Initialize these variables as pointers, so when their values are modified inside a function, the changes persist after the function returns.
    int answer = 0, stack_pos[1] = {0}, way_pos[1] = {0}, Objective_max_energy[1] = {0};
    
    //alloc memory to the stack and initialize it
    stack = alloc_and_initialize_stack(stack, problem);
    
    //alloc memory and initialize both ways
    aux_way = alloc_and_init_way(aux_way, problem->steps);
    correct_way = alloc_and_init_way(correct_way, problem->steps);

    //check what task we are dealing with and initialize the Objective_energy accordingly
    if(problem->ob_energy == -2){
        Objective_max_energy[0] = 0;
    }else{
        Objective_max_energy[0] = problem->ob_energy;
    }
    //set the energy of the first position as 0 so it doesnt count to the current energy and mark it as visited
    problem->mini_map[info->mini_srow-1][info->mini_scol-1] = 0;
    problem->visited[info->mini_srow-1][info->mini_scol-1] = true;

    //call the recursive function
    answer = recursive_BB_DFS(problem, info, stack, aux_way, correct_way, stack_pos, info->mini_srow-1, info->mini_scol-1, problem->init_energy, problem->steps, Objective_max_energy, way_pos);

    //in case of task to set the answer as the max objective energy or -1 if there was no positive energy found
    if(problem->ob_energy == -2){
        if(Objective_max_energy[0] == 0){
            answer = -1;
        }else{
            answer = Objective_max_energy[0];
        }
    }

    //print the results
    fprintf(file_out, "%d %d %d %d %d %d %d %d\n",problem->rows, problem->columns, problem->ob_energy, problem->srows, problem->scolumns, problem->steps, problem->init_energy, answer);
    
    //if its valid print the way
    if(answer != -1){
        for(int i = 0; i < problem->steps; i++){
            fprintf(file_out, "%d %d %d\n", (correct_way[i][0] + info->mini_map_srow), (correct_way[i][1] + info->mini_map_scolumn), problem->mini_map[correct_way[i][0]][correct_way[i][1]]);
        }
    }
    fprintf(file_out, "\n");

    //free all allocated memory to ensure no memory leaks
    free_mini_maps(info, problem);
    free_stack(stack, problem);
    free_ways(aux_way, correct_way, problem->steps);
    return;
}

