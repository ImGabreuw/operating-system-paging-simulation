#include "log.h"

#include <stdarg.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

static FILE *log_file = NULL;

/**
 * O uso de um mutex no log é suficiente para garantir a exclusão mútua durante a escrita no arquivo, evitando sobreposição de logs entre threads. 
 * Não é necessário usar uma variável de condição (pthread_cond_t), pois não há dependência de estados ou gerenciamento entre threads nesse contexto.
 */
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

    log_file = fopen(filename, "a");
    if (log_file == NULL)
    {
        perror("Não foi possível abrir o arquivo de log");
        exit(EXIT_FAILURE);
    }

    pthread_mutex_init(&log_mutex, NULL);
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

    fprintf(log_file, "[%s] [%s] [Thread %ld] ", get_current_time(), level_strings[level], pthread_self());
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
