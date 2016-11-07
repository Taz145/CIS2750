#include "listio.h"

#define MAX_BUFF 1024
/* Helper function to processStrings()
 * Will remove all but 1 of the specifed character, char toRemove.
 * The characters must be sequential
 * e.g abaaabaa -> ababa
 */
static void removeChar(char *oldString, char toRemove);

/* Helper function to processStrings()
 * Will replace any single newlines or carriage returns with '<BR>'
 * Will replace sequential with '<P>'
 * e.g. h\ne\n\nllo -> h<BR>e<P>llo
 * Takes the char * to be parsed as an argurment
 * Returns a char * to the parsed string
 * If any errors are encountered, the function returns NULL
 */
static char *parseNewLines(char *oldString);

/*Helper function to processStrings()
 *Removes any newlines from the beginning of a string if the previous string
 *ended with newlines. Ignores whitespace between newline characters
 *e.g "hello/n" and "/ngoodbye" -> "hello/n" and "goodbye"
 */
static void removeNewLines(struct dataHeader *header);

static void pyFormatting(struct dataHeader *header);



/*
 * This function will return a pointer of type struct dataHeader
 * All variables will be set to NULL/0 initially
 * No arguments are taken
 * If an error occurs the functions NULL
 */
struct returnStruct *buildHeader() {
    struct returnStruct *rtn;
    rtn = malloc(sizeof(struct returnStruct));
    if (rtn == NULL) {
        return NULL;
    }
    rtn->header = malloc(sizeof(struct dataHeader));
    //Checks if malloc worked
    if (rtn->header == NULL) {
        rtn->value = FAILURE;
        return rtn;
    }
    //set to default values
    rtn->header->name = NULL;
    rtn->header->length = 0;
    rtn->header->next = NULL;
    rtn->value = SUCCESS;
    return rtn;
}
/*
 * This funtion sets the name variable in specified struct dataHeader
 * Exits early if the string or header passed is NULL
 * Takes a pointer to the struct dataHeader that is being used and the string
 */
int setName(struct dataHeader *header, char *name) {
    if (header == NULL) {
        return FAILURE;
    }
    if (name == NULL) {
        return FAILURE;
    }

    //Subtract from length if there is already a name
    if (header->name != NULL) {
        header->length -= strlen(header->name);
    }

    //free the header in case of old name
    free(header->name);
    header->name = malloc(sizeof(char) * strlen(name) + 1);
    //copy name to header and update total length
    strcpy(header->name, name);
    header->length += strlen(header->name);
    return SUCCESS;
}

/*
 * Function used to get the current value stored in header->name
 * Takes a pointer to the desired struct dataHeader
 * If an error occurs, the function returns NULL
 */
char *getName(struct dataHeader *header) {
    if (header == NULL) {
        return NULL;
    }
    if (header->name == NULL) {
        return NULL;
    }
    return header->name;
}

/*
 * Function to get to current total length of all strings in the list
 * Takes a pointer to the desired struct dataHeader
 * If an error occurs the function returns FAILURE
 */
int getLength(struct dataHeader *header) {

    if (header == NULL) {
        return FAILURE;
    }

    return header->length;
}

/*
 * Used to add a new string to the list
 * Creates a new struct dataString and links it to the end of the list
 * Exits early if the passed string is NULL
 * Takes a pointer to the desired struct dataHeader and the string to be added
 */
int addString(struct dataHeader *header, char *str) {
    //dataStrings as a placeholder to loop though the list and a new entry
    struct dataString *endOfList;
    struct dataString *new = malloc(sizeof(struct dataString));
    //checks if malloc worked
    if (new == NULL) {
        return FAILURE;
    }
    //checks if the string is valid
    if (str == NULL) {
        free(new);
        return FAILURE;
    }
    new->next = NULL;
    new->string = malloc(sizeof(char) * strlen(str) + 1);
    //check if malloc worked
    if (new->string == NULL) {
        free(new);
        return FAILURE;
    }
    strcpy(new->string, str);
    new->string[strlen(str)] = '\0';
    endOfList = header->next;
    //checks if there are any existing dataStrings in the list already
    if (endOfList == NULL) {
        header->next = new;
    } else {
        //if there are adds the new string to the end of the list
        while (endOfList->next != NULL) {
            endOfList = endOfList->next;
        }
        endOfList->next = new;
    }
    //Update length in header
    header->length += strlen(new->string);
    return SUCCESS;
}
/*
 * Used to print out all the strings in the list
 * prints strings in the order they were added
 * takes a pointer to the desired struct dataHeader
 */
