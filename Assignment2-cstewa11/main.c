#include "listio.h"
#include <stdio.h>

#define MAX_BUFF 1024

int main (int argc, char **argv) {
    if (argc == 1) {
        printf("Please add the filename and run again\n");
        return (FAILURE);
    }
    char * filename = argv[1];
    char BUFF[MAX_BUFF];
    FILE *file  = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file. Exiting\n");
        return (FAILURE);
    }
    struct returnStruct *rtn;
    rtn = buildHeader();
    if (rtn->value == FAILURE) {
        printf("Error on buildHeader(). Exiting\n");
        return (FAILURE);
    }
    setName(rtn->header, "Sample Header");
    while (fgets(BUFF, MAX_BUFF, file) != NULL) {
        addString(rtn->header, BUFF);
    }
    fclose(file);
    addString(rtn->header, "\ntest\n\0");
    addString(rtn->header, "\n\nnother test\0");
    // printString(rtn->header);
    processStrings(rtn->header);
    printString(rtn->header);
    freeStructure(rtn->header);
    free(rtn);
    return (SUCCESS);
}