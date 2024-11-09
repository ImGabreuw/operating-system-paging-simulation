#include "test_utils.h"

#include <stdio.h>

#define COLOR_GREEN "\033[0;32m"
#define COLOR_RED "\033[0;31m"
#define COLOR_RESET "\033[0m"

void test_passed(const char *test_name)
{
    printf("%s%s PASSED%s\n", COLOR_GREEN, test_name, COLOR_RESET);
}

void test_failed(const char *test_name)
{
    printf("%s%s FAILED%s\n", COLOR_RED, test_name, COLOR_RESET);
}
