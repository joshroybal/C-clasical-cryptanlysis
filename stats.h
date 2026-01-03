#ifndef STATS_H
#define STATS_H

#define NC 26

void dump(const int arr[], int n);
void count_letters(int dst[], int m, const char *src, int n);
void compute_frequencies(float dst[], const int src[], int n);
void count_digraphs(int dst[][NC], const char *src, int n);
void get_difrq(float dst[][NC], const char *src, int n);
float ioc(const char *buf, int n);
float corr(const float x[], const float y[], int n);
float engcorr(const char *buf, int n);
float engdc(const char *buf, int n);
int period(const char *buf, int n);
void crack_key(int *key, int p, const char *buf, int n);
void ssidx(int idx[], const int src[], int n);
void swap(int *m, int *n);
void foothill(int arr[], int m, const char *buf, int n);
void test_swaps(const char *buf, int n);

#endif
