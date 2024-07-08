int check_package_manager(){
    if(access("/usr/bin/apt-get", X_OK) == 0){
        return 1; // Return 1 if the system package manager is Apt
    }
    else if(access("/usr/bin/dnf", X_OK) == 0){
        return 2; // 2 for DNF
    }
    else if(access("/usr/bin/pacman", X_OK) == 0){
        return 3;
    }
    else if(access("/usr/bin/zypper", X_OK) == 0){
        return 4;
    }
    else{
        return 5;
    }
}

char* packages_pacman(){
    FILE* fp = popen("pacman -Qq | wc -l" , "r");
    if (fp == NULL){
        printf("Couldn't open the command for pacman!");
        return NULL;
    }

    char* result = malloc(sizeof(char)*128);
    malloc_check(result, "Pacman Result");

    if (result == NULL){
        printf("Command failed to execute for pacman!");
        return NULL;
    }

    int c;
    int i = 0;
    while ((c = fgetc(fp)) != EOF && i < 127) {
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

char* packages_apt(){

    FILE* fp = popen("apt list --installed | wc -l" , "r");
    if (fp == NULL){
        printf("Couldn't open the command for Apt!");
        return NULL;
    }

    char* result = malloc(sizeof(char)*128);
    malloc_check(result, "Apt Result");

    if (result == NULL){
        printf("Command failed to execute for Apt!");
        return NULL;
    }

    int c;
    int i = 0;
    while ((c = fgetc(fp)) != EOF && i < 127) {
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

char* packages_dnf(){

    FILE* fp = popen("dnf list installed | wc -l" , "r");
    if (fp == NULL){
        printf("Couldn't open the command for Dnf!");
        return NULL;
    }

    char* result = malloc(sizeof(char)*128);
    malloc_check(result, "DNF Result");

    if (result == NULL){
        printf("Command failed to execute for Dnf!");
        return NULL;
    }

    int c;
    int i = 0;
    while ((c = fgetc(fp)) != EOF && i < 127) {
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

char* packages_zypper(){

    FILE* fp = popen("zypper se --installed-only | wc -l" , "r");
    if (fp == NULL){
        printf("Couldn't open the command for Zypper!");
        return NULL;
    }

    char* result = malloc(sizeof(char)*128);
    malloc_check(result, "Zypper Result");

    if (result == NULL){
        printf("Command failed to execute for Zypper!");
        return NULL;
    }

    int c;
    int i = 0;
    while ((c = fgetc(fp)) != EOF && i < 127) {
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
