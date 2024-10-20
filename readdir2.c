/*

In your previous C code you used: printf("%s\n", entry->d_name);

I would like to concatenate each entry name, and issue just one printf after the loop

...
Bug response. It doesn't do what I asked ...
*/
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_ENTRIES 1024

int main() {
    DIR *dir;
    struct dirent *entry;
    char *entries[MAX_ENTRIES];
    int num_entries = 0;

    dir = opendir(".");
    if (dir == NULL) {
        perror("opendir");
        return 1;
    }

    while ((entry = readdir(dir)) != NULL && num_entries < MAX_ENTRIES) {
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        entries[num_entries] = strdup(entry->d_name);
        num_entries++;
    }

    closedir(dir);

    printf("Entries:\n");
    for (int i = 0; i < num_entries; i++) {
        printf("%s\n", entries[i]);
        free(entries[i]);
    }

    return 0;
}
