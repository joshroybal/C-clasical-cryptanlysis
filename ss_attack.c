#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "crypt.h"
#include "stats.h"

#define SIZ 125

int main(int argc, char *argv[])
{
    char *ciphertext = (char *) NULL;

    if (argc < 2) {
        fprintf(stderr, "Usage: %s filename\n", argv[0]);
        return(1);
    }
    ciphertext = read_letters(argv[1]);
    if (!ciphertext) {
        fprintf(stderr, "file does not exist\n");
        return(1);
    }
    test_swaps(ciphertext, strlen(ciphertext));
    free(ciphertext);
    return 0;
}
