#pragma once

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

struct globals
{
    int line;
    int col;
    int verbose;
    char *input_file;
    char *output_file;
    FILE *outputFile;
};

extern struct globals global;
extern void GLBinitializeGlobals(void);
void openGlobalOutputFile(void); // Declare the function
void closeGlobalOutputFile(void);