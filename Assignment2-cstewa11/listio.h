/* Header file for liblistio.a
 * Created by Charles Stewart on Monday, September 12, 2016
 * As part of Assignment 1 for CIS 2750 (F16)
 * Template and descriptions of functions provided by Prof. David Calvert
 * Last Edit on Friday, September 23, 2016
 *
 *
 * This library is to be used to store strings in the defined srtuctres
 * These strings can then be parsed to remove extra spaces and tabs
 * and translate single newlines to <BR> and multiple newlines to <P>
 * The sructures can then be written to and read from a binary file
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#define SUCCESS 1
#define FAILURE 0

struct dataString {
    char *string;
    struct dataString *next;
};

struct dataHeader {
    char *name;
    int length;
    struct dataString *next;
};

struct returnStruct {
    int value;
    struct dataHeader *header;
};

/*
 * This function will return a pointer of type struct dataHeader
 * All variables will be set to NULL/0 initially
 * No arguments are taken
 * If an error occurs the functions NULL
 */
struct returnStruct *buildHeader();

/*
 * This funtion sets the name variable in specified struct dataHeader
 * Exits early if the string passed is NULL
 * Takes a pointer to the struct dataHeader that is being used and the string
 */
int setName(struct dataHeader *header, char *name);

/*
 * Function used to get the current value stored in header->name
 * Takes a pointer to the desired struct dataHeader
 * If an error occurs the function returns NULL
 */
char *getName(struct dataHeader *header);

/*
 * Function to get to current total length of all strings in the list
 * Takes a pointer to the desired struct dataHeader
 * If an error occurs the function returns -1
 */
int getLength(struct dataHeader *header);

/*
 * Used to add a new string to the list
 * Creates a new struct dataString and links it to the end of the list
 * Exits early if the passed string is NULL
 * Takes a pointer to the desired struct dataHeader and the string to be added
 */
int addString(struct dataHeader *header, char *str);

/*
 * Used to print out all the strings in the list
 * prints strings in the order they were added
 * takes a pointer to the desired struct dataHeader
 */
int printString(struct dataHeader *header);

/*
 * Parses each string and removes or changes certain characters
 * Multiple consecutive spaces and tabs will be condensed into 1
 * a single linebreak will be replaced with '<BR>'
 * multiple linebreaks will be replaces with '<P>'
 * takes a pointer to the desired struct dataHeader
 */
int processStrings(struct dataHeader *header);



/*
 * Frees up all allocated memory in the list
 * Destroys the list passed so only use when done with that list
 * takes a pointer to the desired struct dataHeader
 */
int freeStructure(struct dataHeader * header);

/*
 * Writes the full list from *header to a file. The file name is specified by *fileName.
 * The data will be written as a binary file and the lengths of each individual string will also be written
 */
int writeStrings(char *filename, struct dataHeader *header);

/* Reads a file specified by *fileName into a new dataHeader
 * Returns a pointer to the new dataHeader when finished
 * If the returned dataHeader should be assigned to a non-initalized dataHeader
 * This function will call buildHeader() to allocate memory on the new header
 * If any errors are encountered, the function attempts to free any memory and returns NULL
 */
struct returnStruct *readStrings(char *filename);
