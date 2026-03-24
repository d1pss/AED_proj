#include "tarefas.h" 
#include "file_management.h"
#include "array_management.h"

/***********************************************************************************
 * @brief The functions that follow are the functions that work with the 2D array 
 * filled with the current problem map reachable energy and the 2D bool array that 
 * checks if that cell has ben visited before
 * 
 **********************************************************************************/

void Get_mini_map_info(M_map_info* info, Problem* problem){
    //check if the square or rectangle around the starting point leaves the bounds of the map
    //calculate the location of the conners of the mini map with the range available
    //starting column
    if((problem->scolumns - problem->steps) <= 0)
    {//check if its out of bounds
        info->mini_map_scolumn = 1;
        info->n_out_of_bounds_scolumn = abs(problem->steps - problem->scolumns) + 1;
    }else{
        info->mini_map_scolumn = problem->scolumns - problem->steps;
        info->n_out_of_bounds_scolumn = 0;
    }
    //ending column
    if((problem->scolumns + problem->steps) > problem->columns)
    {//check if its out of bounds
        info->mini_map_ecolumn = problem->columns;
        info->n_out_of_bounds_ecolumn = (problem->scolumns + problem->steps) - problem->columns;

    }else{
        info->mini_map_ecolumn = problem->scolumns + problem->steps;
        info->n_out_of_bounds_ecolumn = 0;
    }
    //starting row
    if((problem->srows - problem->steps) <= 0)
    {//check if its out of bounds
        info->mini_map_srow = 1;
        info->n_out_of_bounds_srow = abs(problem->steps - problem->srows) + 1;
    }else{
        info->mini_map_srow = problem->srows - problem->steps;
        info->n_out_of_bounds_srow = 0;
    }
    //ending row
    if((problem->srows + problem->steps) > problem->rows)
    {//check if its out of bounds
        info->mini_map_erow = problem->rows;
        info->n_out_of_bounds_erow = (problem->srows + problem->steps) - problem->rows;
    }else{
        info->mini_map_erow = problem->srows + problem->steps;
        info->n_out_of_bounds_erow = 0;
    }
    //do some extra calculations to find the size and the starting point in the mini map
    info->mini_columns = info->mini_map_ecolumn - info->mini_map_scolumn + 1;
    info->mini_rows = info->mini_map_erow - info->mini_map_srow + 1;
    info->mini_srow = problem->steps - info->n_out_of_bounds_srow + 1;
    info->mini_scol = problem->steps - info->n_out_of_bounds_scolumn + 1;
   
    return;
}

void alloc_mini_maps(M_map_info* info, Problem* problem){
    //alloc the squared or rectangular map based on the max dimension previous calculated

    //alloc the array of int*
    problem->mini_map = (int**)malloc(info->mini_rows * sizeof(int*));
    if(problem->mini_map == NULL){
        exit(0);
    }

     //alloc the array of bool*
    problem->visited = (bool**)malloc(info->mini_rows * sizeof(bool*));
    if(problem->visited == NULL){
        exit(0);
    }

    //in each position of the array of int* alloc one int
    //in each position of the array of bool* alloc one bool
    for(int i = 0; i < info->mini_rows; i++){
        problem->mini_map[i] = (int*)malloc(info->mini_columns * sizeof(int));
        problem->visited[i] = (bool*)malloc(info->mini_columns * sizeof(bool));
        if(problem->visited[i] == NULL || problem->mini_map[i] == NULL){
            exit(0);
        }
    }

    init_bool_array(info, problem);

    return;
}

void init_bool_array(M_map_info* info, Problem* problem){
    //initialize the bool array
    for(int i = 0; i < info->mini_rows; i++){
        for(int j = 0; j < info->mini_columns; j++){
            problem->visited[i][j] = false;
        }
    }
    return;
}

void fill_up_mini_map(Problem* problem, M_map_info* info, FILE* file_in){
    int no_save = 0;
    no_save++;
    //travel trought all the main map and only save the values in the mini map if in bounds of it self
    for(int i = 1; i <= problem->rows; i++){
        for(int j = 1; j <= problem->columns; j++){
            if(((i <= info->mini_map_erow) && (i >= info->mini_map_srow)) && ((j <= info->mini_map_ecolumn) && (j >= info->mini_map_scolumn)))
            {//if inside the bound of the mini map save the values
                no_save = fscanf(file_in, "%d", &problem->mini_map[i - info->mini_map_srow][j - info->mini_map_scolumn]);
            }else
            {//outside of bounds advance file pointer to the next number
                no_save = fscanf(file_in, "%*d");
            }
        }
    }
    return;
}

