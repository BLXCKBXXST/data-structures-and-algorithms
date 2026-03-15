#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "sorting.h"

int main(void) {
    srand(time(NULL));
    int sizes[] = {100, 200, 300, 400, 500};

    printf("┌──────────────────────────────────────────────────────────────────────────────────────────┐\n");
    printf("│         ShakerSort: Теоретическое и фактическое значения M+C                            │\n");
    printf("├──────┬────────────┬────────────┬────────────┬──────────────┬──────────────┬──────────────┤\n");
    printf("│  N   │ Mt+Ct(dec) │ Mt+Ct(rnd) │ Mt+Ct(inc) │  Mf+Cf(dec)  │  Mf+Cf(rnd)  │  Mf+Cf(inc)  │\n");
    printf("├──────┼────────────┼────────────┼────────────┼──────────────┼──────────────┼──────────────┤\n");

    for (int s = 0; s < 5; s++) {
        int n = sizes[s];
        int Ctdec = n * (n - 1) / 2;
        int Ctrand = n * (n - 1) / 2;
        int Ctinc = n - 1;
        int Mtdec = 3 * n * (n - 1) / 2;
        int Mtrnd = 3 * n * (n - 1) / 4;
        int Mtinc = 0;
        int Mdec, Cdec, Mrand, Crand, Minc, Cinc;
        RunSort(n, ShakerSort, &Mdec, &Cdec, &Mrand, &Crand, &Minc, &Cinc);
        printf("│ %4d │ %10d │ %10d │ %10d │ %12d │ %12d │ %12d │\n",
            n, Mtdec + Ctdec, Mtrnd + Ctrand, Mtinc + Ctinc,
            Mdec + Cdec, Mrand + Crand, Minc + Cinc);
    }
    printf("└──────┴────────────┴────────────┴────────────┴──────────────┴──────────────┴──────────────┘\n\n");

    printf("┌───────────────────────────────────────────────────────────────────────────────────────────────────────┐\n");
    printf("│               ShakerSort: Фактические значения M и C по отдельности                              │\n");
    printf("├──────┬─────────┬─────────┬──────────┬─────────┬─────────┬──────────┬─────────┬─────────┬──────────┤\n");
    printf("│  N   │  M(dec) │  C(dec) │ M+C(dec) │  M(rnd) │  C(rnd) │ M+C(rnd) │  M(inc) │  C(inc) │ M+C(inc) │\n");
    printf("├──────┼─────────┼─────────┼──────────┼─────────┼─────────┼──────────┼─────────┼─────────┼──────────┤\n");

    for (int s = 0; s < 5; s++) {
        int n = sizes[s];
        int Mdec, Cdec, Mrand, Crand, Minc, Cinc;
        RunSort(n, ShakerSort, &Mdec, &Cdec, &Mrand, &Crand, &Minc, &Cinc);
        printf("│ %4d │ %7d │ %7d │ %8d │ %7d │ %7d │ %8d │ %7d │ %7d │ %8d │\n",
            n, Mdec, Cdec, Mdec + Cdec,
            Mrand, Crand, Mrand + Crand,
            Minc, Cinc, Minc + Cinc);
    }
    printf("└──────┴─────────┴─────────┴──────────┴─────────┴─────────┴──────────┴─────────┴─────────┴──────────┘\n\n");

    printf("┌──────────────────────────────────────────────────────────────────────────────────────────────┐\n");
    printf("│          BubbleSort vs ShakerSort: Сравнение фактических M+C                                  │\n");
    printf("├──────┬──────────────┬──────────────┬──────────────┬──────────────┬──────────────┬──────────────┤\n");
    printf("│  N   │ Bubble(dec)  │ Bubble(rnd)  │ Bubble(inc)  │ Shaker(dec)  │ Shaker(rnd)  │ Shaker(inc)  │\n");
    printf("├──────┼──────────────┼──────────────┼──────────────┼──────────────┼──────────────┼──────────────┤\n");

    for (int s = 0; s < 5; s++) {
        int n = sizes[s];
        int Mdec, Cdec, Mrand, Crand, Minc, Cinc;
        RunSort(n, BubbleSort, &Mdec, &Cdec, &Mrand, &Crand, &Minc, &Cinc);
        int bdec = Mdec + Cdec, brnd = Mrand + Crand, binc = Minc + Cinc;
        RunSort(n, ShakerSort, &Mdec, &Cdec, &Mrand, &Crand, &Minc, &Cinc);
        int sdec = Mdec + Cdec, srnd = Mrand + Crand, sinc = Minc + Cinc;
        printf("│ %4d │ %12d │ %12d │ %12d │ %12d │ %12d │ %12d │\n",
            n, bdec, brnd, binc, sdec, srnd, sinc);
    }
    printf("└──────┴──────────────┴──────────────┴──────────────┴──────────────┴──────────────┴──────────────┘\n");
    return 0;
}
