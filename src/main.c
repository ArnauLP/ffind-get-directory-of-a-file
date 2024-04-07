#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <stdlib.h>
#include <sys/stat.h>

const int MAX_LENGTH = 2048;

void search_file(const char *path, const char *filename) {
    DIR *d;
    struct dirent *dir;
    char fullpath[MAX_LENGTH];
    struct stat statbuf;

    d = opendir(path);
    if (!d) {
        fprintf(stderr, "Error: Unable to acces \"%s\".\n", path);
        return;
    }

    while ((dir = readdir(d)) != NULL) {
        // ignore "." ".."
        if (strcmp(dir->d_name, ".") == 0 || strcmp(dir->d_name, "..") == 0) {
            continue;
        }

        snprintf(fullpath, sizeof(fullpath), "%s/%s", path, dir->d_name);

        // get file status
        if (stat(fullpath, &statbuf) == -1) {
            fprintf(stderr, "Error: Unable to stat file %s\n", fullpath);
            continue;
        }

        // check if it's a directory
        if (S_ISDIR(statbuf.st_mode)) {
            search_file(fullpath, filename);
        } else if (strcmp(dir->d_name, filename) == 0) {
            printf("Found: %s\n", fullpath);
        }
    }

    closedir(d);
}

int main(int argc, char *argv[]) {
    // check user arguments
    if (argc < 3 || argc > 4) {
        printf("Usage: -> %s <directory> <filename>\n or \n", argv[0]);
        return EXIT_FAILURE;
    }

    search_file(argv[1], argv[2]);

    return EXIT_SUCCESS;
}