#define _POSIX_C_SOURCE 200809L

#include "logs.h"

#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <sys/stat.h>

static FILE *log_file = NULL;

static void write_log(const char *level,
                      const char *fmt,
                      va_list args)
{
    if (!log_file)
        return;

    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);

    fprintf(log_file,
            "[%04d-%02d-%02d %02d:%02d:%02d] [%s] ",
            tm_info->tm_year + 1900,
            tm_info->tm_mon + 1,
            tm_info->tm_mday,
            tm_info->tm_hour,
            tm_info->tm_min,
            tm_info->tm_sec,
            level);

    vfprintf(log_file, fmt, args);

    fprintf(log_file, "\n");

    fflush(log_file);
}

void log_init(void)
{
    mkdir("logs", 0755);

    log_file = fopen("logs/server.log", "a");

    if (!log_file) {
        perror("Cannot open logs/server.log");
    }
}

void log_close(void)
{
    if (log_file) {
        fclose(log_file);
        log_file = NULL;
    }
}

void log_info(const char *fmt, ...)
{
    va_list args;

    va_start(args, fmt);
    write_log("INFO", fmt, args);
    va_end(args);
}

void log_error(const char *fmt, ...)
{
    va_list args;

    va_start(args, fmt);
    write_log("ERROR", fmt, args);
    va_end(args);
}