#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>

#define SIZE 4096

int main(int argc, char *argv[])
{
    unsigned char xor = 0;
    int fd, fdw;
    unsigned char buf[SIZE] = "";
    unsigned char buftwo = 0;
    int count;


    if (argc < 3) {
        printf("Not enough argument. File input and File output required");
        exit(3);
    }

    fd = open(argv[1], O_RDONLY);

    if (fd < 0) {
        printf("Error in opening file: %s", argv[1]);
        exit(1);
    }

    fdw = open(argv[2], O_RDONLY);

    if (fdw < 0) {
        printf("Error in opening file: %s", argv[2]);
        close(fd);
        exit(4);
    }



    while ((count = read(fd, buf, SIZE)) != -1 && count != 0) {
        for (size_t i = 0; i < count; i++) {
            xor ^= buf[i];
        }

        if ((count = read(fdw, &buftwo, 1)) == -1) {
            printf("Error in reading file: %s\n", argv[2]);
            break;
        }

        if (xor != buftwo) {
            printf("File corruption detected");
            close(fd);
            close(fdw);
            exit(6);
        }

    }

    printf("Checksum complete");

    close(fd);
    close(fdw);

    return 0;
}
