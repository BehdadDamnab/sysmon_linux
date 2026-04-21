#ifndef LOGGER_H
# define LOGGER_H

typedef struct {
    long user;
    long nice;
    long system;
    long idle;
    long iowait;
    long irq;
    long softirq;
} CpuStats;

typedef struct {
    long total;
    long idle;
} CpuOut;


typedef struct {
    double  cpu;
    float   ram;
    long    uptime;
} SystemStats;

CpuOut      cpu_usage(void);       // ← was float, now CpuOut
float       ram_usage(void);
long        up_time(void);
void        *monitor(void *arg);
SystemStats logger_starter(void);  // ← was void, now SystemStats

#endif