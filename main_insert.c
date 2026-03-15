#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "sorting.h"

int main(void) {
    srand(time(NULL));
    int sizes[] = {100, 200, 300, 400, 500};

    printf("┌──────────────────────────────────────────────────────────────────────────────────────────┐\n");
    printf("│         InsertSort: Теоретическое и фактическое значения M+C                            │\n");
    printf("├──────┬────────────┬────────────┬────────────┬──────────────┬──────────────┬──────────────┤\n");
    printf("│  N   │ Mt+Ct(dec) │ Mt+Ct(rnd) │ Mt+Ct(inc) │  Mf+Cf(dec)  │  Mf+Cf(rnd)  │  Mf+Cf(inc)  │\n");
    printf("├──────┼────────────┼────────────┼────────────┼──────────────┼──────────────┼──────────────┤\n");

    for (int s = 0; s < 5; s++) {
        int n = sizes[s];
        int Tdec = n * (n - 1) + 2 * (n - 1);
        int Trnd = n * (n - 1) / 2 + (n - 1);
        int Tinc = 3 * (n - 1);
        int Mdec, Cdec, Mrand, Crand, Minc, Cinc;
        RunSort(n, InsertSort, &Mdec, &Cdec, &Mrand, &Crand, &Minc, &Cinc);
        printf("│ %4d │ %10d │ %10d │ %10d │ %12d │ %12d │ %12d │\n",
            n, Tdec, Trnd, Tinc,
            Mdec + Cdec, Mrand + Crand, Minc + Cinc);
    }
    printf("└──────┴────────────┴────────────┴────────────┴──────────────┴──────────────┴──────────────┘\n\n");

    printf("┌───────────────────────────────────────────────────────────────────────────────────────────────────────┐\n");
    printf("│               InsertSort: Фактические значения M и C по отдельности                              │\n");
    printf("├──────┬─────────┬─────────┬──────────┬─────────┬─────────┬──────────┬─────────┬─────────┬──────────┤\n");
    printf("│  N   │  M(dec) │  C(dec) │ M+C(dec) │  M(rnd) │  C(rnd) │ M+C(rnd) │  M(inc) │  C(inc) │ M+C(inc) │\n");
    printf("├──────┼─────────┼─────────┼──────────┼─────────┼─────────┼──────────┼─────────┼─────────┼──────────┤\n");

    for (int s = 0; s < 5; s++) {
        int n = sizes[s];
        int Mdec, Cdec, Mrand, Crand, Minc, Cinc;
        RunSort(n, InsertSort, &Mdec, &Cdec, &Mrand, &Crand, &Minc, &Cinc);
        printf("│ %4d │ %7d │ %7d │ %8d │ %7d │ %7d │ %8d │ %7d │ %7d │ %8d │\n",
            n, Mdec, Cdec, Mdec + Cdec,
            Mrand, Crand, Mrand + Crand,
            Minc, Cinc, Minc + Cinc);
    }
    printf("└──────┴─────────┴─────────┴──────────┴─────────┴─────────┴──────────┴─────────┴─────────┴──────────┘\n");
    return 0;
}
