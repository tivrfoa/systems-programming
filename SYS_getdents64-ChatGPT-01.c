/*

Q: write a C code that uses the syscall getdents to list the files and directories in current directory



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
    struct linux_dirent64 {
        ino64_t d_ino;
        off64_t d_off;
        unsigned short d_reclen;
        unsigned char d_type;
        char d_name[];
    };
    struct linux_dirent64 *d;
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
        nread = syscall(SYS_getdents64, fd, buf, BUF_SIZE);
        if (nread == -1) {
            perror("getdents");
            exit(EXIT_FAILURE);
        }

        if (nread == 0) // End of directory
            break;

        for (bpos = 0; bpos < nread;) {
            d = (struct linux_dirent64 *) (buf + bpos);
            printf("%s", d->d_name); // Print file/directory name

            d_type = d->d_type;
            switch (d_type) {
                case DT_REG:  printf(" (regular file)\n"); break;
                case DT_DIR:  printf(" (directory)\n"); break;
                case DT_LNK:  printf(" (symlink)\n"); break;
                default:      printf(" (other)\n"); break;
            }

            bpos += d->d_reclen;
        }
    }

    close(fd);
    return 0;
}

