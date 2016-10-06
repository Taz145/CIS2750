#include "listio.h"


/*
 * This function will return a pointer of type struct dataHeader
 * All variables will be set to NULL/0 initially
 * No arguments are taken
 * If an error occurs the functions NULL
 */
struct dataHeader *buildHeader() {
    struct dataHeader *temp;
    temp = malloc(sizeof(struct dataHeader));
    //Checks if malloc worked
    if (temp == NULL) {
        fprintf(stderr, "Error. Out of memory.");
        return NULL;
    }
    //set to default values
    temp->name = NULL;
    temp->length = 0;
    temp->next = NULL;
    return temp;
}
/*
 * This funtion sets the name variable in specified struct dataHeader
 * Exits early if the string or header passed is NULL
 * Takes a pointer to the struct dataHeader that is being used and the string
 */
void setName(struct dataHeader *header, char *name) {
    if (header == NULL) {
        fprintf(stderr, "Error. Header is NULL. Aborting set.\n");
        return;
    }
    if (name == NULL) {
        fprintf(stderr, "Error. String is NULL. Aboriting set.\n");
        return;
    }

    //free the header in case of old name
    free(header->name);
    header->name = malloc(sizeof(char) * strlen(name) + 1);
    //copy name to header and update total length
    strcpy(header->name, name);
}

/*
 * Function used to get the current value stored in header->name
 * Takes a pointer to the desired struct dataHeader
 * If an error occurs, the function returns NULL
 */
char *getName(struct dataHeader *header) {
    if (header == NULL) {
        fprintf(stderr, "Error, the value of header is NULL");
        return NULL;
    }
    if (header->name == NULL) {
        fprintf(stderr, "Error, the value of header->name is NULL");
        return NULL;
    }
    return header->name;
}

/*
 * Function to get to current total length of all strings in the list
 * Takes a pointer to the desired struct dataHeader
 * If an error occurs the function returns -1
 */
int getLength(struct dataHeader *header) {

    if (header == NULL) {
        fprintf(stderr, "Error, the value of header is NULL");
        return -1;
    }

    int temp;
    temp = header->length;
    return temp;
}

/*
 * Used to add a new string to the list
 * Creates a new struct dataString and links it to the end of the list
 * Exits early if the passed string is NULL
 * Takes a pointer to the desired struct dataHeader and the string to be added
 */
void addString(struct dataHeader *header, char *str) {
    //dataStrings as a placeholder to loop though the list and a new entry
    struct dataString *endOfList;
    struct dataString *new = malloc(sizeof(struct dataString));
    //checks if malloc worked
    if (new == NULL) {
        fprintf(stderr, "Memory Error\n");
        return;
    }
    //checks if the string is valid
    if (str == NULL) {
        fprintf(stderr, "Error, string to be added is NULL. Aborting add.\n");
        free(new);
        return;
    }
    new->next = NULL;
    new->string = malloc(sizeof(char) * strlen(str) + 1);
    //check if malloc worked
    if (new->string == NULL) {
        fprintf(stderr, "Memory error. Aborting add\n");
        free(new);
        return;
    }
    strcpy(new->string, str);

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
    header->length += strlen(new->string) + 1;
}
/*
 * Used to print out all the strings in the list
 * prints strings in the order they were added
 * takes a pointer to the desired struct dataHeader
 */
//change #1
void printStrings(struct dataHeader *header) {
    if (header == NULL) {
        fprintf(stderr, "Error. The value of header is NULL.\n");
        return;
    }
    if (header->name == NULL) {
        fprintf(stderr, "Error. The value of header->name is NULL.\n");
        return;
    }
    //dataString to loop through the list
    struct dataString *list = header->next;

    //change #2
    //printf("%s\n", header->name);

    //loops through list until it hits the end
    while (list != NULL) {
        printf("%s\n", list->string);
        list = list->next;
    }
    printf("---End of List---\n");
}
/*
 * Parses each string and removes or changes certain characters
 * Multiple consecutive spaces and tabs will be condensed into 1
 * a single linebreak will be replaced with '<BR>'
 * multiple linebreaks will be replaces with '<P>'
 * takes a pointer to the desired struct dataHeader
 */
