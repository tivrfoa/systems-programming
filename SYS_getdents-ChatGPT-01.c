/*
Q: You used SYS_getdents64
Can you use SYS_getdents instead?

---
Sure! If you want to use the SYS_getdents syscall instead of SYS_getdents64, the only significant difference is the structure of the directory entry. The getdents system call uses the struct linux_dirent (instead of struct linux_dirent64).

Here's an updated version of the code using SYS_getdents:
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

#define BUF_SIZE 1024

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
    char d_type;

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
            printf("%s", d->d_name); // Print file/directory name

            // We cannot use d_type directly here like in getdents64, so we use stat instead:
            struct stat st;
            if (stat(d->d_name, &st) == 0) {
                if (S_ISREG(st.st_mode))
                    printf(" (regular file)\n");
                else if (S_ISDIR(st.st_mode))
                    printf(" (directory)\n");
                else if (S_ISLNK(st.st_mode))
                    printf(" (symlink)\n");
                else
                    printf(" (other)\n");
            } else {
                printf(" (unknown)\n");
            }

            bpos += d->d_reclen;
        }
    }

    close(fd);
    return 0;
}

