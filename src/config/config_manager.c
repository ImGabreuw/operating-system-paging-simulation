#include "config_manager.h"

#include "log.h"

int load_properties(ConfigurationManager *manager, const char *filePath)
{
    FILE *file = fopen(filePath, "r");
    if (file == NULL)
    {
        log_message(LOG_ERROR, "Erro ao abrir arquivo de configuração");
        return -1;
    }

    char line[512];
    manager->properties_loaded = 0;

    while (fgets(line, sizeof(line), file) != NULL && manager->properties_loaded < MAX_PROPERTIES)
    {
        if (line[0] == '#' || line[0] == '\n')
        {
            continue; // Ignora comentários e linhas vazias
        }

        // Encontra o delimitador "="
        char *delimiter = strchr(line, '=');
        if (delimiter == NULL)
        {
            continue; // Linha inválida, ignora
        }

        int count = 0;
        for (char *p = strtok(line, "="); p != NULL; p = strtok(NULL, " "))
        {
            if (count == 0)
            {
                strncpy(manager->properties[manager->properties_loaded].path, p, MAX_PATH_LENGTH - 1);
                manager->properties[manager->properties_loaded].path[MAX_PATH_LENGTH - 1] = '\0';
            }

            if (count == 1)
            {
                strncpy(manager->properties[manager->properties_loaded].value, p, MAX_VALUE_LENGTH - 1);
                manager->properties[manager->properties_loaded].value[MAX_VALUE_LENGTH - 1] = '\0';
            }

            ++count;
        }

        manager->properties_loaded++;
    }

    log_message(LOG_INFO, "Foram carregadas %d configurações do simulador.", manager->properties_loaded);

    fclose(file);
    return 0;
}

char *get_property(ConfigurationManager *manager, const char *path)
{
    for (int i = 0; i < manager->properties_loaded; i++)
    {
        if (strcmp(manager->properties[i].path, path) == 0)
        {
            return manager->properties[i].value;
        }
    }
    return NULL;
}

void free_properties(ConfigurationManager *manager)
{
    for (int i = 0; i < manager->properties_loaded; i++)
    {
        free(manager->properties[i].value);
    }
}
