#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <pthread.h>

#include "socket.h"
#include "logger.h"
#include "file_writer.h"
#include "timer.h"
#include "daemon.h"

int main(int argc, char **argv)
{
    (void)argc;

    

    if (argc < 2) {
        printf("Usage: ./sysmon <option>\n");
        return 1;
    }

    daemonize();
    
    if (strcmp(argv[1], "timer") == 0)
    {
        printf("Timer Option!\n");
        start_timer();
    }
    else if (strcmp(argv[1], "socket") == 0)
    {
        printf("Socket Option!\n");
        create_socket();
    }
    else
    {
        printf("Unknown option: %s\n", argv[1]);
    }

    return 0;

}