void free_mini_maps(M_map_info* info, Problem* problem){
    //free the int and bool mini map
    for(int i = 0; i < info->mini_rows; i++){
        free(problem->mini_map[i]);
        free(problem->visited[i]);
        
    }
    free(problem->mini_map);
    free(problem->visited);
}




/***********************************************************************************
 * @brief The functions that follow are the functions that work with the max energys
 * of the mini map creating the Org_sum_max_energys 
 * 
 **********************************************************************************/

void read_diamond_and_save_energys(Problem* problem, M_map_info* info, int* array_positive_energys, int max_size_array[1], int steps, int center_srow, int center_scol){
    int aux0[2] = {center_srow-1, center_scol}, aux1[2] = {center_srow, center_scol+1},
        aux2[2] = {center_srow+1, center_scol}, aux3[2] = {center_srow, center_scol-1};
    //reads only the diamod shape out of the mini map based on the center point and the number of steps given
    for(int i = 1; i <= steps; i++){
        for(int j = 0; j < i*4; j++){
            switch (j%4){
            case 0://top left
                if(check_inbound_cords(aux0[0], aux0[1], info))
                {//if the cords are in bounds of the mini map
                    //check that cell and see if the energy in it is positive and it has not been visited before
                    if(problem->mini_map[aux0[0]][aux0[1]] > 0 && problem->visited[aux0[0]][aux0[1]] == false){
                        //add energy to the array
                        array_positive_energys[max_size_array[0]] = problem->mini_map[aux0[0]][aux0[1]];
                        max_size_array[0]++;
                    }
                }
                //send aux to the next point that is the diagonal of the curr point or if it is the 
                //last point of that step send back to the begining but start in the point next to the one before
                if(j/4 == i-1){
                    aux0[0] = center_srow-1;
                    aux0[1] = center_scol - i;
                }else{
                    aux0[0] = aux0[0] - 1;
                    aux0[1] = aux0[1] + 1;
                }
                break;
            
            case 1://top right
                if(check_inbound_cords(aux1[0], aux1[1], info))
                {//if the cords are in bounds of the mini map
                    //check that cell and see if the energy in it is positive and it has not been visited before
                    if(problem->mini_map[aux1[0]][aux1[1]] > 0 && problem->visited[aux1[0]][aux1[1]] == false){
                        //add energy to the array
                        array_positive_energys[max_size_array[0]] = problem->mini_map[aux1[0]][aux1[1]];
                        max_size_array[0]++;
                    }
                }
                //send aux to the next point that is the diagonal of the curr point or if it is the 
                //last point of that step send back to the begining but start in the point next to the one before
                if(j/4 == i-1){ 
                    aux1[0] = center_srow - i;
                    aux1[1] = center_scol+1;
                }else{
                    aux1[0] = aux1[0] + 1;
                    aux1[1] = aux1[1] + 1;
                }
                break;

            case 2://bottom right
                if(check_inbound_cords(aux2[0], aux2[1], info))
                {//if the cords are in bounds of the mini map
                    //check that cell and see if the energy in it is positive and it has not been visited before
                    if(problem->mini_map[aux2[0]][aux2[1]] > 0 && problem->visited[aux2[0]][aux2[1]] == false){
                        //add energy to the array
                        array_positive_energys[max_size_array[0]] = problem->mini_map[aux2[0]][aux2[1]];
                        max_size_array[0]++;
                    }
                }
                //send aux to the next point that is the diagonal of the curr point or if it is the 
                //last point of that step send back to the begining but start in the point next to the one before
                if(j/4 == i-1){ 
                    aux2[0] = center_srow+1;
                    aux2[1] = center_scol + i;
                }else{
                    aux2[0] = aux2[0] + 1;
                    aux2[1] = aux2[1] - 1;
                }
                break;

            case 3://bottom left
                if(check_inbound_cords(aux3[0], aux3[1], info))
                {//if the cords are in bounds of the mini map
                    //check that cell and see if the energy in it is positive and it has not been visited before
                    if(problem->mini_map[aux3[0]][aux3[1]] > 0 && problem->visited[aux3[0]][aux3[1]] == false){
                        //add energy to the array
                        array_positive_energys[max_size_array[0]] = problem->mini_map[aux3[0]][aux3[1]];
                        max_size_array[0]++;
                    }
                }
                //send aux to the next point that is the diagonal of the curr point or if it is the 
                //last point of that step send back to the begining but start in the point next to the one before
                if(j/4 == i-1){ 
                    aux3[0] = center_srow + i;
                    aux3[1] = center_scol-1;
                }else{
                    aux3[0] = aux3[0] - 1;
                    aux3[1] = aux3[1] - 1;
                }
                break;
            
            default:
                exit(0);
            }
        }
    }
    return;
}