int printString(struct dataHeader *header) {
    if (header == NULL) {
        return FAILURE;
    }
    //dataString to loop through the list
    struct dataString *list = header->next;

    //loops through list until it hits the end
    while (list != NULL) {
        printf("%s\n", list->string);
        list = list->next;
    }
    printf("\n---End of List---\n");
    return SUCCESS;
}
/*
 * Parses each string and removes or changes certain characters
 * Multiple consecutive spaces and tabs will be condensed into 1
 * a single linebreak will be replaced with '<BR>'
 * multiple linebreaks will be replaces with '<P>'
 * takes a pointer to the desired struct dataHeader
 */
int processStrings (struct dataHeader *header) {
    if (header == NULL) {
        return FAILURE;
    }

    struct dataString * temp = header->next;
    int oldLength, newLength;
    //used to pass values of strings when calling removeNewLines()
    char *placeHolder;
    removeNewLines(header);
    while (temp != NULL) {
        //gets old length of each string
        oldLength = strlen(temp->string);
        removeChar(temp->string, 32);
        removeChar(temp->string, 9);

        placeHolder = malloc(sizeof(char) * strlen(temp->string) + 1);
        //check that malloc worked
        if (placeHolder == NULL) {
            return FAILURE;
        }
        strcpy(placeHolder, temp->string);
        free(temp->string);
        temp->string = parseNewLines(placeHolder);
        free(placeHolder);
        //gets new length and updates header->length
        newLength = strlen(temp->string);
        header->length -= (oldLength - newLength);

        temp = temp->next;
    }
    pyFormatting(header);
    return SUCCESS;
}

/* Helper function to processStrings()
 * Will remove all but 1 of the specifed character, char toRemove.
 * The characters must be sequential
 * e.g abaaabaa -> ababa
 */
static void removeChar (char *oldString, char toRemove) {
    char *newString = malloc(sizeof(char) * strlen(oldString) + 1);
    //check that malloc worked
    if (newString == NULL) {
        return;
    }
    int k = 0;
    bool prevChar = false;

    for (int x = 0; x < strlen(oldString) + 1; x++) {
        if (oldString[x] != toRemove) {
            newString[k++] = oldString[x];
            prevChar = false;
        } else if (oldString[x] == toRemove) {
            if (!prevChar) {
                newString[k++] = oldString[x];
                prevChar = true;
            }
        }
    }
    strcpy(oldString, newString);
    free(newString);
}

/* Helper function to processStrings()
 * Will replace any single newlines or carriage returns with '<BR>'
 * Will replace sequential with '<P>'
 * e.g. h\ne\n\nllo -> h<BR>e<P>llo
 * If any errors are encountered, the function returns NULL
 */
