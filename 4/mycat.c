#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[])
{
    int fd;
    ssize_t n;

    char buf[BUFSIZ] = {};

    if (argc == 1)
    {
        while ((n = read(STDIN_FILENO, buf, BUFSIZ)) > 0)
        {
            write(STDOUT_FILENO, buf, n);
        }
    }
    else if (argc == 2)
    {
        if ((fd = open(argv[1], O_RDONLY)) < 0)
        {
            perror("Open error");
            exit(EXIT_FAILURE);
        }

        while ((n = read(fd, buf, BUFSIZ)) > 0)
        {
            write(STDOUT_FILENO, buf, n);
        }
    }

    exit(EXIT_SUCCESS);
}