#include "log.h"

#include <stdarg.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/time.h>

static FILE *log_file = NULL;
static struct timeval start_time; // Usa timeval para compatibilidade com sistemas mais simples

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

/**
 * Função para obter o tempo decorrido desde a inicialização do log (em milissegundos).
 */
static long get_elapsed_time_ms()
{
    struct timeval current_time;
    gettimeofday(&current_time, NULL); // Obtém o tempo atual em segundos e microssegundos

    long seconds = current_time.tv_sec - start_time.tv_sec;
    long microseconds = current_time.tv_usec - start_time.tv_usec;
    return seconds * 1000.0 + microseconds / 1000.0;
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

    gettimeofday(&start_time, NULL);
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

    long elapsed_time = get_elapsed_time_ms();

    fprintf(log_file, "[%s] [%s] [Clock %ld ms] ", get_current_time(), level_strings[level], elapsed_time);
    vfprintf(log_file, format, args);
    fprintf(log_file, "\n");

    va_end(args);
    fflush(log_file);

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
