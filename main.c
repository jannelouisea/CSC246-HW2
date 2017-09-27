#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>

/* Error status */
#define INCORRECT_USAGE -1
#define UNABLE_TO_OPEN_FILE -2
#define INVALID_ENTRY -70
#define INVALID_PDE -80
#define INVALID_PTE -90

/* Argument indexes */
#define FILEPATH_IDX 1
#define PDBR_IDX 2
#define VA_IDX 3

/* Global constants */
#define NUM_OF_PAGES 128
#define NUM_OF_ENTRIES 32
#define VALID_BIT_MASK 128
#define PFN_MASK 127

int getPFN(char * entry) {
    int entryDec = (int) strtol(entry, NULL, 16);
    int validBit = entryDec & VALID_BIT_MASK;
    if (validBit) {
        return entryDec & PFN_MASK;
    } else {
        return INVALID_ENTRY;
    }
}

int getComponentsFromVA(int va, int mask, int shiftR) {
    return (va & mask) >> shiftR;
}

int convertHexToDec(char * va) {
    if (va[0] == '0' && va[1] == 'x') {
        return (int) strtol(va, NULL, 0);
    } else {
        return (int) strtol(va, NULL, 16);
    };
    // return (va[0] == '0' && va[1] == 'x') ? (int) strtol(va, NULL, 0) : (int) strtol(va, NULL, 16);
}

char ** parseLine(char * line) {
    // printf("%s", line);

    char * delim = " \n";
    int count = 0;
    char * tmp = line;
    char ** result = 0;

    while (*tmp) {
        if ((isspace(*tmp) && !isspace(*(tmp + 1))) || strcmp(tmp, delim) == 1) { // account for new line?
            count++;
        }
        tmp++;
    }
    // printf("Count: %d\n", count);

    result = malloc(sizeof(char *) * count);

    tmp = line;
    if (result) {
        size_t idx  = 0;
        char * token = strtok(tmp, delim);

        while (token) {
            assert(idx < count);
            *(result + idx) = malloc(strlen(token) + 1);
            strcpy(*(result + idx), token);
            // printf("%s-", *(result + idx));
            idx++;
            token = strtok(0, delim);
        }
        // printf("\n");
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

    int lineLen = 108;
    char line[108];
    while(fgets(line, sizeof(line), fp) != NULL) {
        char ** tokens = parseLine(line);
        tokens += 2;
        while (*tokens) {
            *(paTable + paTableIdx) = malloc(sizeof(char) * 3);
            // *(paTable + paTableIdx) = malloc(strlen(*tokens) + 1);
            strcpy(*(paTable + paTableIdx), *tokens);
            // printf("%s-", *(paTable + paTableIdx));
            paTableIdx++;
            tokens++;
        }
        // printf("\n");
    }
    assert(paTableIdx == NUM_OF_ENTRIES * NUM_OF_PAGES);

    // Printing out the table
    /*
    size_t i = 0;
    while (i < (NUM_OF_ENTRIES * NUM_OF_PAGES)) {
        if (i % NUM_OF_ENTRIES == 0) {
            printf("\n");
        }
        printf("%s ", *(paTable + i));
        i++;
    }
    */

    fclose(fp);

    /* Get the page directory base register */
    int pdbr = (int) strtol(argv[PDBR_IDX], NULL, 10);
    assert(pdbr < NUM_OF_PAGES);

    int vaDec = convertHexToDec(argv[VA_IDX]);
    printf("Virtual Address %04x\n", vaDec);
    int pdi = getComponentsFromVA(vaDec, 31744, 10);
    int pti = getComponentsFromVA(vaDec, 992, 5);
    int offset = getComponentsFromVA(vaDec, 31, 0);

    /* Get PDE */
    char ** tmp = paTable;
    char * pde = *(tmp + (pdbr * NUM_OF_ENTRIES) + pdi);
    int ptPFN = getPFN(pde);
    if (ptPFN == INVALID_ENTRY) {
        printf("    --> Fault (page directory entry not valid)\n");
        exit(INVALID_PDE);
    }
    printf("    --> pde index:0x%x [decimal %d] pde contents:0x%s (valid %d, pfn 0x%02x [decimal %d])\n",
           pdi, pdi, pde, 1, ptPFN, ptPFN);

    /* Get PTE */
    char * pte = *(tmp + (ptPFN * NUM_OF_ENTRIES) + pti);
    int pePFN = getPFN(pte);
    if (pePFN == INVALID_ENTRY) {
        printf("        --> Fault (page table entry not valid)\n");
        exit(INVALID_PTE);
    }
    printf("        --> pde index:0x%x [decimal %d] pde contents:0x%s (valid %d, pfn 0x%02x [decimal %d])\n",
           pti, pti, pte, 1, pePFN, pePFN);



}