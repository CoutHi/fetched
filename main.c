#define _POSIX_C_SOURCE 2
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "custom_functions.h"
#include "package_functions.h"

#define PROJECT_DIR "/usr/share/fetched/"

struct package_manager_commands{
    char* update;
    char* install;
    char* delete;
    char* search;
    char* list;
};

int main() {

    int i = 0;
    int c = 0;

    // Execute command to get Linux distribution details
    char* command = malloc(sizeof(char) * 512);
    malloc_check(command, "Command");

    snprintf(command, 512, "cat /etc/*-release");
    char* result = execute_command(command);
    if (result == NULL) {
        free(command);
        return 1;
    }

    printf("System Information:");
    underline();
    // Extract distribution details
    char* distro = malloc(sizeof(char) * 128);
    malloc_check(distro, "Distro");

    find_string(distro, result, "PRETTY_NAME=", 128);
    distro[strlen(distro)] = '\0';

    // Print distribution details
    printf("Distribution: %s\n", distro);
     
    // Kernel Information
    command[0] = '\0';
    snprintf(command, 512, "uname -r");
    char* kernel = execute_command(command);
    if (kernel == NULL) {
        free(command);
        return 1;
    }else{
        printf("Kernel Version: %s\n",kernel);
        free(kernel);
    }

    // Extract image name
    command[0] = '\0';
    char* image = malloc(sizeof(char) * 64);
    malloc_check(image, "Image");

    i = 0;
    c = 0;
    while ((distro[i] != '-' && distro[i] != ' ') && i < 63) {
        image[c] = distro[i];
        i++;
        c++;
    }
    image[c] = '\0';
    
    printf("Help:");
    underline();

    // Distro URLs
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
    
    // Display image using ascii conversion
    snprintf(command, 512, "ascii_me %simages/%s-linux.png 70 35", PROJECT_DIR,image);
    system(command);
    free(image);
    free(distro);
    free(result);

    printf("Disk And Memory:");
    underline();

    // Execute command to get kernel version
    // Execute command to get disk usage information
    char* disk_usage_result = execute_command("df -h --output=source,target,used,size | grep -E '/dev/sd|/dev/nvme|/dev/hd|/dev/mmcb|/dev/mapper|/dev/vgname|/dev/vd|/dev/xvd' | awk '{print \"Disk (\" $2 \"):\", $3, $4, $5}'");    
    if (disk_usage_result == NULL) {
        printf("Failed to retrieve disk usage information.\n");
        free(command);
        return 1;
    }else{
        printf("%s\n",disk_usage_result);
        free(disk_usage_result);
    }

    // Memory usage
    char* mem_usage = execute_command("free -h | grep Mem | awk '{print $3, $2}'");
    if (mem_usage == NULL){
        printf("Failed to retrieve memory usage information.\n");
        free(command);
        return 1;
    }
    printf("Memory: %s", mem_usage);

    printf("\nPackages:");
    underline();
    // Get the amount of flatpak packages
    char* flatpak_packages = execute_command("flatpak list --app | wc -l");
    if(flatpak_packages == NULL){
        printf("Failed to retrieve flatpak information.\n");
        free(command);
        return 1;
    }

    printf("Flatpak: %s",flatpak_packages);

    // Get the number of packages depending on the package manager in the system.
    int package_manager = check_package_manager();
    char* package_number;

    switch(package_manager){
        case 1:
            package_number = packages_apt();
            malloc_check(package_number, "Package Number");
            printf("Standard: %s\n",package_number);
            break;
        case 2:
            package_number = packages_dnf();
            malloc_check(package_number, "Package Number");
            printf("Standard: %s\n",package_number);
            break;
        case 3:
            package_number = packages_pacman();
            malloc_check(package_number, "Package Number");
            printf("Standard: %s\n",package_number);
            break;
        case 4:
            package_number = packages_zypper();
            malloc_check(package_number, "Package Number");
            printf("Standard: %s\n",package_number);
            break;
        default:
            break;
    }
    free(package_number);

    struct package_manager_commands package_commands;
    switch(package_manager){
        case 1:
            package_commands.update = "sudo apt-get update && sudo apt-get upgrade";
            package_commands.install = "sudo apt-get install [x]";
            package_commands.delete = "sudo apt-get remove [x]";
            package_commands.search = "apt-get search [x]";
            package_commands.list = "apt-get list --installed";
            break;
        case 2:
            package_commands.update = "sudo dnf update";
            package_commands.install = "sudo dnf install [x]";
            package_commands.delete = "sudo dnf remove [x]";
            package_commands.search = "dnf search [x]";
            package_commands.list = "dnf list installed";
            break;
        case 3:
            package_commands.update = "sudo pacman -Syu";
            package_commands.install = "sudo pacman -S [x]";
            package_commands.delete = "sudo pacman -Rns [x]";
            package_commands.search = "pacman -Ss [x]";
            package_commands.list = "pacman -Qq";
            break;
        case 4:
            package_commands.update = "sudo zypper up";
            package_commands.install = "sudo zypper in [x]";
            package_commands.delete = "sudo zypper rm [x]";
            package_commands.search = "zypper se [x]";
            package_commands.list = "zypper se -i";
            break;
        default:
             break;
    }
    printf("\nPackage Manager Commands:");
    underline();
    printf("Update: %s\nInstall: %s\nDelete: %s\nSearch: %s\nList: %s\n",package_commands.update,package_commands.install,package_commands.delete,package_commands.search,package_commands.list);

    printf("\nTerminal Environment:");
    underline();
    // Get SHELL information
    char* shell = execute_command("echo $SHELL");
    if(shell == NULL){
        printf("Failed to retrieve SHELL information.\n");
        free(command);
        return 1;
    }
    printf("Shell: %s",shell);

    // Execute command to get terminal emulator information
    char* terminal_info = execute_command("echo $TERM"); //Alternative `update-alternatives --display x-terminal-emulator | grep currently`
    if (terminal_info == NULL) {
        printf("Failed to retrieve terminal emulator information.\n");
    } else {
        // Print terminal emulator information
        printf("Emulator:%s\n", terminal_info);
        free(terminal_info);
    }

    char* uptime_info = execute_command("uptime -p"); // Alternative `w` or `cat /proc/uptime` or `systemctl status | grep Since`
    if (uptime_info == NULL) {
        printf("Failed to retrieve system uptime information.\n");
    } else {
        // Print system uptime information
        printf("System Uptime:\n%s\n", uptime_info);
        free(uptime_info);
    }

    // CPU Information
    printf("CPU Information:");
    underline();

    char* cpu_info = execute_command("grep -m1 \"model name\" /proc/cpuinfo && grep -m1 \"cpu cores\" /proc/cpuinfo");
    if(cpu_info == NULL){
        printf("Failed to retrieve CPU information!");
        free(command);
        free(cpu_info);
    // Function to make every letter in the String uppercase after a space.
    }else{
        i = 0;
        char ch; 
        cpu_info[i] = toupper(cpu_info[i]);
        for(i = 0; i < strlen(cpu_info); i++){
            if(cpu_info[i] == ' ' || cpu_info[i] == '\n'){
                ch = cpu_info[i+1];
                cpu_info[i+1] = toupper(ch);
            }
        }
        printf("%s",cpu_info);
        free(cpu_info);
    }

    // Get GPU Information
    char* gpu_info = execute_command("lspci | grep VGA");
    if(gpu_info == NULL){
        printf("Failed to retrieve GPU information!");
        free(command);
        free(gpu_info);
    }
    else{
        char* formatted_gpu;
        formatted_gpu = malloc(sizeof(char)*128);
        malloc_check(formatted_gpu, "formatted_gpu");

        i = 0;
        c = 0;
        while(gpu_info[i] != '['){
            i++;
        }
        i++;
        while(gpu_info[i] != ']'){
            formatted_gpu[c] = gpu_info[i];
            i++;
            c++;
        }
        formatted_gpu[c] = '\0';

        printf("\nGPU Information:");
        underline();
        printf("Model Name: %s",formatted_gpu);
        free(gpu_info);
        free(formatted_gpu);
    }
    
    printf("\n\nDesktop:");
    underline();
    // Get Desktop Environment
    char* desktop_env = execute_command("echo $XDG_CURRENT_DESKTOP");
    if(desktop_env == NULL){
        printf("Failed to retrieve Desktop information!");
        free(command);
        free(desktop_env);
    }
    else{
        printf("Desktop Environment: %s",desktop_env);
        free(desktop_env);
    }
    
    // X11 or Wayland?
    char* session_type = execute_command("echo $XDG_SESSION_TYPE");
    malloc_check(session_type, "session_type");
    printf("Session Type: %s\n",session_type);
    free(session_type);
    // Free allocated memory
    free(command);

    return 0;
}

