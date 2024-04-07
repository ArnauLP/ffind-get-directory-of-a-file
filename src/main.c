#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <stdlib.h>
#include <unistd.h>

const int MAX_LENGTH = 2048;

int main(int argc, char *argv[]) {
    DIR *d;
    struct dirent *dir;

    // check user arguments
    if (argc < 3 || argc > 4) {
        printf("Usage: -> %s <directory> <filename>\n or \n", argv[0]);
        printf("Usage: -> %s <directory> <filename> <options>\n", argv[0]);
        return EXIT_FAILURE;
    }

    // define the length to allocate
    size_t path_len = strlen(argv[1]);
    size_t file_len = strlen(argv[2]);
    size_t extra_len = strlen("/");
    size_t full_len = path_len + file_len + extra_len;
    if (full_len >= MAX_LENGTH || file_len >= MAX_INPUT) {
        printf("Error: path or file is too long!\n");
        return EXIT_FAILURE;
    }

    // allocate memory and copy in a safe way
    char *path = (char *) malloc((path_len + 1) * sizeof(char));
    char *filename = (char *) malloc((file_len + 1) * sizeof(char));
    //char *complete_path = (char *) malloc((full_len + 1) * sizeof(char));
    if (path == NULL || filename == NULL /*|| complete_path == NULL*/) {
        fprintf(stderr, "Error: Failed memory allocation!\n");
        return EXIT_FAILURE;
    }
    strncpy(path, argv[1], path_len);
    strncpy(filename, argv[2], file_len);
    path[MAX_LENGTH] = '\0';
    filename[MAX_INPUT] = '\0';

    // verify directory
    d = opendir(path);
    if (!d) {
        printf("Error: no directory!\n");
        return EXIT_FAILURE;
    }

    // search for file
    while ((dir = readdir(d)) != NULL) {
        //printf("->%s\n", dir->d_name);
        if (strcmp(dir->d_name, filename) == 0) {
            //printf("File {%s} found.\n", filename);
            printf("%s/%s\n", getcwd(dir->d_name, MAX_LENGTH), filename);
            return EXIT_SUCCESS;
        }
    }
    closedir(d);

    return EXIT_SUCCESS;
}