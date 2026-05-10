#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "search.h"
#include "sorting.h"

int main(void) {
    srand(time(NULL));

    int sizes[] = {100, 200, 300, 400, 500, 600, 700, 800, 900, 1000};
    int ns = 10;

    double avg1[10], avg2[10];
    double avgAll1[10], avgAll2[10];

    for (int s = 0; s < ns; s++) {
        int n = sizes[s];
        int arr[n];
        FillInc(n, arr);
        long sum1 = 0, sum2 = 0;
        for (int key = 0; key < n; key++) {
            int C1, C2;
            BSearch1(n, arr, key, &C1);
            BSearch2(n, arr, key, &C2);
            sum1 += C1;
            sum2 += C2;
        }
        avg1[s] = (double)sum1 / n;
        avg2[s] = (double)sum2 / n;
    }

    for (int s = 0; s < ns; s++) {
        int n = sizes[s];
        int K = n / 10;
        int arr[n];
        for (int i = 0; i < n; i++) arr[i] = i / K;
        int unique = n / K;
        int found[n];
        long sum1 = 0, sum2 = 0;
        for (int key = 0; key < unique; key++) {
            int C1, C2;
            BSearchAll1(n, arr, key, found, &C1);
            BSearchAll2(n, arr, key, found, &C2);
            sum1 += C1;
            sum2 += C2;
        }
        avgAll1[s] = (double)sum1 / unique;
        avgAll2[s] = (double)sum2 / unique;
    }

    /* ===================================================================
     * Основная таблица ТЗ: Трудоемкость двоичного поиска элемента
     * Теория: BSearch1 C = 2·log2(n), BSearch2 C = log2(n)
     * =================================================================== */
    printf("                          Трудоемкость двоичного поиска элемента\n");
    printf("┌──────┬──────────────────┬──────────────────┬──────────────────┬──────────────────┐\n");
    printf("│  N   │ C теор I версия  │ Сф I версия      │ C теор II версия │ Сф II версия     │\n");
    printf("├──────┼──────────────────┼──────────────────┼──────────────────┼──────────────────┤\n");
    for (int s = 0; s < ns; s++) {
        int n = sizes[s];
        int Cth1 = 2 * (int)floor(log2((double)n));
        int Cth2 = (int)floor(log2((double)n));
        printf("│ %4d │ %16d │ %16.2f │ %16d │ %16.2f │\n",
            n, Cth1, avg1[s], Cth2, avg2[s]);
    }
    printf("└──────┴──────────────────┴──────────────────┴──────────────────┴──────────────────┘\n");

    /* ===================================================================
     * Основная таблица ТЗ (п.4*): Трудоемкость двоичного поиска всех элементов
     * =================================================================== */
    printf("\n                       Трудоемкость двоичного поиска всех элементов\n");
    printf("┌──────┬──────────────────────┬──────────────────────┐\n");
    printf("│  N   │  Сф All I версия     │  Сф All II версия    │\n");
    printf("├──────┼──────────────────────┼──────────────────────┤\n");
    for (int s = 0; s < ns; s++) {
        printf("│ %4d │ %20.2f │ %20.2f │\n",
            sizes[s], avgAll1[s], avgAll2[s]);
    }
    printf("└──────┴──────────────────────┴──────────────────────┘\n");

    /* ===================================================================
     * === Доп. === BSearch1/BSearch2: разбор C — теория vs факт
     * =================================================================== */
    printf("\n=== Доп. ===\n");
    printf("                          BSearch1 vs BSearch2: теория vs факт по C\n");
    printf("┌──────┬──────────────────┬──────────────────┬──────────────────┬──────────────────┐\n");
    printf("│  N   │C теор (BSearch1) │ Сф факт(BSearch1)│C теор (BSearch2) │ Сф факт(BSearch2)│\n");
    printf("├──────┼──────────────────┼──────────────────┼──────────────────┼──────────────────┤\n");
    for (int s = 0; s < ns; s++) {
        int n = sizes[s];
        int Cth1 = 2 * (int)floor(log2((double)n));
        int Cth2 = (int)floor(log2((double)n));
        printf("│ %4d │ %16d │ %16.2f │ %16d │ %16.2f │\n",
            n, Cth1, avg1[s], Cth2, avg2[s]);
    }
    printf("└──────┴──────────────────┴──────────────────┴──────────────────┴──────────────────┘\n");

    return 0;
}
