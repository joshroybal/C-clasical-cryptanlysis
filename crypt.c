#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <limits.h>
#include "crypt.h"

/* const char buffers are always alphabetic characters processed by 
 * *read_letters function so do not waste time with ex post facto 
 * sanitization  */

void memchk(void *p)
{
   if (!p) {
      fprintf(stderr, "memory allocation error\n");
      exit(1);
   }
}

int ltridx(char c)
{
   return toupper(c) - 'A';
}

char *resize(char *buf, int *siz)
{
    char *newbuf;

    if (*siz > INT_MAX / 2 - 1) {
        fprintf(stderr, "32 bit address space insufficient.\n");
        free(buf);
        exit(1);
    }
    *siz *= 2;
    newbuf = realloc(buf, *siz * sizeof(char));
    memchk(newbuf);
    return newbuf;
}

char *read_letters(char *filename)
{
    int c, i = 0, n = 0, siz = BUFSIZ;
    char pad[] = "AAEO";
    char *buf;
    FILE *fp;
    
    if ((fp = fopen(filename, "r")) == NULL)
        return(NULL);
    
    buf = malloc(BUFSIZ * sizeof(char));
    memchk(buf);
    
    while ((c = fgetc(fp)) != EOF)
        if (isalpha(c)) {
            buf[n++] = toupper(c);
            if (n >= siz)
                buf = resize(buf, &siz);
        }
    fclose(fp);
    
    while (n % 5 != 0) {
        buf[n++] = pad[i++];
        if (n >= siz)
            buf = resize(buf, &siz);
    }
    buf[n] = '\0';
    
    return buf;
}

void teleprinter(const char *buf, int n)
{
    int i, j, k;
    char line[30];

    for (i = 0; i < n; i += 25) {
        k = 0;
        for (j = i; j < n && j < i + 25; j++) {
            line[k++] = toupper(buf[j]);
            if (j < n - 1 && k < 29 && j % 5 == 4)
                line[k++] = ' ';
        }
        line[k] = '\0';
        puts(line);
    }
}
    
char *coltran(const char *buf, int k)
{
    int i, j, m = 0, n = strlen(buf);
    char *res = malloc((n + 1) * sizeof(char));

    memchk(res);
    for (i = 0; i < k; ++i)
        for (j = i; j < n; j += k)
            res[m++] = buf[j];
    res[m] = '\0';
    return res;
}

int *trankey(const char *s)
{
    char c;
    int i, j = 0, n = strlen(s);
    int *key = malloc(n * sizeof(int));
 
    memchk(key);
    for (c = 'A'; c <= 'Z' && j < n; ++c)
        for (i = 0; i < n && j < n; ++i)
            if (toupper(s[i]) == c)
                key[j++] = i;
    return key;
}

char *keytran(const char *buf, const char *s)
{
    int i, j, k = strlen(s), m = 0, n = strlen(buf);
    int *key = trankey(s);
    char *res = malloc((n + 1) * sizeof(char));

    memchk(res);
    for (i = 0; i < k; ++i)
        for (j = key[i]; j < n; j += k)
            res[m++] = buf[j];
    res[m] = '\0';
    free(key);
    return res;
}

int rndint(int a, int b)
{
    float r = (float) rand() / ((float) RAND_MAX + 1.0F);
    
    return a + (b - a + 1) * r;
}

void exchange(char *a, char *b)
{
    char t = *a;
    
    *a = *b;
    *b = t;
}

void shuffle(char *a, int n)
{
    int i;

    for (i = n - 1; i > 0; i--)
        exchange(&a[i], &a[rndint(0, i)]);
}

char *subkey()
{
    char *key = malloc(NC * sizeof(char));

    memchk(key);
    memcpy(key, "ABCDEFGHIJKLMNOPQRSTUVWXYZ", NC);
    shuffle(key, NC);
    return key;
}

char *simplesub(const char *buf, const char *key)
{
    int i, n = strlen(buf);
    char *result = malloc((n + 1) * sizeof(char)); 

    memchk(result);
    for (i = 0; i < n; i++)
        result[i] = key[buf[i] - 'A'];
    result[n] = '\0';
    return result;
}

