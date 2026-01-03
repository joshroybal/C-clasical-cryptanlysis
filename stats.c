#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "stats.h"
#include "crypt.h"

/* english monographic frequencies */
static const float EMF[NC] = {0.0781,0.0128,0.0293,0.0411,0.1305,0.0288,
    0.0139,0.0565,0.0677,0.0023,0.0042,0.0360,0.0262,0.0728,0.0821,0.0215,
    0.0014,0.0664,0.0646,0.0902,0.0277,0.0100,0.0149,0.0030,0.0151,0.0009};

/* english digraphic frequencies */
static const float EDF[NC][NC] = {
{0.0001,0.0032,0.0039,0.0015,0.0,0.001,0.0018,0.0,0.0016,0.0,0.001,0.0077,0.0018,0.0172,0.0002,0.0031,0.0001,0.0101,0.0067,0.0124,0.0012,0.0024,0.0007,0.0,0.0027,0.0001},
{0.0008,0.0,0.0,0.0,0.0058,0.0,0.0,0.0,0.0006,0.0002,0.0,0.0021,0.0001,0.0,0.0011,0.0,0.0,0.0006,0.0005,0.0,0.0025,0.0,0.0,0.0,0.0019,0.0},
{0.0044,0.0,0.0012,0.0,0.0055,0.0001,0.0,0.0046,0.0015,0.0,0.0008,0.0016,0.0,0.0,0.0059,0.0001,0.0,0.0007,0.0001,0.0038,0.0016,0.0,0.0001,0.0,0.0,0.0},
{0.0045,0.0018,0.0004,0.001,0.0039,0.0012,0.0002,0.0003,0.0057,0.0001,0.0,0.0007,0.0009,0.0005,0.0037,0.0007,0.0001,0.001,0.0032,0.0039,0.0008,0.0004,0.0009,0.0,0.0006,0.0},
{0.0131,0.0011,0.0064,0.0107,0.0039,0.0023,0.002,0.0015,0.004,0.0001,0.0002,0.0046,0.0043,0.012,0.0046,0.0032,0.0014,0.0154,0.0145,0.008,0.0007,0.0016,0.0041,0.0017,0.0017,0.0},
{0.0021,0.0002,0.0009,0.0001,0.0025,0.0014,0.0001,0.0006,0.0021,0.0001,0.0,0.001,0.0003,0.0002,0.0038,0.0003,0.0,0.0004,0.0008,0.0042,0.0011,0.0001,0.0004,0.0,0.0001,0.0},
{0.0011,0.0002,0.0001,0.0001,0.0032,0.0003,0.0001,0.0016,0.001,0.0,0.0,0.0004,0.0001,0.0003,0.0023,0.0001,0.0,0.0021,0.0007,0.0013,0.0008,0.0,0.0002,0.0,0.0001,0.0},
{0.0084,0.0001,0.0002,0.0001,0.0251,0.0002,0.0,0.0005,0.0072,0.0,0.0,0.0003,0.0001,0.0002,0.0046,0.0001,0.0,0.0008,0.0003,0.0022,0.0002,0.0,0.0007,0.0,0.0001,0.0},
{0.0018,0.0007,0.0055,0.0016,0.0037,0.0027,0.001,0.0,0.0,0.0,0.0008,0.0039,0.0032,0.0169,0.0063,0.0003,0.0,0.0021,0.0106,0.0088,0.0,0.0014,0.0001,0.0001,0.0,0.0004},
{0.0,0.0,0.0,0.0,0.0002,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0004,0.0,0.0,0.0,0.0,0.0,0.0004,0.0,0.0,0.0,0.0,0.0},
{0.0,0.0,0.0,0.0,0.0028,0.0,0.0,0.0,0.0008,0.0,0.0,0.0,0.0,0.0003,0.0003,0.0,0.0,0.0,0.0002,0.0001,0.0,0.0,0.0003,0.0,0.0003,0.0},
{0.0034,0.0007,0.0008,0.0028,0.0072,0.0005,0.0001,0.0,0.0057,0.0001,0.0003,0.0055,0.0004,0.0001,0.0028,0.0002,0.0002,0.0002,0.0012,0.0019,0.0008,0.0002,0.0005,0.0,0.0047,0.0},
{0.0056,0.0009,0.0001,0.0002,0.0048,0.0,0.0,0.0001,0.0026,0.0,0.0,0.0,0.0005,0.0003,0.0028,0.0016,0.0,0.0,0.0006,0.0006,0.0013,0.0,0.0002,0.0,0.0003,0.0},
{0.0054,0.0007,0.0031,0.0118,0.0064,0.0008,0.0075,0.0009,0.0037,0.0003,0.0003,0.001,0.0007,0.0009,0.0065,0.0007,0.0,0.0005,0.0051,0.011,0.0012,0.0004,0.0015,0.0001,0.0014,0.0},
{0.0009,0.0018,0.0018,0.0016,0.0003,0.0094,0.0003,0.0003,0.0013,0.0,0.0005,0.0017,0.0044,0.0145,0.0023,0.0029,0.0,0.0113,0.0037,0.0053,0.0096,0.0013,0.0036,0.0,0.0004,0.0002},
{0.0021,0.0001,0.0,0.0,0.004,0.0,0.0,0.0007,0.0008,0.0,0.0,0.0029,0.0,0.0,0.0028,0.0026,0.0,0.0042,0.0003,0.0014,0.0007,0.0,0.0001,0.0,0.0002,0.0},
{0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.002,0.0,0.0,0.0,0.0,0.0},
{0.0057,0.0004,0.0014,0.0016,0.0148,0.0006,0.0006,0.0003,0.0077,0.0001,0.0011,0.0012,0.0015,0.0012,0.0054,0.0008,0.0,0.0018,0.0039,0.0063,0.0006,0.0005,0.001,0.0,0.0017,0.0},
{0.0075,0.0013,0.0021,0.0006,0.0084,0.0013,0.0006,0.003,0.0042,0.0,0.0002,0.0006,0.0014,0.0019,0.0071,0.0024,0.0002,0.0006,0.0041,0.0121,0.003,0.0002,0.0027,0.0,0.0004,0.0},
{0.0056,0.0014,0.0006,0.0009,0.0094,0.0005,0.0001,0.0315,0.0128,0.0,0.0,0.0012,0.0014,0.0008,0.0111,0.0008,0.0,0.003,0.0032,0.0053,0.0022,0.0004,0.0016,0.0,0.0021,0.0},
{0.0018,0.0005,0.0017,0.0011,0.0011,0.0001,0.0012,0.0002,0.0005,0.0,0.0,0.0028,0.0009,0.0033,0.0002,0.0017,0.0,0.0049,0.0042,0.0045,0.0,0.0,0.0,0.0001,0.0001,0.0001},
{0.0015,0.0,0.0,0.0,0.0053,0.0,0.0,0.0,0.0019,0.0,0.0,0.0,0.0,0.0,0.0006,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0},
{0.0032,0.0,0.0003,0.0004,0.003,0.0001,0.0,0.0048,0.0037,0.0,0.0,0.0004,0.0001,0.001,0.0017,0.0002,0.0,0.0001,0.0003,0.0006,0.0001,0.0001,0.0002,0.0,0.0,0.0},
{0.0003,0.0,0.0005,0.0,0.0001,0.0,0.0,0.0,0.0004,0.0,0.0,0.0,0.0,0.0,0.0001,0.0004,0.0,0.0,0.0,0.0001,0.0001,0.0,0.0,0.0,0.0,0.0},
{0.0011,0.0011,0.001,0.0004,0.0012,0.0003,0.0005,0.0005,0.0018,0.0,0.0,0.0006,0.0004,0.0003,0.0028,0.0007,0.0,0.0005,0.0017,0.0021,0.0001,0.0003,0.0014,0.0,0.0,0.0},
{0.0,0.0,0.0,0.0,0.0005,0.0,0.0,0.0,0.0002,0.0,0.0,0.0001,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0001}
};

