void underline(){
    printf("\n");
    int i = 0;
    while(i < 25){
        printf("=");
        i++;
    }
    printf("\n");
}

void malloc_check(void* x, const char* msg) {
    if (x == NULL) {
        printf("Memory Allocation failed for %s\n", msg);
        free(x);
        exit(1);
    }
}

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
