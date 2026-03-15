#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "sorting.h"

int main(void) {
    srand(time(NULL));
    int sizes[] = {10, 100};

    printf("┌──────────────────────────────────────────────────────────────────────────────────────────────────────────┐\n");
    printf("│                        SelectSort vs SelectSortImproved: Сравнение M+C                                 │\n");
    printf("├──────┬────────┬──────────┬─────────────────────────────────────────┬─────────────────────────────────────────┤\n");
    printf("│      │        │          │          SelectSort: Mf+Cf           │       SelectSortImproved: Mf+Cf      │\n");
    printf("│  N   │   Mt   │    Ct    ├────────────┬────────────┬────────────├────────────┬────────────┬────────────┤\n");
    printf("│      │        │          │ Mf+Cf(dec) │ Mf+Cf(rnd) │ Mf+Cf(inc) │ Mf+Cf(dec) │ Mf+Cf(rnd) │ Mf+Cf(inc) │\n");
    printf("├──────┼────────┼──────────┼────────────┼────────────┼────────────┼────────────┼────────────┼────────────┤\n");

    for (int s = 0; s < 2; s++) {
        int n = sizes[s];
        int A[n], B[n], C[n], arr[n];
        int Mt = 3 * (n - 1);
        int Ct = n * (n - 1) / 2;
        int M, Cv;

        FillInc(n, A); FillDec(n, B); FillRand(n, C);

        CopyArray(n, B, arr); SelectSort(n, arr, &M, &Cv);
        int Mdec_o = M, Cdec_o = Cv;
        CopyArray(n, C, arr); SelectSort(n, arr, &M, &Cv);
        int Mrnd_o = M, Crnd_o = Cv;
        CopyArray(n, A, arr); SelectSort(n, arr, &M, &Cv);
        int Minc_o = M, Cinc_o = Cv;

        CopyArray(n, B, arr); SelectSortImproved(n, arr, &M, &Cv);
        int Mdec_i = M, Cdec_i = Cv;
        CopyArray(n, C, arr); SelectSortImproved(n, arr, &M, &Cv);
        int Mrnd_i = M, Crnd_i = Cv;
        CopyArray(n, A, arr); SelectSortImproved(n, arr, &M, &Cv);
        int Minc_i = M, Cinc_i = Cv;

        printf("│ %4d │ %6d │ %8d │ %10d │ %10d │ %10d │ %10d │ %10d │ %10d │\n",
            n, Mt, Ct,
            Mdec_o + Cdec_o, Mrnd_o + Crnd_o, Minc_o + Cinc_o,
            Mdec_i + Cdec_i, Mrnd_i + Crnd_i, Minc_i + Cinc_i);
    }
    printf("└──────┴────────┴──────────┴────────────┴────────────┴────────────┴────────────┴────────────┴────────────┘\n");
    return 0;
}
