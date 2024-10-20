/*

I want to join all entry names in a big string, and separate each entry name with '\n'

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
    char *joined_entries = NULL;
    int joined_entries_len = 0;

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

        joined_entries_len += strlen(entry->d_name) + 1; // Account for '\n'
        joined_entries = realloc(joined_entries, joined_entries_len);
        strcat(joined_entries, entry->d_name);
        strcat(joined_entries, "\n");
    }

    closedir(dir);

    printf("Joined entries:\n%s", joined_entries);

    for (int i = 0; i < num_entries; i++) {
        free(entries[i]);
    }
    free(joined_entries);

    return 0;
}