char shift(char c, int k)
{
    return 'A' + (ltridx(c) + k) % NC;
}

char *shift_encrypt(const char *plaintext, int k)
{
    int i, n = strlen(plaintext);
    char *ciphertext = malloc((n + 1) * sizeof(char));

    memchk(ciphertext);
    for (i = 0; i < n; ++i)
        ciphertext[i] = shift(plaintext[i], k);
    ciphertext[n] = '\0';
    return ciphertext;
}

char affine(char c, int a, int b)
{
    return 'A' + (a*ltridx(c) + b) % NC;
}

char *affine_encrypt(const char *plaintext, int a, int b)
{
    int i, n = strlen(plaintext);
    char *ciphertext = malloc((n + 1) * sizeof(char));

    memchk(ciphertext);
    for (i = 0; i < n; ++i)
        ciphertext[i] = affine(plaintext[i], a, b);
    ciphertext[n] = '\0';
    return ciphertext;
}

int *vigenere_key(const char *s)
{
    int i, n = strlen(s);
    int *key = malloc(n * sizeof(int));

    memchk(key);
    for (i = 0; i < n; ++i)
        key[i] = ltridx(s[i]);
    return key;
}

char *vigenere_encrypt(const char *plaintext, const char *str)
{
    int i, k = strlen(str), n = strlen(plaintext);
    int *key = vigenere_key(str);
    char *ciphertext = malloc((n + 1) * sizeof(char));

    memchk(ciphertext);
    for (i = 0; i < n; ++i)
        ciphertext[i] = shift(plaintext[i], key[i % k]);
    ciphertext[n] = '\0';
    free(key);
    return ciphertext;
}

char *vigenere_decrypt(const char *ciphertext, const char *str)
{
    int i, k = strlen(str), n = strlen(ciphertext);
    int *key = vigenere_key(str);
    char *plaintext = malloc((n + 1) * sizeof(char));

    memchk(plaintext);
    for (i = 0; i < n; ++i)
        plaintext[i] = shift(ciphertext[i], NC - key[i % k]);
    plaintext[n] = '\0';
    free(key);
    return plaintext;
}

char *autokey_encrypt(const char *plaintext, const char *str)
{
    int i, k = strlen(str), n = strlen(plaintext);
    int *key = vigenere_key(str);
    char *ciphertext = malloc((n + 1) * sizeof(char));

    memchk(ciphertext);
    for (i = 0; i < n; ++i) {
        ciphertext[i] = shift(plaintext[i], key[i % k]);
        key[i % k] = ltridx(plaintext[i]);
    }
    ciphertext[n] = '\0';
    free(key);
    return ciphertext;
}

char *autokey_decrypt(const char *ciphertext, const char *str)
{
    int i, k = strlen(str), n = strlen(ciphertext);
    int *key = vigenere_key(str);
    char *plaintext = malloc((n + 1) * sizeof(char));

    memchk(plaintext);
    for (i = 0; i < n; ++i) {
        plaintext[i] = shift(ciphertext[i], (NC - key[i % k]) % NC);
        key[i % k] = ltridx(plaintext[i]);
    }
    plaintext[n] = '\0';
    free(key);
    return plaintext;
}

int *cipherdisk_key(const char *s)
{
    int i, n = strlen(s);
    int *key = malloc(n * sizeof(int));

    memchk(key);
    for (i = 0; i < n; ++i)
        key[i] = NC - 1 - ltridx(s[i]);
    return key;
}

int *progressive_key(const char *s)
{
    int i, n = strlen(s);
    int *key = malloc(NC * n * sizeof(int));

    memchk(key);
    for (i = 0; i < NC * n; ++i)
        key[i] = NC - 1 - (ltridx(s[i % n]) + i/n) % NC;
    return key;
}

char *cipherdisk(const char *plaintext, const char *str)
{
    int i, k, n = strlen(plaintext);
    int *key = progressive_key(str);
    char *ciphertext = malloc((n + 1) * sizeof(char));

    memchk(ciphertext);
    k = NC * strlen(str);
    for (i = 0; i < n; ++i)
        ciphertext[i] = affine(plaintext[i], NC - 1, key[i % k]);
    ciphertext[n] = '\0';
    free(key);
    return(ciphertext);
}

