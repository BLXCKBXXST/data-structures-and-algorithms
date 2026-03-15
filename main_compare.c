#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "sorting.h"

int main(void) {
    srand(time(NULL));
    int sizes[] = {100, 200, 300, 400, 500};

    printf("┌──────────────────────────────────────────────────────────────────────┐\n");
    printf("│Трудоёмкость квадратичных методов сортировки (случайный массив, Мф+Сф)│\n");
    printf("├──────┬────────────┬────────────┬────────────┬────────────┤\n");
    printf("│  N   │ SelectSort │ BubbleSort │ ShakerSort │ InsertSort │\n");
    printf("├──────┼────────────┼────────────┼────────────┼────────────┤\n");

    for (int s = 0; s < 5; s++) {
        int n = sizes[s];
        int src[n], arr[n];
        int M, C;
        FillRand(n, src);
        CopyArray(n, src, arr); SelectSort(n, arr, &M, &C); int sel = M + C;
        CopyArray(n, src, arr); BubbleSort(n, arr, &M, &C); int bub = M + C;
        CopyArray(n, src, arr); ShakerSort(n, arr, &M, &C); int shk = M + C;
        CopyArray(n, src, arr); InsertSort(n, arr, &M, &C); int ins = M + C;
        printf("│ %4d │ %10d │ %10d │ %10d │ %10d │\n",
            n, sel, bub, shk, ins);
    }
    printf("└──────┴────────────┴────────────┴────────────┴────────────┘\n");
    return 0;
}