int get_sum_max_energy(Problem* problem, M_map_info* info, int* array_positive_energys, int steps, int curr_row, int curr_col){
    int max_sum_of_reachble_energy = 0, max_size_array[1] = {0};

    //read all the positive energys from the diamond shape of the mini map
    read_diamond_and_save_energys(problem, info, array_positive_energys, max_size_array, steps, curr_row, curr_col);

    //and sum max k energys
    if(max_size_array[0] < steps)
    {//if all the positive energys found are less than the number of steps sum all energys
        for(int i = 0; i < max_size_array[0]; i++){
            max_sum_of_reachble_energy += array_positive_energys[i];
        }
    }else{
        //if all the positive energys found are bigger than the number of steps

        //We use Quickselect to efficiently find the 'k' largest values in the array.
        //Instead of fully sorting the array (O(n log n)), Quickselect finds the k largest
        //elements in average O(n) time, which is faster since we don't need them in order.
        //After running Quickselect, the largest 'k' values are in the end of the array.
        quickSelect(array_positive_energys, 0, max_size_array[0]-1, max_size_array[0]-steps);

        //Sum the largest 'k' values that are in the end of the array.
        for(int i = max_size_array[0] - steps; i < max_size_array[0]; i++){
            max_sum_of_reachble_energy += array_positive_energys[i];
        }
    }
    
    return max_sum_of_reachble_energy;
}




/***********************************************************************************
 * @brief The functions that follow are the functions that work with the stack 
 * and LC_array arrays that are used in the LC_B&B_DFS implementation
 * 
 **********************************************************************************/

int** alloc_and_initialize_stack(int** stack, Problem* problem){
    //alloc the stack that is a 2D array with 2*k+2 positions of 2 ints
    //that store the cells that are going to be analized
    stack = (int**)malloc((2+2*problem->steps)*sizeof(int*));
    if(stack == NULL){
        exit(0);
    }
    for(int i = 0; i < (2+2*problem->steps); i++){
        stack[i] = (int*)calloc(2,sizeof(int));
        if(stack[i] == NULL){
            exit(0);
        }
        //set the first element of the two ints to -1 to simbolize that it is empty
        stack[i][0] = -1;
    }
    return stack;
}

int remove_first_in_stack(int** stack, int curr_cell[2], int stack_pos){
    //remove the last position of the array given by stack_pos - 1
    curr_cell[0] = stack[stack_pos-1][0];
    curr_cell[1] = stack[stack_pos-1][1];
    //set to -1 to simbolize that it is empty
    stack[stack_pos-1][0] = -1;
    return stack_pos-1;
}

int add_in_stack(int** stack, int* cords_to_add, int stack_pos){
    //add to the last position of the array given by stack_pos
    stack[stack_pos][0] =  cords_to_add[0];
    stack[stack_pos][1] =  cords_to_add[1];
    return stack_pos+1;
}

void add_to_LC_array(int LC_array[][2], int x, int y){
    //add to this auxiliar 2D array x and y
    for(int i = 0; i < 4; i++){
        if(LC_array[i][0] == -1){
            LC_array[i][0] = x;
            LC_array[i][1] = y;
            break;
        }
    }
    return;
}

int remove_from_LC_array_and_add_to_stack(int LC_array[][2], int** stack, int stack_pos){
    //after being sorted add the auxiliar array to the stack inverted so the first positions (the better ways) come first in the stack
    for(int i = 3; i != -1; i--){
        if(LC_array[i][0] == -1){
            continue;
        }
        stack_pos = add_in_stack(stack, LC_array[i], stack_pos);
        LC_array[i][0] = -1;
    }
    
    return stack_pos;
}

