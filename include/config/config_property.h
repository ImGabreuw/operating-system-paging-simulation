#ifndef CONFIG_PROPERTY_H
#define CONFIG_PROPERTY_H

#define MAX_PATH_LENGTH 256

typedef struct {
    char path[MAX_PATH_LENGTH];
    void* value; 
} ConfigurationProperty;

#endif // CONFIG_PROPERTY_H