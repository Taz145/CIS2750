#include "listio.h"
#include <stdio.h>

#define MAX_BUFF 1024

int main (int argc, char **argv) {
    if (argc == 1) {
        printf("Please add the filename and run again\n");
        return (FAILURE);
    }
    char * filename = argv[1];
    struct returnStruct *rtn;
    rtn = buildHeader();
    if (rtn->value == FAILURE) {
        printf("Error on buildHeader(). Exiting\n");
        return (FAILURE);
    }
    setName(rtn->header, filename);
    addString(rtn->header, "\ntest\n\0");
    addString(rtn->header, "\n   \nThis is a test String\0");
    addString(rtn->header, "\nanother   test      String\0");
    processStrings(rtn->header);
    printString(rtn->header);
    freeStructure(rtn->header);
    free(rtn);
    return (SUCCESS);
}
