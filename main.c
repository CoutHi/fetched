
#define _POSIX_C_SOURCE 2
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void print_spaces() {
    char d = ' ';
    int i = 0;
    for (i = 0; i < 55; i++) {
        printf("%c", d);
    }
}

void find_string(char* dest, char* source, const char* string, int buffer_size) {
    char* start_pos = strstr(source, string);
    int i = 0;
    int c = 0;

    if (start_pos) {
        start_pos += strlen(string) + 1;
        i = start_pos - source;

        while (source[i] != '"' && i < buffer_size - 1) {
            dest[c] = source[i];
            i++;
            c++;
        }
    } else {
        printf("String %s not found!", string);
        exit(1);
    }

    dest[c] = '\0';
}

void malloc_check(void* x, const char* msg) {
    if (x == NULL) {
        printf("Memory Allocation failed for %s\n", msg);
        exit(1);
    }
}

char* execute_command(const char* command) {
    FILE* fp = popen(command, "r");
    if (fp == NULL) {
        perror("Command failed");
        return NULL;
    }

    char* result = malloc(sizeof(char) * 1024);
    malloc_check(result, "Command result");

    int c;
    int i = 0;
    while ((c = fgetc(fp)) != EOF && i < 1023) {
        result[i] = (char)c;
        i++;
    }
    result[i] = '\0';

    if (pclose(fp) == -1) {
        perror("pclose failed");
        free(result);
        return NULL;
    }

    return result;
}

int main() {
    // Execute command to get Linux distribution details
    char* command = malloc(sizeof(char) * 512);
    malloc_check(command, "Command");

    snprintf(command, 512, "cat /etc/*-release");
    char* result = execute_command(command);
    if (result == NULL) {
        free(command);
        return 1;
    }

    printf("System Information:\n");
    printf("===================\n");
    // Extract distribution details
    char* distro = malloc(sizeof(char) * 128);
    malloc_check(distro, "Distro");

    find_string(distro, result, "PRETTY_NAME=", 128);
    distro[strlen(distro)] = '\0';

    // Print distribution details
    printf("Distribution: %s\n", distro);

    // Execute command to get kernel version
    command[0] = '\0';
    snprintf(command, 512, "uname -r");
    char* kernel = execute_command(command);
    if (kernel == NULL) {
        free(command);
        free(distro);
        free(result);
        return 1;
    }

    // Print kernel version
    printf("Kernel Version: %s\n", kernel);

    // Execute command to get disk usage information
    char* disk_usage_result = execute_command("df -h --output=source,target,used,size | grep -E '/dev/sd|/dev/nvme|/dev/hd|/dev/mmcb|/dev/mapper|/dev/vgname|/dev/vd|/dev/xvd' | awk '{print \"Disk (\" $2 \"):\", $3, $4, $5}'");    
    if (disk_usage_result == NULL) {
        printf("Failed to retrieve disk usage information.\n");
        free(command);
        free(distro);
        free(result);
        free(kernel);
        return 1;
    }

    // Print disk usage information for / 
    printf("Disk Usage: \n%s\n", disk_usage_result);

    // Extract URLs
    char* distro_url = malloc(sizeof(char) * 256);
    malloc_check(distro_url, "Distro_URL");

    find_string(distro_url, result, "HOME_URL=", 256);
    printf("URL: %s\n", distro_url);
    free(distro_url);

    char* documentation = malloc(sizeof(char) * 256);
    malloc_check(documentation, "Documentation");

    find_string(documentation, result, "DOCUMENTATION_URL=", 256);
    printf("Documentation: %s\n", documentation);
    free(documentation);

    // Extract image name
    command[0] = '\0';
    char* image = malloc(sizeof(char) * 64);
    malloc_check(image, "Image");

    int i = 0;
    int c = 0;
    while ((distro[i] != '-' && distro[i] != ' ') && i < 63) {
        image[c] = distro[i];
        i++;
        c++;
    }
    image[c] = '\0';

    // Display image using 'chafa' (you may need to adjust this based on your setup)
    snprintf(command, 512, "chafa --view-size 52x52 ./images/%s-linux.png", image);
    system(command);

    // Free allocated memory
    free(command);
    free(distro);
    free(result);
    free(kernel);
    free(image);
    free(disk_usage_result);

    return 0;
}

