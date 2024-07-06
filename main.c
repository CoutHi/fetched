#define _POSIX_C_SOURCE 2
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "custom_functions.h"

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

    // Print disk usage information for  
    printf("Disk Usage: \n%s\n", disk_usage_result);

    // Memory usage
    char* mem_usage = execute_command("free -h | grep Mem | awk '{print $3, $2}'");
    if (mem_usage == NULL){
        printf("Failed to retrieve memory usage information.\n");
        free(command);
        free(distro);
        free(result);
        free(kernel);
        return 1;
    }
    printf("Memory usage: %s", mem_usage);

    // Get the amount of flatpak packages
    char* flatpak_packages = execute_command("flatpak list --app | wc -l");
    if(flatpak_packages == NULL){
        printf("Failed to retrieve flatpak information.\n");
        free(command);
        free(distro);
        free(result);
        free(kernel);
        return 1;
    }
    printf("Flatpak: %s\n",flatpak_packages);

    // Get the amount of packages from the normal package manager
    // TODO, Make a switch case that gets the amount of packages based on the distribution and the distribution's package manager commands.

    // Get SHELL information
    char* shell = execute_command("echo $SHELL");
    if(shell == NULL){
        printf("Failed to retrieve SHELL information.\n");
        free(command);
        free(distro);
        free(result);
        free(kernel);
        return 1;
    }
    printf("Shell: %s\n",shell);

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

