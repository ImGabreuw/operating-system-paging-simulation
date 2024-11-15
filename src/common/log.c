#include "log.h"

#include <stdarg.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/time.h>

static FILE *log_file = NULL;

static int start_time;

static pthread_mutex_t log_mutex;

static const char *get_current_time()
{
    static char buffer[20];
    time_t rawtime;
    struct tm *timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);

    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeinfo);
    return buffer;
}

void log_init(const char *filename)
{
    if (log_file != NULL)
    {
        fclose(log_file);
    }

    log_file = fopen(filename, "w");
    if (log_file == NULL)
    {
        perror("Não foi possível abrir o arquivo de log");
        exit(EXIT_FAILURE);
    }

    pthread_mutex_init(&log_mutex, NULL);
    start_time = 0;
}

void log_message(LogLevel level, const char *format, ...)
{
    if (log_file == NULL)
    {
        return;
    }

    const char *level_strings[] = {"INFO", "WARNING", "ERROR"};

    pthread_mutex_lock(&log_mutex);

    va_list args;
    va_start(args, format);

    fprintf(log_file, "[%s] [%s] [Clock %d] ", get_current_time(), level_strings[level], start_time);
    vfprintf(log_file, format, args);
    fprintf(log_file, "\n");

    va_end(args);
    fflush(log_file);
    
    ++start_time;

    pthread_mutex_unlock(&log_mutex);
}

void log_cleanup()
{
    if (log_file != NULL)
    {
        fclose(log_file);
        log_file = NULL;
    }

    pthread_mutex_destroy(&log_mutex);
}
