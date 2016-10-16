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
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Couldn't open the file. Does it exist?\n");
        return FAILURE;
    }
    struct returnStruct *rtn;
    rtn = buildHeader();
    if (rtn->value == FAILURE) {
        printf("Error on buildHeader(). Exiting\n");
        return (FAILURE);
    }
    setName(rtn->header, filename);
    while (fgets(BUFF, MAX_BUFF, file) != NULL) {
        addString(rtn->header, BUFF);
    }
    fclose(file);
    processStrings(rtn->header);
    freeStructure(rtn->header);
    free(rtn);
    return (SUCCESS);
}