void dump(const int a[], int n)
{
    int i;

    for (i = 0; i < n; ++i)
        /* printf(" %d", a[i]); */
        putchar('A' + a[i]);
    putchar('\n');
}

/* buf is always and only uppercase letters in ASCII range 65-90, 
 * therefore do not waste time on sanitization */
void count_letters(int counts[], int m, const char *buf, int n)
{
    int i;

    memset(counts, 0, m * sizeof(int));
    for (i = 0; i < n; ++i)
        ++counts[buf[i] - 'A'];
} 

void compute_frequencies(float frq[], const int cnt[], int n)
{
    int i;
    float acc = 0.0F;
    for (i = 0; i < n; ++i)
        acc += cnt[i];
    for (i = 0; i < n; ++i)
        frq[i] = cnt[i] / acc;
}

void count_digraphs(int cnt[][NC], const char *buf, int n)
{
    int i, j, k;

    memset(cnt, 0, NC * NC * sizeof(int));
    for (k = 0; k < n - 1; ++k) {
        i = ltridx(buf[k]);
        j = ltridx(buf[k + 1]);
        ++cnt[i][j];
    }
}

void get_difrq(float frq[][NC], const char *buf, int n)
{
    int i, j;
    int cnt[NC][NC];

    count_digraphs(cnt, buf, n);
    for (i = 0; i < NC; ++i)
        for (j = 0; j < NC; ++j)
            frq[i][j] = cnt[i][j] / (float) (n - 1);
}

float ioc(const char *buf, int n)
{
    int i;
    float acc = 0.0F;
    int counts[NC] = { 0 };

    count_letters(counts, NC, buf, n);
    for (i = 0; i < NC; ++i)
        acc += counts[i] * (float) (counts[i] - 1);
    return acc / ((n * (float) (n - 1)) / (float) NC);
}

float corr(const float a[], const float b[], int n)
{
    int i;
    float acc = 0.0F;

    for (i = 0; i < n; ++i)
        acc += a[i] * b[i];
    return acc;
}

float engcorr(const char *buf, int n)
{
    int counts[NC];
    float frequencies[NC];

    count_letters(counts, NC, buf, n);
    compute_frequencies(frequencies, counts, NC);
    return corr(frequencies, EMF, NC);
}

