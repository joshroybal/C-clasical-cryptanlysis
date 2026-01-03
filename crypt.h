#ifndef CRYPT_H
#define CRYPT_H

#define NC 26
#define BUFSIZ 8192

void memchk(void *p);
int ltridx(char c);
char *resize(char *buf, int *siz);
char *read_letters(char *);
void teleprinter(const char *buf, int n);
char *coltran(const char *buf, int k);
int *trankey(const char *str);
char *keytran(const char *buf, const char *str);
int rndint(int lo, int hi);
void exchange(char *a, char *b);
void shuffle(char *a, int n);
char *subkey();
char *simplesub(const char *buf, const char *key);
char shift(char c, int k);
char *shift_encrypt(const char *buf, int k);
char *shift_decrypt(const char *buf, int k);
char affine(char c, int a, int b);
char *affine_encrypt(const char *buf, int a, int b);
char *affine_decrypt(const char *buf, int a, int b);
int *vigenere_key(const char *str);
char *vigenere_encrypt(const char *buf, const char *str);
char *vigenere_decrypt(const char *buf, const char *str);
char *autokey_encrypt(const char *buf, const char *str);
char *autokey_decrypt(const char *buf, const char *str);
int *cipherdisk_key(const char *str);
int *progessive_key(const char *str);
char *cipherdisk(const char *buf, const char *str);

#endif
