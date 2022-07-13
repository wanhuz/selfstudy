#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>


int main(int argc, char *argv[])
{
    int fd;
    unsigned char buf[1] = "";
    unsigned char chksksum[8];
    int count;

    fd = open("test.txt", O_RDONLY);

    if (fd < 0) {
        printf("Error in opening file");
        exit(1);
    }

    int a = 0;
    int b = 0;


    while ((count = read(fd, buf, 1)) != -1 && count != 0) {
        if (buf[0] != "\n") {
            a = ((a + (buf[0] - '0')) % 255);
            b = ((b + a) % 255);
        }
    }



    printf("Fletcher Checksum: %d%d\n",  a, b);




    close(fd);

    return 0;
}
