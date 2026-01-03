#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "crypt.h"
#include "stats.h"

#define SIZ 125

int main(int argc, char *argv[])
{
    int i, n, p;
    char *plaintext = (char *) NULL;
    char *ciphertext = (char *) NULL;
    char *key = (char *) NULL;
    int *shifts = (int *) NULL;

    if (argc < 2) {
        fprintf(stderr, "Usage: %s filename\n", argv[0]);
        return(1);
    }
    
    ciphertext = read_letters(argv[1]);
    if (!ciphertext) {
        fprintf(stderr, "file does not exist\n");
        return(1);
    }
    n = strlen(ciphertext);
    p = period(ciphertext, n);
    shifts = malloc(p * sizeof(int));
    memchk(shifts);
    crack_key(shifts, p, ciphertext, n);
    key = malloc((p + 1) * sizeof(char));
    memchk(key);
    memset(key, '\0', p + 1);
    for (i = 0; i < p; ++i)
        key[i] = 'a' + (NC - shifts[i]) % NC;
    plaintext = vigenere_decrypt(ciphertext, key);
    puts("ciphertext");
    teleprinter(ciphertext, SIZ);
    printf("ioc = %.3f\n", ioc(ciphertext, n));
    printf("corr = %.3f\n", engcorr(ciphertext, n));
    printf("period = %d\n", p);
    printf("key: %s\n", key);
    puts("plaintext");
    teleprinter(plaintext, SIZ);
    printf("ioc = %.3f\n", ioc(plaintext, n));
    printf("corr = %.3f\n", engcorr(plaintext, n));
    free(ciphertext);
    free(shifts);
    free(key);
    free(plaintext);
    return(0);
}
