#define _POSIX_C_SOURCE 2
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void find_string(char* dest, char* source,const char* string, int buffer_size){
    char* start_pos = strstr(source, string);
    if(start_pos != NULL){
        start_pos += strlen(string) + 1; // Move to start of the actual string
        char* end_pos = strchr(start_pos, '"');
        if(end_pos != NULL){
            *end_pos = '\0'; // Null-terminate the string
            strncpy(dest, start_pos, buffer_size); // Copy up to 127 characters
        }
    }
}
void malloc_check(char* x, const char* msg){
    if(x == NULL){
        printf("Memory Allocation failed for %s",msg);
        free(x);
        exit(1);
    }
}

int main(){

    char* command;
    command = malloc(sizeof(char)*512);
    malloc_check(command, "Command");
   
    snprintf(command, 512, "cat /etc/*-release");
    FILE* fp = popen(command, "r");

    if(fp == NULL){
        perror("Command failed");
        free(command);
        return 1;
    }

    char* result;
    result = malloc(sizeof(char)*512);
   
    malloc_check(result,"Result");

    int c;
    int i = 0;
    while((c = fgetc(fp)) != EOF && i < 511){
        result[i] = (char)c;
        i++;
    }
    result[i] = '\0';

    if(pclose(fp) == -1){
        printf("pclose failed");
        return 1;
    }

    char* distro;
    distro = malloc(sizeof(char)*128);
    malloc_check(distro, "Distro");

    find_string(distro, result, "PRETTY_NAME=", 128);

    printf("DISTRIBUTION: %s\n",distro);
    free(distro);

    char* kernel;
    kernel = malloc(sizeof(char)*128);
    malloc_check(kernel, "Kernel");

    
    command[0] = '\0';
    snprintf(command, 512, "uname -r");

    fp = popen(command, "r");

    if(fp == NULL){
        perror("Command failed");
        free(command);
        return 1;
    }

    c = 0;
    i = 0;
    while((c = fgetc(fp)) != EOF && i < 511){
        kernel[i] = (char)c;
        i++;
    }
    kernel[i] = '\0';
    free(command);

    if(pclose(fp) == -1){
        printf("pclose failed!");
        return 1;
    }

    printf("KERNEL: %s",kernel);
}