int sort_LC_array(int LC_array[][2], int* ob_energy, int curr_step, int curr_energy, Problem* problem, M_map_info* info){
    int max_energys[4] = {0}, N = 0, j = 0, swapped = 1, size_array = 2*curr_step*(curr_step+1);
    int* array_positive_energys = NULL;
    
    //find LC_array size
    for(int i = 0; i < 4; i++){
        if(LC_array[i][0] == -1){
            break;
        }
        N++;
    }

    //alloc it out here so we dont have to alloc the array multiple times inside the get_sum_max_energy func
    //because it will have allways the same size
    array_positive_energys = (int*)malloc(size_array * sizeof(int));
    if(array_positive_energys == NULL) exit(0);

    //for each position of the array get the max energy available for that point
    for(int i = 0; i < N; i++){
        max_energys[i] = get_sum_max_energy(problem, info, array_positive_energys, curr_step, LC_array[i][0], LC_array[i][1]);
    }

    //free the array because we no longer need it
    free(array_positive_energys);

    //sort the array based on the cell energy + max energys available for that cell
    while (swapped){
        swapped = 0;
        for(int i = 0; i < N - j - 1; i++){
            if(problem->mini_map[LC_array[i][0]][LC_array[i][1]] + max_energys[i] < problem->mini_map[LC_array[i+1][0]][LC_array[i+1][1]] + max_energys[i+1]){
                swapped = 1;
                swap_LC_array_and_max_energys(LC_array, max_energys, i, i+1);
            }
        }
        j++;
    }

    //check if the current energy + the potencial energy are enough to reach the objective energy
    //or the current energy is greater than zero and keep the array or remove that path because is not valid
    for(int i = 0; i < N; i++){
        if(curr_energy + problem->mini_map[LC_array[i][0]][LC_array[i][1]] + max_energys[i] < ob_energy[0]){
            for(int j = i; j < N; j++){
                LC_array[j][0] = -1;
            }
            return i;
        }
    }
    return N;

}

void swap_LC_array_and_max_energys(int LC_array[][2], int max_energys[4], int i, int j){
    int tmp[2] = {LC_array[i][0], LC_array[i][1]}, tmp2 = max_energys[i];
    //swap all the points and the max energys
    LC_array[i][0] = LC_array[j][0];
    LC_array[i][1] = LC_array[j][1];
    LC_array[j][0] = tmp[0];
    LC_array[j][1] = tmp[1];
    max_energys[i] = max_energys[j];
    max_energys[j] = tmp2;
    return;
}

int check_inbound_cords(int x, int y, M_map_info* info){
    //check if the cords given are in bounds of the mini map
    return !(x < 0 || y < 0 || x > info->mini_rows - 1 || y > info->mini_columns - 1);
}

void free_stack(int** stack, Problem* problem){
    //free all positions of the stack
    for(int i = 0; i < 2+problem->steps*2; i++){
        free(stack[i]);
    }
    free(stack);
    return;
}




/***********************************************************************************
 * @brief The functions that follow are the functions that work with the ways arrays
 * that store the way taken to the correct answer
 * 
 **********************************************************************************/

int** alloc_and_init_way(int** way, int size){
    //alloc the array that is a 2D array with k positions of 2 ints
    //that store the cells that we travel trough
    way = (int**)malloc(size * sizeof(int*));
    if(way == NULL) exit(0);

    for(int i = 0; i < size; i++){
        way[i] = (int*)malloc(2 * sizeof(int));
        if(way[i] == NULL) exit(0);
        //set the first element of the two ints to -1 to simbolize that it is empty
        way[i][0] = -1;
    }
    return way;
}

void free_ways(int** aux_way, int** correct_way, int size){
    //free all positions of the way array
    for(int i = 0; i < size; i++){
        free(aux_way[i]);
        free(correct_way[i]);
    }
    free(aux_way);
    free(correct_way);
    return;
}

int remove_cords_from_aux_way(int** way, int way_pos){
    //remove the last point (that was previous inserted) in the array using array_pos - 1
    way[way_pos-1][0] = -1;
    return way_pos-1;
}

int add_cords_to_aux_way(int** way, int way_pos, int* curr_cell){
    //add the current cords into the first position in the array available indicated by way_pos
    way[way_pos][0] = curr_cell[0];
    way[way_pos][1] = curr_cell[1];
    return way_pos+1;
}

void copy_way(int** aux_way, int** correct_way, int size){
    //copy the aux_way into the correct_way
    for(int i = 0; i < size; i++){
        correct_way[i][0] = aux_way[i][0];
        correct_way[i][1] = aux_way[i][1];
    }
    return;
}



/***********************************************************************************
 * @brief The functions that follow are part of the quick select algorithm
 * 
 **********************************************************************************/

void swap(int* a, int* b){
    int t = *a;
    *a = *b;
    *b = t;
    return;
}
 
int partition (int arr[], int low, int high){
    int pivot = arr[low];
    int i = low,j;  //Index of smaller element
 
    for (j = low+1; j <= high; j++){
        //If current element is smaller than or equal to pivot
        if (arr[j] <= pivot){
            i++;    //increment index of smaller element
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i], &arr[low]);
    return i;
}
 
void quickSelect(int arr[], int low, int high, int k){   
    int part_idx;
    if (low < high){
        //get the pivot
        part_idx = partition(arr, low, high);
        
        //call quick select again
        if(k <= part_idx){
            quickSelect(arr, low,part_idx - 1, k);
        }else if(k == part_idx+1){
            return;
        }else{
            quickSelect(arr,part_idx + 1, high, k);
        }
    }
    return;
}


