#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/time.h>


int main(int argc, char *argv[])
{
    struct timeval time;
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

    int rc = gettimeofday(&time, NULL);

    if (rc != 0) {
        printf("Error in get time");
        exit(2);
    }

    double t_start = (double)time.tv_sec + ((double)time.tv_usec / 1e6);
    while ((count = read(fd, buf, 1)) != -1 && count != 0) {
        if (buf[0] != "\n") {
            a = ((a + (buf[0] - '0')) % 255);
            b = ((b + a) % 255);
        }
    }

    rc = gettimeofday(&time, NULL);

    if (rc != 0) {
        printf("Error in get time");
        exit(2);
    }

    double t_end = (double)time.tv_sec + ((double)time.tv_usec / 1e6);


    printf("Fletcher Checksum: %d%d\n",  a, b);
    printf("Time (secs): %f\n",  t_end - t_start);




    close(fd);

    return 0;
}
