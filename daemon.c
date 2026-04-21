#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

void daemonize(void)
{
    pid_t pid;

    pid = fork();
    if (pid < 0)  exit(EXIT_FAILURE);
    if (pid > 0)  exit(EXIT_SUCCESS);

    if (setsid() < 0)
        exit(EXIT_FAILURE);

    pid = fork();
    if (pid < 0)  exit(EXIT_FAILURE);
    if (pid > 0)  exit(EXIT_SUCCESS); 

    umask(0);

    if (chdir("/") < 0)
        exit(EXIT_FAILURE);


    close(STDIN_FILENO);
    close(STDOUT_FILENO);
    close(STDERR_FILENO);

    int devnull = open("/dev/null", O_RDWR);
    dup2(devnull, STDIN_FILENO);
    dup2(devnull, STDOUT_FILENO);
    dup2(devnull, STDERR_FILENO);
    if (devnull > 2) close(devnull);

}
