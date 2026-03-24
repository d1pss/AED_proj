#include "tarefas.h"
#include "file_management.h"
#include "array_management.h"

Problem* allocate_struct_problem(Problem* problem){
    //alloc the struct
    problem = (Problem*)malloc(sizeof(Problem));
    if(problem == NULL){
        exit(0);
    }
    return problem;
}

M_map_info* allocate_struct_m_map_info(M_map_info* info){
    //alloc struct
    info = (M_map_info*)malloc(sizeof(M_map_info));
    if(info == NULL){
        exit(0);
    }
    return info;
}

FILE* file_in_open(char* input_file_name){
    const char* extension_name = ".maps"; 
    FILE* file_in = NULL;

    //check if the file in name ends in .maps  
    if(strcmp(input_file_name + (strlen(input_file_name) - strlen(extension_name)), extension_name) != 0){
        exit(0);
    }

    //open the problems file in
    file_in = fopen(input_file_name, "r");
    if(file_in == NULL){
        exit(0);
    }
    return file_in;
}

FILE* file_out_open(char* input_file_name, FILE* file_in){
    int out_len = 0;
    char* out_name = NULL;
    FILE* file_out = NULL;

    //get name for output file based on input file name
    out_len = strlen(input_file_name);
    out_name = (char*)calloc((out_len + 3), sizeof(char*));
    if(out_name == NULL){
        exit(0);
    }
    memcpy(out_name, input_file_name, (size_t)(out_len - 4));
    strcat(out_name, "solmaps");

    //open output file
    file_out = fopen(out_name, "w");
    if(file_out == NULL){
        fclose(file_in);
        exit(0);
    }
    free(out_name);
    return file_out;
}