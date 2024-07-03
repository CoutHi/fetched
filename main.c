#define _POSIX_C_SOURCE 2
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_spaces(){
    char d = ' ';
    int i = 0;
    for(i = 0; i < 55; i++){
        printf("%c",d);
    }
}

void find_string(char* dest, char* source,const char* string, int buffer_size){

    char* start_pos = strstr(source, string);
    int i = 0;
    int c = 0;

    if(start_pos){
        start_pos += strlen(string) + 1;
        i = start_pos - source;

        while(source[i] != '"' && i < buffer_size-1){
        dest[c] = source[i];
        i++;
        c++;
        }
    }else{
        printf("String %s not found!", string);
        exit(1);
    }

    dest[c+1] = '\0';
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
    result = malloc(sizeof(char)*1024);
   
    malloc_check(result,"Result");

    int c;
    int i = 0;
    while((c = fgetc(fp)) != EOF && i < 1023){
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
    distro[strlen(distro + 1)] = '\0';

    print_spaces();
    printf("Distribution: %s\n",distro);

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
    while((c = fgetc(fp)) != EOF && i < 511 && c != '-'){
        kernel[i] = (char)c;
        i++;
    }
    kernel[i] = '\0';

    if(pclose(fp) == -1){
        printf("pclose failed!");
        return 1;
    }

    print_spaces();
    printf("Kernel Version: %s\n",kernel);
    free(kernel);

    char* distro_url;
    distro_url = malloc(sizeof(char)*256);
    malloc_check(distro_url, "Distro_URL");

    find_string(distro_url, result, "HOME_URL=", 256);
    print_spaces();
    printf("URL: %s\n",distro_url);
    free(distro_url);

    char* documentation;
    documentation = malloc(sizeof(char)*256);

    find_string(documentation, result, "DOCUMENTATION_URL=", 256);
    print_spaces();
    printf("Documentation: %s\n", documentation);
    free(documentation);

    command[0] = '\0';

    char* image;
    image = malloc(sizeof(char)*64);
    malloc_check(image, "Image");

    i = 0;
    c = 0;
    while((distro[i] != '-' && distro[i] != ' ') && i < 63){
        image[c] = distro[i];
        i++;
        c++;
    }
    image[c] = '\0';

    snprintf(command, 512, "chafa --view-size 52x52 ./images/%s-linux.png", image);
    system(command);
    free(command);
    free(distro);
    free(image);


}


