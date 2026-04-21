#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <pthread.h>

#include "logger.h"
#include "file_writer.h"

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;


void timer_callback(union sigval sv)
{
    
    (void)sv;
    printf("Timer fired!\n");

    if (pthread_mutex_trylock(&lock) != 0) {
        printf("Callback still running, skipping...\n");
        return;
    }
    
    SystemStats stats = logger_starter();
    file_writer(stats);

    pthread_mutex_unlock(&lock);

}


void start_timer(void)
{
   
    struct sigevent sev;
    memset(&sev, 0, sizeof(sev));
    sev.sigev_notify           = SIGEV_THREAD;
    sev.sigev_notify_function  = timer_callback;
    sev.sigev_value.sival_ptr  = NULL;

    timer_t timer_id;
    if (timer_create(CLOCK_MONOTONIC, &sev, &timer_id) != 0) {
        perror("timer_create");
        exit(EXIT_FAILURE);
    }
    
    printf("Timer started. Press Ctrl+C to stop.\n");


    struct itimerspec its;
    its.it_value.tv_sec     = 1;   /* first firing: after 2 seconds */
    its.it_value.tv_nsec    = 0;
    its.it_interval.tv_sec  = 3;   /* repeat every 2 seconds */
    its.it_interval.tv_nsec = 0;

    if (timer_settime(timer_id, 0, &its, NULL) != 0) {
        perror("timer_settime");
        exit(EXIT_FAILURE);
    }



    while (1) pause();

    timer_delete(timer_id);
    
}