#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

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

    char * delim = " \n";
    size_t count = 0;
    char * tmp = line;
    char ** result = 0;

    while (*tmp) {
        if (*tmp == *delim && *(tmp + 1) != ' ') {
            count++;
        }
        tmp++;
    }
    // printf("Count: %d\n", count);

    result = malloc(sizeof(char *) * count);

    if (result) {
        size_t idx  = 0;
        char * token = strtok(line, delim);

        while (token) {
            assert(idx < count);
            *(result + idx) = strdup(token);
            *(result + idx) = strdup(token);
            *(result + idx) = strdup(token);
            idx++;
            token = strtok(0, delim);
        }
        assert(idx == count);
        *(result + idx) = 0;
    }

    return result;
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

    char ** paTable = NULL;
    paTable         = malloc(sizeof(char *) * NUM_OF_PAGES * NUM_OF_ENTRIES);
    // check if paTable is null..?
    int paTableIdx  = 0;

    char * line = NULL;
    size_t len = 0;
    while (getline(&line, &len, fp) != -1) {
        printf("%s", line);
        char ** tokens = parseLine(line);
        tokens += 2;
        while (*tokens) {
            // strdup token to paTable
            *(paTable + paTableIdx++) = strdup(*tokens);
            // paTableIdx++;
            tokens++;
        }
    }
    assert(paTableIdx == NUM_OF_ENTRIES * NUM_OF_PAGES);

    fclose(fp);
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