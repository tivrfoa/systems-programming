/*

Asking Gemini:
In C, how to use readdir to list files and directories in current folder?
*/
#include <dirent.h>
#include <stdio.h>
#include <string.h>

int main() {
    DIR *dir;
    struct dirent *entry;

    dir = opendir("."); // Open the current directory
    if (dir == NULL) {
        perror("opendir");
        return 1;
    }

    while ((entry = readdir(dir)) != NULL) {
        // Skip "." and ".." entries
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        printf("%s\n", entry->d_name);
    }

    closedir(dir);
    return 0;
}
