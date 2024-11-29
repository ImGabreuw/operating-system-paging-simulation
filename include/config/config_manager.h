#ifndef CONFIG_MANAGER_H
#define CONFIG_MANAGER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "config_property.h"

#define MAX_PROPERTIES 100

typedef struct {
    ConfigurationProperty properties[MAX_PROPERTIES];
    int properties_loaded;
} ConfigurationManager;

/**
 * Carrega as propriedades de configuração a partir de um arquivo.
 * 
 * @param manager Ponteiro para a estrutura ConfigurationManager.
 * @param filePath Caminho do arquivo de configuração a ser carregado.
 * @return 0 se o carregamento for bem-sucedido, -1 caso contrário.
 */
int load_properties(ConfigurationManager* manager, const char* filePath);

/**
 * Retorna o valor da propriedade correspondente ao caminho fornecido.
 * 
 * @param manager Ponteiro para a estrutura ConfigurationManager.
 * @param path Caminho da propriedade a ser buscada.
 * @return Ponteiro para o valor da propriedade, ou NULL se não for encontrada.
 */
char* get_property(ConfigurationManager* manager, const char* path);

/**
 * Libera a memória alocada para os valores das propriedades.
 * 
 * @param manager Ponteiro para a estrutura ConfigurationManager.
 */
void free_properties(ConfigurationManager* manager);

#endif // CONFIG_MANAGER_H
