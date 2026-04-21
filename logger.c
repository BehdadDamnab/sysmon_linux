#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <pthread.h>

#include "logger.h"


CpuOut cpu_usage(void)
{

    CpuStats s;
    CpuOut CpuOut_;

    int fd = open("/proc/stat", O_RDONLY);

    if (fd < 0) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    char buf[128] = {0};
    char c;
    int i = 0;
    while (read(fd, &c, 1) > 0) 
    {
        buf[i++] = c;
        if (c == '\n') break;
    }
    buf[i] = '\0';

    // printf("CPU out: %s\n", buf);

    sscanf(buf, "cpu  %ld %ld %ld %ld %ld %ld %ld",
        &s.user, &s.nice, &s.system,
        &s.idle, &s.iowait, &s.irq, &s.softirq);

    // printf("%ld\n", s.softirq);
    CpuOut_.idle = s.idle + s.iowait;
    CpuOut_.total = s.user + s.nice + s.system + s.idle + s.iowait + s.irq + s.softirq;

    close(fd);

    return CpuOut_;
}

float ram_usage(void)
{

    long total_ram;
    long available_ram;

    int fd = open("/proc/meminfo", O_RDONLY);

    if (fd < 0) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    char c;

    char ram_s[128] = {0};
    int i = 0;
    while (read(fd, &c, 1) > 0) 
    {
        ram_s[i++] = c;
        if (c == '\n') break;
    }
    ram_s[i] = '\0';

    // printf("%s", ram_s);

    sscanf(ram_s, "MemTotal: %ld kB", &total_ram);
    // printf("%ld\n", total_ram);


    while (read(fd, &c, 1) > 0) 
    {
        if (c == '\n') break;
    }


    memset(&ram_s, 0, sizeof(ram_s));
    i = 0;
    while (read(fd, &c, 1) > 0) 
    {
        ram_s[i++] = c;
        if (c == '\n') break;
    }
    ram_s[i] = '\0';
    // printf("%s", ram_s);

    sscanf(ram_s, "MemAvailable: %ld kB", &available_ram);
    // printf("%ld\n", available_ram);

    close(fd);

    return (1 - ((float)available_ram/(float)total_ram))*100;

}


long up_time(void)
{

    int fd = open("/proc/uptime", O_RDONLY);
    
    if (fd < 0) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    long UpTime;

    char c;
    char buf[32] = {0};
    int i = 0;
    while (read(fd, &c, 1) > 0) 
    {
        buf[i++] = c;
        if (c == '.') break;
    }
    buf[i] = '\0';
    sscanf(buf, "%ld", &UpTime);
    
    close(fd);

    return UpTime;

}


void *monitor(void *arg)
{

    (void)arg;

    CpuOut cpu_out_1, cpu_out_2;


    cpu_out_1 = cpu_usage();

    sleep(2);

    cpu_out_2 = cpu_usage();

    double *cpu_usage_persentage = malloc(sizeof(double));
    *cpu_usage_persentage = (1.0 - (double)(cpu_out_2.idle - cpu_out_1.idle) / (cpu_out_2.total - cpu_out_1.total)) * 100.0;
    // printf("CPU Usage: %.2f%%\n", *cpu_usage_persentage);

    return cpu_usage_persentage;

}


SystemStats logger_starter(void)
{

    SystemStats stats;
    void *retval;

    pthread_t tid;
    pthread_create(&tid, NULL, monitor, NULL);

    stats.ram = ram_usage();
    // printf("Ram Usage: %.2f%%\n", stats.ram);

    stats.uptime = up_time();
    // printf("Up Time (s): %ld\n", UpTime);

    pthread_join(tid, &retval);
    stats.cpu = *(double *)retval;
    free(retval);

    return stats;

}