static char *parseNewLines (char *oldString) {
    int spaceNeeded = 0, length = strlen(oldString);
    bool currChar = false, nextChar = false;
    char *newString = malloc(sizeof(char) * 1);

    //check that malloc worked
    if (newString == NULL) {
        return NULL;
    }

    for (int i = 0; i < length; i++) {
        //booleans to make 'if' statements cleaner
        currChar = (oldString[i] == 10 || oldString[i] == 13);
        nextChar = (oldString[i + 1] == 10 || oldString[i + 1] == 13);
        //single newLine
        if (currChar && !nextChar) {
            spaceNeeded += 4;
            //Check that realloc found memory
            if ( (newString = realloc(newString, sizeof(char) * spaceNeeded) ) == NULL)  {
                free(newString);
                return NULL;
            }
            newString[spaceNeeded - 4] = '<';
            newString[spaceNeeded - 3] = 'B';
            newString[spaceNeeded - 2] = 'R';
            newString[spaceNeeded - 1] = '>';
            //Multiple newLines
        } else if (currChar && nextChar) {
            spaceNeeded += 3;
            if ( (newString = realloc(newString, sizeof(char) * spaceNeeded) ) == NULL)  {
                free(newString);
                return NULL;
            }
            newString[spaceNeeded - 3] = '<';
            newString[spaceNeeded - 2] = 'P';
            newString[spaceNeeded - 1] = '>';

            //skip over all the other newlines in the sequence
            while (oldString[i + 1] == 10 || oldString[i + 1] == 13 || oldString[i + 1] == 32 || oldString[i + 1] == 9) {
                i++;
            }
            //Non-newLine character
        } else {
            spaceNeeded++;
            if ( (newString = realloc(newString, sizeof(char) * spaceNeeded) ) == NULL)  {
                free(newString);
                return NULL;
            }
            newString[spaceNeeded - 1] = oldString[i];
        }
    }
    //add space for null character
    if ( (newString = realloc(newString, sizeof(char) * spaceNeeded + 1) ) == NULL)  {

        free(newString);
        return NULL;
    }
    newString[spaceNeeded] = '\0';

    return (newString);
}

static void removeNewLines (struct dataHeader *header) {
    if (header == NULL || header->name == NULL
            || header->next == NULL || header->next->string == NULL) {
        return;
    }
    int i = 0, k = 0;
    struct dataString *list = header->next;
    struct dataString *nextString = list->next;
    char *tempString;
    bool prevString = false, currString = false;
    while (nextString != NULL) {
        k = 0;
        i = 0;
        currString = false;
        if (strlen(list->string) == 0) {
            list = nextString;
            nextString = nextString->next;
            if (nextString == NULL) {
                return;
            }
        }
        for (i = 0; i < strlen(list->string) + 1; ++i) {
            //if newline set prevString flag to true
            if (list->string[strlen(list->string) - i] == 10 || list->string[strlen(list->string) - i] == 13) {
                prevString = true;
            }
        }

        i = 0;
        if (strlen(nextString->string) == 0) {
            nextString = nextString->next;
            if (nextString == NULL) {
                return;
            }
        }
        //if the beginning of the next string is whitespace, keep going until it's not
        while (nextString->string[i] == 32 || nextString->string[i] == 9
                || nextString->string[i] == 10 || nextString->string[i] == 13) {
            if (nextString->string[i] == 10 || nextString->string[i] == 13) {
                currString = true;
            }
            i++;
        }
        if (prevString && currString) {
            tempString = malloc(sizeof(char) * strlen(nextString->string) - i + 1);
            for (; i < strlen(nextString->string) + 1; i++ ) {
                tempString[k++] = nextString->string[i];
            }
            //nextString->string = realloc(nextString->string, sizeof(char) * strlen(tempString) + 3);
            strcpy(nextString->string, tempString);
            free(tempString);
            list->string = realloc(list->string, sizeof(char) * strlen(list->string) + 2);
            strcat(list->string, "\n\0");
        }
        //if nextString is zero length move nextString but not list
        //as to skip over the zero length string
        if (strlen(nextString->string) == 0) {
            nextString = nextString->next;
        } else {
            list = list->next;
            nextString = list->next;
            prevString = false;
        }
    }
}

static void pyFormatting (struct dataHeader *header) {
    char BUFF[MAX_BUFF];
    char pyPath[] = "./html-formatting.py ";
    char *command = calloc(1,sizeof(char) * strlen(pyPath) + strlen(header->name) + 2);
    strcat(command, pyPath);
    strcat(command, header->name);
    FILE *fifo;
    struct dataString *list = header->next;
    pid_t pid;
    pid = fork();
    if (pid == 0) {
        system(command);
    } else {
        fifo = fopen("q1", "w");
        fprintf(fifo, "%s\n", header->name);
        while (list != NULL) {
            fprintf(fifo, "%s", list->string);
            list = list->next;
        }
        fclose(fifo);

        mkfifo("q2", 0666);
        fifo = fopen("q2", "r");
        while (fscanf(fifo, "%s ", BUFF) == 1) {
            printf("%s\n", BUFF);
        }
        fclose(fifo);
        remove("q2");
    }
    free(command);
}
/*
 * Frees up all allocated memory in the list
 * Destroys the list passed so only use when done with that list
 * takes a pointer to the desired struct dataHeader
 */
