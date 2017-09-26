#include <stdio.h>
#include <stdlib.h>

/* Error status */
#define INCORRECT_USAGE -1
#define UNABLE_TO_OPEN_FILE -2

/* Argument indexes */
#define FILEPATH_IDX 1
#define PDBR_IDX 2
#define VA_IDX 3

/* Global constants */
#define NUM_OF_PAGES 128
#define NUM_OF_ENTRIES 32

char ** parseLine(char * line) {
    // use example from internet but without the extra stuff
    // make sure to check for space skips
}

int main(int argc, char * argv[]) {
    // Check number of arguments
    if (argc != 4) {
        printf("ERROR: Incorrect usage.\nUsage: ./p4 <filepath> <pdbr> <virtual adress>");
        return INCORRECT_USAGE;
    }

    // open file
    FILE * fp;
    fp = fopen(argv[FILEPATH_IDX], "r");
    if (fp == NULL) {
        printf("ERROR: Unable to open file %s\n", argv[FILEPATH_IDX]);
        return UNABLE_TO_OPEN_FILE;
    }

    char ** paTable;
    paTable = malloc(sizeof(char *) * NUM_OF_PAGES * NUM_OF_ENTRIES);

    // while getc to get file in each line
    // get an array of tokens...
    // skip the next two tokens

    // strdup to paTable

    // assert that the number of index equals

    // get the base register

    // translate the virtual address to components
    // turn virtual address into an integer

    // use masks and right shift

    // validBit mask 128
    // pfn mask 127

}