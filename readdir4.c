/*

avoid realloc. Just issue one malloc for joined_entries before the loop, so it supports up to 1000 characters

*/
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_ENTRIES 1024
#define MAX_JOINED_ENTRIES_LEN 1000

int main() {
    DIR *dir;
    struct dirent *entry;
    char *entries[MAX_ENTRIES];
    int num_entries = 0;
    char joined_entries[MAX_JOINED_ENTRIES_LEN + 1]; // +1 for null terminator
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

        if (joined_entries_len + strlen(entry->d_name) + 1 > MAX_JOINED_ENTRIES_LEN) {
            fprintf(stderr, "Joined entries exceeded maximum length.\n");
            break;
        }

        strcat(joined_entries + joined_entries_len, entry->d_name);
        strcat(joined_entries + joined_entries_len, "\n");
        joined_entries_len += strlen(entry->d_name) + 1;
    }

    closedir(dir);

    printf("Joined entries:\n%s", joined_entries);

    for (int i = 0; i < num_entries; i++) {
        free(entries[i]);
    }

    return 0;
}