int freeStructure (struct dataHeader * header) {
    struct dataString *temp = header->next, *curr;

    while (temp != NULL) {
        curr = temp->next;
        if (temp->string != NULL) {
            free(temp->string);
        }
        free(temp);
        temp = curr;
    }
    if (header->name != NULL) {
        free(header->name);
    }
    free(header);
    return SUCCESS;
}

/*
 * Writes the full list from *header to a file. The file name is specified by *fileName.
 * The data will be written as a binary file and the lengths of each individual string will also be written
 */
int writeStrings (char * fileName, struct dataHeader *header) {
    //check for valid arguments
    if (header == NULL) {
        return FAILURE;
    }
    if (header->name == NULL) {
        return FAILURE;
    }
    if (fileName == NULL) {
        return FAILURE;
    }

    FILE *fileToWrite = fopen(fileName, "w+");
    int stringLength;
    struct dataString *list = header->next;

    //Writing the header
    stringLength = strlen(header->name) + 1;
    fwrite(&stringLength, sizeof(int), 1, fileToWrite);
    fwrite(header->name, sizeof(char), stringLength, fileToWrite);
    fwrite(&header->length, sizeof(int), 1, fileToWrite);

    //Looping and writing the rest of the list
    while (list != NULL) {
        stringLength = strlen(list->string) + 1;
        fwrite(&stringLength, sizeof(int), 1, fileToWrite);
        fwrite(list->string, sizeof(char), stringLength, fileToWrite);
        list = list->next;
    }
    fclose(fileToWrite);
    return SUCCESS;
}

/* Reads a file specified by *fileName into a new dataHeader
 * Returns the new dataHeader when finished
 * If any errors are encountered, the function attempts to free any memory and returns NULL
 */
struct returnStruct *readStrings (char *fileName) {
    struct returnStruct *rtn = buildHeader();


    if (fileName == NULL) {
        rtn->value = FAILURE;
        freeStructure(rtn->header);
        rtn->header = NULL;
        return rtn;
    }

    FILE *fileToRead = fopen(fileName, "r");
    if (fileToRead == NULL) {
        rtn->value = FAILURE;
        freeStructure(rtn->header);
        rtn->header = NULL;
        return rtn;
    }
    int stringLength, tempLength;
    char *bufferString;

    //reads the length of the string to come, mallocs space
    //then reads string into new string
    fread(&stringLength, sizeof(int), 1, fileToRead);
    bufferString = malloc(sizeof(char) * stringLength);
    fread(bufferString, sizeof(char), stringLength, fileToRead);
    if (strlen(bufferString) + 1 != stringLength) {
        freeStructure(rtn->header);
        rtn->value = FAILURE;
        return rtn;
    }
    setName(rtn->header, bufferString);
    //The total length of the list
    fread(&tempLength, sizeof(int), 1, fileToRead);
    free(bufferString);

    while (fread(&stringLength, sizeof(int), 1, fileToRead) == 1) {
        bufferString = malloc(sizeof(char) * stringLength);

        //check that malloc worked
        if (bufferString == NULL) {
            rtn->value = FAILURE;
            freeStructure(rtn->header);
            fclose(fileToRead);
            rtn->header = NULL;
            return rtn;
        }

        fread(bufferString, sizeof(char), stringLength, fileToRead);

        if (strlen(bufferString) + 1 != stringLength) {

            freeStructure(rtn->header);
            rtn->value = FAILURE;
            return rtn;
        }
        addString(rtn->header, bufferString);
        free(bufferString);
    }
    rtn->header->length = tempLength;
    fclose(fileToRead);
    rtn->value = SUCCESS;
    return (rtn);
}