float engdc(const char *buf, int n)
{
    int i, j;
    float corr = 0.0F;
    float frq[NC][NC];

    get_difrq(frq, buf, n);
    for (i = 0; i < NC; ++i)
        for (j = 0; j < NC; ++j)
            corr += frq[i][j] * EDF[i][j];
    return corr;
}

int period(const char *buf, int n)
{
    int p, i, j, k;
    float acc;
    float idx[NC] = { 0 };
    char *work = malloc((n + 1) * sizeof(char));

    memchk(work);
    for (p = 2; p < NC; ++p) {
        acc = 0.0F;
        for (i = 0; i < p; ++i) {
            k = 0;
            memset(work, '\0', n + 1);
            for (j = i; j < n; j += p)
                work[k++] = buf[j];
            acc += ioc(work, k);
        }
        idx[p] = acc / p;
        if (idx[p] >= 1.456 && idx[p] <= 1.976) {
            free(work);
            return p;
        }
    }
    p = 0;
    for (i = 2; i < NC; i++) {
        printf("p = %d, ioc = %f\n", i, idx[i]);
        if (fabs(idx[i] - 1.742) < fabs(idx[p] - 1.742))
            p = i;
    }
    putchar('\n');
    printf("p = %d, ioc = %f\n", p, idx[p]);
    free(work);
    return p;
}

/* ciphertext only attack on Vigenere cipher */
void crack_key(int *key, int p, const char *buf, int n)
{
    int i, j, k, siz = ceil(n / (float) p) + 1;
    float shifts[NC];
    char *ciphertext = malloc(siz * sizeof(char));
    char *plaintext;
    
    memchk(ciphertext);
    for (i = 0; i < p; ++i) {
        memset(ciphertext, '\0', siz);
        k = 0;
        for (j = i; j < n; j += p)
            ciphertext[k++] = buf[j];
        for (j = 0; j < NC; ++j) {
            plaintext = shift_encrypt(ciphertext, j);
            shifts[j] = engcorr(plaintext, strlen(plaintext));
            free(plaintext);
        }
        k = 0;
        for (j = 0; j < NC; ++j)
            if (shifts[j] > shifts[k])
                k = j;
        key[i] = k;
    }
    free(ciphertext);
}

void ssidx(int idx[], const int src[], int n)
{
    int i, j, loc;

    for (i = 0; i < n; ++i)
        idx[i] = i;
    for (i = 0; i < n - 1; ++i) {
        loc = i;
        for (j = i + 1; j < n; ++j)
            if (src[idx[j]] > src[idx[loc]])
                loc = j;
        if (loc != i)
            swap(&idx[i], &idx[loc]);
    }
}

void swap(int *m, int *n)
{
    int tmp = *m;

    *m = *n;
    *n = tmp;
}

void foothill(int key[], int m, const char *buf, int n)
{
    int i;
    /*                  ABCDEFGHIJKLMNOPQRSTUVWXYZ */
    const char ORD[] = "ETAOINSHRDLCUMWFGYPBVKJXQZ";
    int cnt[NC], idx[NC];
    
    count_letters(cnt, m, buf, n);
    ssidx(idx, cnt, m);
    dump(idx, m);
    for (i = 0; i < m; ++i)
        key[idx[i]] = ORD[i] - 'A';
    dump(key, m);
}

void test_swaps(const char *ciphertext, int n)
{
    int i, j, k, x, y;
    float best, curr, start = 0;
    int bestkey[NC], currkey[NC];
    const char ORD[] = "ETAOINSHRDLCUMWFGYPBVKJXQZ";
    char *plaintext = malloc((n + 1) * sizeof(char));

    memchk(plaintext);
    memset(plaintext, '\0', n + 1);
    curr = best = engcorr(ciphertext, n);
    foothill(currkey, NC, ciphertext, n);
    dump(currkey, NC);
    memcpy(bestkey, currkey, sizeof(currkey));
    while (best - start > 0.001) {
        start = best;
        for (i = 0; i < NC - 1; ++i) {
            x = ORD[i] - 'A';
            for (j = i + 1; j < NC; ++j) {
                y = ORD[j] - 'A';
                swap(&currkey[x], &currkey[y]);
                for (k = 0; k < n; ++k)
                    plaintext[k] = 'A' + currkey[ciphertext[k] - 'A'];
                curr = engcorr(plaintext, n);
                if (curr > best) {
                    best = curr;
                    memcpy(bestkey, currkey, sizeof(currkey));
                    printf("%f ", best);
                    dump(bestkey, NC);
                }
                swap(&currkey[y], &currkey[x]);
            }
            memcpy(currkey, bestkey, sizeof(bestkey));
        }
        /* memcpy(currkey, bestkey, sizeof(bestkey));  */
    }

    dump(bestkey, NC);
    for (i = 0; i < n; ++i)
        plaintext[i] = 'A' + bestkey[ciphertext[i] - 'A'];
    teleprinter(plaintext, 125);
    printf("%f %f\n", 676*engdc(ciphertext, n), 676*engdc(plaintext, n));
    free(plaintext);
}
