#define _POSIX_C_SOURCE 2
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    free(command);

    if(pclose(fp) == -1){
        printf("pclose failed");
        return 1;
    }

    char* distro;
    distro = malloc(sizeof(char)*128);
    
    malloc_check(distro, "Distro");

    printf("%s",result);
}


