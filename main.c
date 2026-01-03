#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "crypt.h"
#include "stats.h"

int main(int argc, char *argv[])
{
    int i, n, p;
    char *buf = (char *) NULL;
    char *plaintext = (char *) NULL;
    char *ciphertext = (char *) NULL;
    char *key = (char *) NULL;
    int *shifts = (int *) NULL;

    if (argc < 2) {
        fprintf(stderr, "Usage: %s filename\n", argv[0]);
        return(1);
    }

    buf = read_letters(argv[1]);
    if (buf == (char *) NULL) {
        fprintf(stderr, "file does not exist\n");
        return(1);
    }
    n = strlen(buf);

    puts("plaintext");
    teleprinter(buf);

    puts("columnar transposition cipher");
    ciphertext = coltran(buf, 6);
    teleprinter(ciphertext);
    free(ciphertext);

    puts("keyword transposition cipher");
    ciphertext = keytran(buf, "brubaker");
    teleprinter(ciphertext);
    free(ciphertext);

    puts("simple substitution cipher");
    key = subkey();
    ciphertext = simplesub(buf, key);
    teleprinter(ciphertext);
    free(key);
    free(ciphertext);

    puts("shift cipher");
    ciphertext = shift_encrypt(buf, 13);
    teleprinter(ciphertext);
    free(ciphertext);

    puts("affine cipher");
    ciphertext = affine_encrypt(buf, 25, 25);
    puts("ciphertext");
    teleprinter(ciphertext);
    plaintext = affine_encrypt(ciphertext, 25, 25);
    free(ciphertext);
    puts("plaintext");
    teleprinter(plaintext);
    free(plaintext);

    puts("Vigenere cipher");
    ciphertext = vigenere_encrypt(buf, "brubaker");
    puts("ciphertext");
    teleprinter(ciphertext);
    plaintext = vigenere_decrypt(ciphertext, "brubaker");
    free(ciphertext);
    puts("plaintext");
    teleprinter(plaintext);
    free(plaintext);

    puts("autokey cipher");
    ciphertext = autokey_encrypt(buf, "brubaker");
    puts("ciphertext");
    teleprinter(ciphertext);
    plaintext = autokey_decrypt(ciphertext, "brubaker");
    free(ciphertext);
    puts("plaintext");
    teleprinter(plaintext);
    free(plaintext);

    puts("cipherdisk");
    ciphertext = cipherdisk(buf, "brubaker");
    puts("ciphertext");
    teleprinter(ciphertext);
    plaintext = cipherdisk(ciphertext, "brubaker");
    puts("plaintext");
    teleprinter(plaintext);
    free(plaintext);
    free(ciphertext);

    ciphertext = vigenere_encrypt(buf, "brubaker");
    printf("ioc = %f\n", ioc(ciphertext, n));
    printf("corr = %f\n", engcorr(ciphertext, n));
    p = period(ciphertext, n);
    printf("period = %d\n", p);
    shifts = (int *) malloc(p * sizeof(int));
    memchk(shifts);
    crack_key(shifts, p, ciphertext, n);
    key = (char *) malloc((p + 1) * sizeof(char));
    memset(key, '\0', p + 1);
    for (i = 0; i < p; ++i)
        key[i] = 'a' + shifts[i];
    free(shifts);
    printf("key: %s\n", key);
    plaintext = vigenere_encrypt(ciphertext, key);
    free(ciphertext);
    free(key);
    teleprinter(plaintext);
    free(plaintext);
    free(buf);
    return(0);
}
