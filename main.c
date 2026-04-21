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

int main(void)
{

    daemonize();
    
    create_socket();

    return 0;

}