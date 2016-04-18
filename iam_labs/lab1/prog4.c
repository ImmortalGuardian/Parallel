#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

const int bufsize=1024;

int main(int argc, char *argv[])
{
    int fd=open(argv[1], O_RDONLY);
    void *buf=calloc(bufsize, 1);

    while (read(fd, buf, bufsize)>0)
        write(STDOUT_FILENO, buf, bufsize);

    return 0;
}
