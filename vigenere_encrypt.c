#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "crypt.h"

int main(int argc, char *argv[])
{
    char *plaintext;
    char *ciphertext;

    if (argc < 3) {
        fprintf(stderr, "Usage: %s filename key\n", argv[0]);
        return(1);
    }
    plaintext = read_letters(argv[1]);
    if (!plaintext) {
        fprintf(stderr, "file does not exist\n");
        return(1);
    }
    ciphertext = vigenere_encrypt(plaintext, argv[2]);
    teleprinter(ciphertext, strlen(ciphertext));
    free(plaintext);
    free(ciphertext);
    return 0;
}
