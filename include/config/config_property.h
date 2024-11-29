#ifndef CONFIG_PROPERTY_H
#define CONFIG_PROPERTY_H

#define MAX_PATH_LENGTH 256
#define MAX_VALUE_LENGTH 1024

typedef struct {
    char path[MAX_PATH_LENGTH];
    char value[MAX_VALUE_LENGTH]; 
} ConfigurationProperty;

#endif // CONFIG_PROPERTY_H