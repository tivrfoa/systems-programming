/*
Q: Instead of printing inside the loop, store all entries in one string, with each entry separated by '\n', eg: file1.txt (regular file)\nfile2.c\n

Then print this string after the loop
*/
#define _GNU_SOURCE
#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>

#define BUF_SIZE 1024
#define MAX_ENTRIES_SIZE 4096

int main() {
    int fd;
    int nread;
    char buf[BUF_SIZE];
    struct linux_dirent {
        long d_ino;
        off_t d_off;
        unsigned short d_reclen;
        char d_name[];
    };
    struct linux_dirent *d;
    int bpos;
    char entry[MAX_ENTRIES_SIZE] = {0};  // String to hold all directory entries
    char temp[256];                      // Temporary buffer to build individual entries
    struct stat st;

    // Open the current directory
    fd = open(".", O_RDONLY | O_DIRECTORY);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    // Read directory entries using getdents
    for (;;) {
        nread = syscall(SYS_getdents, fd, buf, BUF_SIZE);
        if (nread == -1) {
            perror("getdents");
            exit(EXIT_FAILURE);
        }

        if (nread == 0) // End of directory
            break;

        for (bpos = 0; bpos < nread;) {
            d = (struct linux_dirent *) (buf + bpos);

            // We cannot use d_type directly here like in getdents64, so we use stat instead:
            if (stat(d->d_name, &st) == 0) {
                if (S_ISREG(st.st_mode))
                    snprintf(temp, sizeof(temp), "%s (regular file)\n", d->d_name);
                else if (S_ISDIR(st.st_mode))
                    snprintf(temp, sizeof(temp), "%s (directory)\n", d->d_name);
                else if (S_ISLNK(st.st_mode))
                    snprintf(temp, sizeof(temp), "%s (symlink)\n", d->d_name);
                else
                    snprintf(temp, sizeof(temp), "%s (other)\n", d->d_name);
            } else {
                snprintf(temp, sizeof(temp), "%s (unknown)\n", d->d_name);
            }

            // Append the entry to the main string
            strncat(entry, temp, sizeof(entry) - strlen(entry) - 1);

            bpos += d->d_reclen;
        }
    }

    // Print all the stored directory entries
    printf("%s", entry);

    close(fd);
    return 0;
}

