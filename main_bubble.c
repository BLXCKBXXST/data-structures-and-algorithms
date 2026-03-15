#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "sorting.h"

int main(void) {
    srand(time(NULL));
    int sizes[] = {100, 200, 300, 400, 500};

    printf("┌──────────────────────────────────────────────────────────────────────────────────────────┐\n");
    printf("│                BubbleSort: Теоретическое и фактическое значения M+C                      │\n");
    printf("├──────┬────────────┬────────────┬────────────┬──────────────┬──────────────┬──────────────┤\n");
    printf("│  N   │ Mt+Ct(dec) │ Mt+Ct(rnd) │ Mt+Ct(inc) │  Mf+Cf(dec)  │  Mf+Cf(rnd)  │  Mf+Cf(inc)  │\n");
    printf("├──────┼────────────┼────────────┼────────────┼──────────────┼──────────────┼──────────────┤\n");

    for (int s = 0; s < 5; s++) {
        int n = sizes[s];
        int Ct    = n * (n - 1) / 2;
        int Mtdec = 3 * n * (n - 1) / 2;
        int Mtrnd = 3 * n * (n - 1) / 4;
        int Mtinc = 0;
        int Mdec, Cdec, Mrand, Crand, Minc, Cinc;
        RunSort(n, BubbleSort, &Mdec, &Cdec, &Mrand, &Crand, &Minc, &Cinc);
        printf("│ %4d │ %10d │ %10d │ %10d │ %12d │ %12d │ %12d │\n",
            n, Mtdec + Ct, Mtrnd + Ct, Mtinc + Ct,
            Mdec + Cdec, Mrand + Crand, Minc + Cinc);
    }
    printf("└──────┴────────────┴────────────┴────────────┴──────────────┴──────────────┴──────────────┘\n\n");

    printf("┌───────────────────────────────────────────────────────────────────────────────────────────────────────┐\n");
    printf("│                      BubbleSort: Фактические значения M и C по отдельности                        │\n");
    printf("├──────┬─────────┬─────────┬──────────┬─────────┬─────────┬──────────┬─────────┬─────────┬──────────┤\n");
    printf("│  N   │  M(dec) │  C(dec) │ M+C(dec) │  M(rnd) │  C(rnd) │ M+C(rnd) │  M(inc) │  C(inc) │ M+C(inc) │\n");
    printf("├──────┼─────────┼─────────┼──────────┼─────────┼─────────┼──────────┼─────────┼─────────┼──────────┤\n");

    for (int s = 0; s < 5; s++) {
        int n = sizes[s];
        int Mdec, Cdec, Mrand, Crand, Minc, Cinc;
        RunSort(n, BubbleSort, &Mdec, &Cdec, &Mrand, &Crand, &Minc, &Cinc);
        printf("│ %4d │ %7d │ %7d │ %8d │ %7d │ %7d │ %8d │ %7d │ %7d │ %8d │\n",
            n, Mdec, Cdec, Mdec + Cdec,
            Mrand, Crand, Mrand + Crand,
            Minc, Cinc, Minc + Cinc);
    }
    printf("└──────┴─────────┴─────────┴──────────┴─────────┴─────────┴──────────┴─────────┴─────────┴──────────┘\n");
    return 0;
}
