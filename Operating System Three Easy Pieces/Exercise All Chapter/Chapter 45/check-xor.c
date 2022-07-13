#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>


int main(int argc, char *argv[])
{
    unsigned char xor = 0;
    int fd;
    unsigned char buf[1] = "";
    unsigned char chksksum[8];
    int count;

    fd = open("test.txt", O_RDONLY);

    if (fd < 0) {
        printf("Error in opening file");
        exit(1);
    }



    while ((count = read(fd, buf, 1)) != -1 && count != 0) {
        if (buf[0] != "\n")
            xor ^= (buf[0] - '0'); //XOR'ing decimal is equivalent to XOR'ing binary
                                   //- '0' to convert char into number by substracting 0s of ANSI

    }

    printf("XOR: %d\n", xor);




    close(fd);

    return 0;
}
