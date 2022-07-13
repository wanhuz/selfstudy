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

    fdw = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);

    if (fdw < 0) {
        printf("Error in opening file: %s", argv[2]);
        close(fd);
        exit(4);
    }



    while ((count = read(fd, buf, SIZE)) != -1 && count != 0) {
        for (size_t i = 0; i < count; i++) {
            xor ^= buf[i];
        }

        if (write(fdw, &xor, 1) == -1) {
            printf("Write error");
            close(fd);
            close(fdw);
            exit(6);
        }


    }

    printf("Checksum write successful");

    close(fd);
    close(fdw);

    return 0;
}
