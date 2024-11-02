#include <stdio.h>
#include <stdlib.h>

#include "log.h"

/*
Integrantes:

Enzo Ribeiro                    - 10418262
Gabriel Ken Kazama Geronazzo    - 10418247
Lucas Pires de Camargo Sarai    - 10418013
Lucas Zanini da Silva           - 10417361

Código fonte: https://github.com/ImGabreuw/operating-system-paging-simulation
*/


int main(int argc, char const *argv[])
{
    const char log_filename[] = "log.txt";
    log_init(log_filename);

    log_message(LOG_INFO, "OS started...");

    log_cleanup();

    return EXIT_SUCCESS;
}