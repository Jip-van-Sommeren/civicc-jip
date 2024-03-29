#include "globals.h"
#include <stdio.h>

struct globals global;

/*
 * Initialize global variables from globals.mac
 */

void GLBinitializeGlobals()
{
    global.col = 0;
    global.line = 0;
    global.input_file = NULL;
    global.output_file = NULL;
    global.outputFile = NULL; // Initialize to NULL
}

void openGlobalOutputFile()
{
    if (global.outputFile == NULL && global.output_file != NULL)
    {
        global.outputFile = fopen(global.output_file, "w");
        if (global.outputFile == NULL)
        {
            // Handle error, could not open file
        }
    }
}

void closeGlobalOutputFile()
{
    if (global.outputFile != NULL)
    {
        fclose(global.outputFile);
        global.outputFile = NULL;
    }
}