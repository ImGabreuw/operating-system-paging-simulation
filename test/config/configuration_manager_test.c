#include <stdio.h>
#include <stdlib.h>

#include "config_manager.h"
#include "log.h"

void test_properties_load()
{
    log_init("test_properties_load.log");

    ConfigurationManager config;

    if (load_properties(&config, "simulator.properties") != 0)
    {
        log_message(LOG_ERROR, "Falha ao carregar o arquivo de configuração.");
        return;
    }

    log_message(LOG_INFO, "Properties loaded: %d", config.properties_loaded);

    char *memory_size = (char *)get_property(&config, "memory.size");
    if (memory_size != NULL)
    {
        log_message(LOG_INFO ,"Tamanho da memória: %s", memory_size);
    }
    else
    {
        log_message(LOG_INFO, "Propriedade 'memory.size' não encontrada.");
    }

    // Libera a memória alocada
    free_properties(&config);

    log_cleanup();
}

int main(void)
{
    test_properties_load();

    return EXIT_SUCCESS;
}