#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <pthread.h>


#include "logger.h"

pthread_mutex_t file_lock = PTHREAD_MUTEX_INITIALIZER;


void file_writer(SystemStats stats)
{

    int fd = open("/tmp/sysmon.log", O_WRONLY | O_CREAT | O_APPEND, 0644);

    if (fd < 0)
    {
        perror("open");
        exit(EXIT_FAILURE);
    }


    char timebuf[32];
    time_t now = time(NULL);
    struct tm *tm  = localtime(&now);
    strftime(timebuf, sizeof(timebuf), "%Y-%m-%d %H:%M:%S", tm);


    char buf[256];
    int  len;

    len = snprintf(buf, sizeof(buf),
        "[%s] [INFO] CPU Usage  : %.2f%%\n"
        "[%s] [INFO] RAM Usage  : %.2f%%\n"
        "[%s] [INFO] Up Time (s): %ld\n"
        "=================================================\n",
        timebuf, stats.cpu,
        timebuf, stats.ram,
        timebuf, stats.uptime);


    pthread_mutex_lock(&file_lock);
    write(fd, buf, len); 
    pthread_mutex_unlock(&file_lock);

    
    close(fd);

}