void processStrings (struct dataHeader *header) {
    if (header == NULL) {
        printf("Error. The value of header is NULL.\n");
        return;
    }

    struct dataString *temp = header->next;
    int oldLength, newLength;
    //used to pass values of strings when calling removeNewLines()
    char *placeHolder;

    removeChar(header->name, 32);
    removeChar(header->name, 9);

    placeHolder = malloc(sizeof(char) * strlen(header->name) + 1);
    //check that malloc worked
    if (placeHolder == NULL) {
        fprintf(stderr, "Memory error\n");
        return;
    }
    strcpy(placeHolder, header->name);
    free(header->name);
    header->name = replaceNewlines(placeHolder);
    free(placeHolder);

    //same as above but for the rest of the list
    while (temp != NULL) {
        oldLength = strlen(temp->string) + 1;
        removeChar(temp->string, 32);
        removeChar(temp->string, 9);

        placeHolder = malloc(sizeof(char) * strlen(temp->string) + 1);
        //check that malloc worked
        if (placeHolder == NULL) {
            fprintf(stderr, "Memory error\n");
            return;
        }
        strcpy(placeHolder, temp->string);
        free(temp->string);
        temp->string = replaceNewlines(placeHolder);
        free(placeHolder);

        newLength = strlen(temp->string) + 1;
        //update length, '-=' in case replaceNewLines() adds more characters than were in oldLength
        header->length -= (oldLength - newLength);

        temp = temp->next;
    }
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
        fprintf(stderr, "Memory error\n");
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
static char *replaceNewlines (char *oldString) {
    int spaceNeeded = 0, length = strlen(oldString);
    bool currChar = false, nextChar = false;
    char *newString = malloc(sizeof(char) * 1);

    //check that malloc worked
    if (newString == NULL) {
        fprintf(stderr, "Memory error\n");
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
                fprintf(stderr, "Memory Error.\n");
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
                fprintf(stderr, "Memory Error.\n");
                free(newString);
                return NULL;
            }
            newString[spaceNeeded - 3] = '<';
            newString[spaceNeeded - 2] = 'P';
            newString[spaceNeeded - 1] = '>';

            //skip over all the other newlines and whitespace in the sequence
            while (oldString[i + 1] == 10 || oldString[i + 1] == 13 || oldString[i + 1] == 32 || oldString[i + 1] == 9) {
                i++;
            }
            //Non-newLine character
        } else {
            spaceNeeded++;
            if ( (newString = realloc(newString, sizeof(char) * spaceNeeded) ) == NULL)  {
                fprintf(stderr, "Memory Error.\n");
                free(newString);
                return NULL;
            }
            newString[spaceNeeded - 1] = oldString[i];
        }
    }
    //add space for null character
    if ( (newString = realloc(newString, sizeof(char) * spaceNeeded + 1) ) == NULL)  {
        fprintf(stderr, "Memory Error.\n");
        free(newString);
        return NULL;
    }
    newString[spaceNeeded] = '\0';

    return (newString);
}

/*
 * Frees up all allocated memory in the list
 * Destroys the list passed so only use when done with that list
 * takes a pointer to the desired struct dataHeader
 */
void freeStructure (struct dataHeader * header) {
    struct dataString *temp = header->next, *curr;

    while (temp != NULL) {
        curr = temp->next;
        free(temp->string);
        free(temp);
        temp = curr;
    }
    free(header->name);
    free(header);
}

/*
 * Writes the full list from *header to a file. The file name is specified by *fileName.
 * The data will be written as a binary file and the lengths of each individual string will also be written
 */
void writeStrings (char * fileName, struct dataHeader *header) {
    //check for valid arguments
    if (header == NULL) {
        fprintf(stderr, "Error. The value of header is NULL. Aborting write.\n");
        return;
    }
    if (header->name == NULL) {
        fprintf(stderr, "Error. The value of header->name is NULL. Aborting write.\n");
        return;
    }
    if (fileName == NULL) {
        fprintf(stderr, "Error. The value of fileName is NULL. Aborting write.\n");
        return;
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
}

/* Reads a file specified by *fileName into a new dataHeader
 * Returns the new dataHeader when finished
 * If any errors are encountered, the function attempts to free any memory and returns NULL
 */
struct dataHeader *readStrings (char *fileName) {
    if (fileName == NULL) {
        fprintf(stderr, "Error. The value if fileName is NULL. Aborting read.\n");
        return NULL;
    }
    FILE *fileToRead = fopen(fileName, "r");
    int stringLength, tempLength;
    char *bufferString;
    struct dataHeader *header = buildHeader();

    //reads the length of the string to come, mallocs space
    //then reads string into new string
    fread(&stringLength, sizeof(int), 1, fileToRead);
    bufferString = malloc(sizeof(char) * stringLength);
    fread(bufferString, sizeof(char), stringLength, fileToRead);
    setName(header, bufferString);
    //The total length of the list
    fread(&tempLength, sizeof(int), 1, fileToRead);
    free(bufferString);

    while (fread(&stringLength, sizeof(int), 1, fileToRead) == 1) {
        bufferString = malloc(sizeof(char) * stringLength);

        //check that malloc worked
        if (bufferString == NULL) {
            fprintf(stderr, "Memory error\n");
            freeStructure(header);
            fclose(fileToRead);
            return NULL;
        }

        fread(bufferString, sizeof(char), stringLength, fileToRead);
        addString(header, bufferString);
        free(bufferString);
    }
    header->length = tempLength;
    fclose(fileToRead);
    return (header);
}
