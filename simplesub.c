#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "crypt.h"

int main(int argc, char *argv[])
{
    char *plaintext;
    char *ciphertext;
    char *key;

    if (argc < 2) {
        fprintf(stderr, "Usage: %s filename\n", argv[0]);
        return(1);
    }
    plaintext = read_letters(argv[1]);
    if (!plaintext) {
        fprintf(stderr, "file does not exist\n");
        return(1);
    }
    key = subkey();
    ciphertext = simplesub(plaintext, key);
    teleprinter(ciphertext, strlen(ciphertext));
    free(plaintext);
    free(ciphertext);
    free(key);
    return 0;
}
