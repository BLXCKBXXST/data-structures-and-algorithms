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

    /* Считаем данные для всех таблиц */
    double avg1[10], avg2[10];
    int max1[10], max2[10];
    double avgAll1[10], avgAll2[10];
    int maxAll1[10], maxAll2[10];

    for (int s = 0; s < ns; s++) {
        int n = sizes[s];
        int arr[n];
        FillInc(n, arr);
        long sum1 = 0, sum2 = 0;
        max1[s] = 0; max2[s] = 0;
        for (int key = 0; key < n; key++) {
            int C1, C2;
            BSearch1(n, arr, key, &C1);
            BSearch2(n, arr, key, &C2);
            sum1 += C1; if (C1 > max1[s]) max1[s] = C1;
            sum2 += C2; if (C2 > max2[s]) max2[s] = C2;
        }
        avg1[s] = (double)sum1 / n;
        avg2[s] = (double)sum2 / n;
    }

    /* BSearchAll данные */
    for (int s = 0; s < ns; s++) {
        int n = sizes[s];
        int K = n / 10;
        int arr[n];
        for (int i = 0; i < n; i++) arr[i] = i / K;
        int unique = n / K;
        int found[n];
        long sum1 = 0, sum2 = 0;
        maxAll1[s] = 0; maxAll2[s] = 0;
        for (int key = 0; key < unique; key++) {
            int C1, C2;
            BSearchAll1(n, arr, key, found, &C1);
            BSearchAll2(n, arr, key, found, &C2);
            sum1 += C1; if (C1 > maxAll1[s]) maxAll1[s] = C1;
            sum2 += C2; if (C2 > maxAll2[s]) maxAll2[s] = C2;
        }
        avgAll1[s] = (double)sum1 / unique;
        avgAll2[s] = (double)sum2 / unique;
    }

    /* ===================================================================
     * Таблица 1: Трудоемкость двоичного поиска элемента (ТОЧНО ПО ТЗ)
     * =================================================================== */
    printf("┌────────────────────────────────────────────────────┐\n");
    printf("│       Трудоемкость двоичного поиска элемента       │\n");
    printf("├──────┬──────────────────┬──────────────────┤\n");
    printf("│  N   │  Сф I версия     │  Сф II версия    │\n");
    printf("├──────┼──────────────────┼──────────────────┤\n");
    for (int s = 0; s < ns; s++) {
        printf("│ %4d │ %16.2f │ %16.2f │\n",
            sizes[s], avg1[s], avg2[s]);
    }
    printf("└──────┴──────────────────┴──────────────────┘\n");

    /* ===================================================================
     * Таблица 2: Трудоемкость двоичного поиска всех элементов (ТОЧНО ПО ТЗ)
     * =================================================================== */
    printf("\n┌────────────────────────────────────────────────────────┐\n");
    printf("│   Трудоемкость двоичного поиска всех элементов        │\n");
    printf("├──────┬──────────────────────┬──────────────────────┤\n");
    printf("│  N   │  Сф All I версия     │  Сф All II версия    │\n");
    printf("├──────┼──────────────────────┼──────────────────────┤\n");
    for (int s = 0; s < ns; s++) {
        printf("│ %4d │ %20.2f │ %20.2f │\n",
            sizes[s], avgAll1[s], avgAll2[s]);
    }
    printf("└──────┴──────────────────────┴──────────────────────┘\n");

    /* ===================================================================
     * Таблица 3: Теория vs факт для обеих версий
     * Теория: BSearch1 C = 2*floor(log2(n)), BSearch2 C = floor(log2(n))
     * =================================================================== */
    printf("\n┌──────────────────────────────────────────────────────────────────────────────┐\n");
    printf("│              Двоичный поиск: теория vs факт                                  │\n");
    printf("├──────┬──────────────────┬──────────────────┬──────────────────┬──────────────────┤\n");
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

    /* Выводы */
    printf("\nВывод: BSearch2 (с одним сравнением на шаг) эффективнее BSearch1 по числу сравнений.\n");
    printf("Вывод: BSearchAll2 (lower/upper bound) эффективнее BSearchAll1 (линейное расширение) для повторяющихся элементов.\n");

    return 0;
}
