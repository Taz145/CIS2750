#include "listio.h"
#include <stdio.h>

#define MAX_BUFF 1024

int main (int argc, char **argv) {
    if (argc == 1) {
        printf("Please add the filename and run again\n");
        return (FAILURE);
    }
    char * filename = calloc(1, sizeof(char) * strlen(argv[1]) + strlen(".html") + 1);
    strcat(filename, argv[1]);
    strcat(filename, ".html");

    char BUFF[MAX_BUFF];
    FILE *file;
    file = fopen(filename, "r");
    if (file != NULL) {
        while (fgets(BUFF, MAX_BUFF, file) != NULL) {
            printf("%s", BUFF);
        }
        fclose(file);
        free(filename);
        return SUCCESS;
    }
    file = fopen(argv[1], "r");
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
    setName(rtn->header, argv[1]);
    while (fgets(BUFF, MAX_BUFF, file) != NULL) {
        if(strlen(BUFF) != 0) {
            addString(rtn->header, BUFF);
        }
    }
    fclose(file);
    processStrings(rtn->header);
    freeStructure(rtn->header);
    free(rtn);
    free(filename);
    return (SUCCESS);
}
