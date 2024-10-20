/*

Write a C code that uses the getdents syscall to list the files and directories in the current directory.

Don't save the entries in 'buf'. Instead, concatenate them in a string, like you did before, using:

#define MAX_JOINED_ENTRIES_LEN 1000

char joined_entries[MAX_JOINED_ENTRIES_LEN + 1] = {0}; // Initialize to zero

*/
#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/syscall.h>
#include <string.h>

#define MAX_ENTRIES 1024
#define MAX_JOINED_ENTRIES_LEN 1000

// https://elixir.bootlin.com/linux/v6.11.4/source/fs/readdir.c#L245
struct linux_dirent {
	unsigned long	d_ino;
	unsigned long	d_off;
	unsigned short	d_reclen;
	char		d_name[];
};

// https://elixir.bootlin.com/linux/v6.11.4/source/include/uapi/linux/fcntl.h#L98
#define AT_FDCWD		-100    /* Special value used to indicate
                                           openat should use the current
                                           working directory. */

int main() {
    int dirfd;
    struct linux_dirent *dirent;
    char buf[MAX_ENTRIES * sizeof(struct linux_dirent)];
    int nread, offset, i;
    char joined_entries[MAX_JOINED_ENTRIES_LEN + 1] = {0}; // Initialize to zero
    int joined_entries_len = 0;

    dirfd = AT_FDCWD; // Current directory

    while ((nread = syscall(SYS_getdents, dirfd, buf, MAX_ENTRIES * sizeof(struct linux_dirent))) > 0) {
        for (offset = 0; offset < nread; offset += dirent->d_reclen) {
            dirent = (struct linux_dirent *)(buf + offset);
            if (dirent->d_name[0] != '.') { // Skip "." and ".."
                if (joined_entries_len + strlen(dirent->d_name) + 1 > MAX_JOINED_ENTRIES_LEN) {
                    fprintf(stderr, "Joined entries exceeded maximum length.\n");
                    break;
                }
                strcat(joined_entries + joined_entries_len, dirent->d_name);
                strcat(joined_entries + joined_entries_len, "\n");
                joined_entries_len += strlen(dirent->d_name) + 1;
            }
        }
    }

    printf("Joined entries:\n%s", joined_entries);

    return 0;
}
