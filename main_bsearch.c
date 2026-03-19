#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "search.h"
#include "sorting.h"

static void PrintFound(int key, int idx, int C, int pos_check) {
    (void)pos_check;
    if (idx >= 0)
        printf("  ключ=%-5d найден на поз. %-5d  C=%d\n", key, idx, C);
    else
        printf("  ключ=%-5d НЕ НАЙДЕН              C=%d\n", key, C);
}

int main(void) {
    srand(time(NULL));

    /* ==============================================================
     * Задание 1: Проверка корректности (n=16)
     * ============================================================== */
    {
        int n = 16;
        int arr[16];
        FillInc(n, arr);
        int C, idx;

        printf("\n=== Проверка корректности (n=%d, arr=0..%d) ===\n", n, n-1);
        printf("\n┌──────────────────────────────────────┐\n");
        printf(  "│ I версия (BSearch1)                    │\n");
        printf(  "├───────────┬─────────────┬─────────────┤\n");
        printf(  "│   Ключ   │  Позиция   │      C      │\n");
        printf(  "├───────────┼─────────────┼─────────────┤\n");
        /* Первый */
        idx = BSearch1(n, arr, arr[0], &C);
        printf("│ %9d │ %11d │ %11d │\n", arr[0], idx, C);
        /* Последний */
        idx = BSearch1(n, arr, arr[n-1], &C);
        printf("│ %9d │ %11d │ %11d │\n", arr[n-1], idx, C);
        /* Средний */
        idx = BSearch1(n, arr, arr[n/2], &C);
        printf("│ %9d │ %11d │ %11d │\n", arr[n/2], idx, C);
        /* Нет в массиве */
        idx = BSearch1(n, arr, -1, &C);
        printf("│ %9d │ %11s │ %11d │\n", -1, "НЕТ", C);
        idx = BSearch1(n, arr, n,  &C);
        printf("│ %9d │ %11s │ %11d │\n", n,  "НЕТ", C);
        printf("└───────────┴─────────────┴─────────────┘\n");

        printf("┌──────────────────────────────────────┐\n");
        printf(  "│ II версия (BSearch2)                   │\n");
        printf(  "├───────────┬─────────────┬─────────────┤\n");
        printf(  "│   Ключ   │  Позиция   │      C      │\n");
        printf(  "├───────────┼─────────────┼─────────────┤\n");
        idx = BSearch2(n, arr, arr[0], &C);
        printf("│ %9d │ %11d │ %11d │\n", arr[0], idx, C);
        idx = BSearch2(n, arr, arr[n-1], &C);
        printf("│ %9d │ %11d │ %11d │\n", arr[n-1], idx, C);
        idx = BSearch2(n, arr, arr[n/2], &C);
        printf("│ %9d │ %11d │ %11d │\n", arr[n/2], idx, C);
        idx = BSearch2(n, arr, -1, &C);
        printf("│ %9d │ %11s │ %11d │\n", -1, "НЕТ", C);
        idx = BSearch2(n, arr, n,  &C);
        printf("│ %9d │ %11s │ %11d │\n", n,  "НЕТ", C);
        printf("└───────────┴─────────────┴─────────────┘\n");
    }

    /* ==============================================================
     * Задание 2: Таблица трудоёмкости
     * 2А: N | С теор. | Сф I avg | Сф I max
     * 2Б: N | Сф II avg | Сф II max
     * Склеены друг за другом
     * ============================================================== */
    {
        int sizes[] = {100, 200, 300, 400, 500, 600, 700, 800, 900, 1000};
        int ns = 10;

        /* Считаем данные для обеих частей */
        double avg1[10], avg2[10];
        int max1[10], max2[10], Cth[10];
        for (int s = 0; s < ns; s++) {
            int n = sizes[s];
            int arr[n];
            FillInc(n, arr);
            Cth[s] = (int)(log2((double)n) + 1);
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

        /* --- 2А: I версия --- */
        printf("\n┌──────┬────────────┬────────────┬────────────┐\n");
        printf(  "│      │ Трудоёмкость: I версия (BSearch1) │\n");
        printf(  "│  N   ├────────────┬────────────┬────────────┤\n");
        printf(  "│      │  С теор.    │  Сф (avg)  │  Сф (max)  │\n");
        printf(  "├──────┼────────────┼────────────┼────────────┤\n");
        for (int s = 0; s < ns; s++)
            printf("│ %4d │ %10d │ %10.2f │ %10d │\n",
                sizes[s], Cth[s], avg1[s], max1[s]);
        printf("└──────┴────────────┴────────────┴────────────┘\n");

        /* --- 2Б: II версия --- */
        printf("┌──────┬────────────┬────────────┬────────────┐\n");
        printf(  "│      │ Трудоёмкость: II версия (BSearch2)│\n");
        printf(  "│  N   ├────────────┬────────────┬────────────┤\n");
        printf(  "│      │  С теор.    │  Сф (avg)  │  Сф (max)  │\n");
        printf(  "├──────┼────────────┼────────────┼────────────┤\n");
        for (int s = 0; s < ns; s++)
            printf("│ %4d │ %10d │ %10.2f │ %10d │\n",
                sizes[s], Cth[s], avg2[s], max2[s]);
        printf("└──────┴────────────┴────────────┴────────────┘\n");
    }

    /* ==============================================================
     * Задание 3*: Поиск ВСЕХ вхождений
     * 3А: N | Сф All1 avg | Сф All1 max
     * 3Б: N | Сф All2 avg | Сф All2 max
     * Склеены друг за другом
     * ============================================================== */
    {
        int sizes[] = {100, 200, 300, 400, 500, 600, 700, 800, 900, 1000};
        int ns = 10;

        double avg1[10], avg2[10];
        int max1[10], max2[10];
        for (int s = 0; s < ns; s++) {
            int n = sizes[s];
            int K = n / 10;
            int arr[n];
            for (int i = 0; i < n; i++) arr[i] = i / K;
            int unique = n / K;
            int found[n];
            long sum1 = 0, sum2 = 0;
            max1[s] = 0; max2[s] = 0;
            for (int key = 0; key < unique; key++) {
                int C1, C2;
                BSearchAll1(n, arr, key, found, &C1);
                BSearchAll2(n, arr, key, found, &C2);
                sum1 += C1; if (C1 > max1[s]) max1[s] = C1;
                sum2 += C2; if (C2 > max2[s]) max2[s] = C2;
            }
            avg1[s] = (double)sum1 / unique;
            avg2[s] = (double)sum2 / unique;
        }

        /* --- 3А: BSearchAll1 --- */
        printf("\n┌──────┬──────────────┬──────────────┐\n");
        printf(  "│      │ BSearchAll1: поиск всех вхождений│\n");
        printf(  "│  N   ├──────────────┬──────────────┤\n");
        printf(  "│      │  Сф (avg)     │  Сф (max)     │\n");
        printf(  "├──────┼──────────────┼──────────────┤\n");
        for (int s = 0; s < ns; s++)
            printf("│ %4d │ %12.2f │ %12d │\n",
                sizes[s], avg1[s], max1[s]);
        printf("└──────┴──────────────┴──────────────┘\n");

        /* --- 3Б: BSearchAll2 --- */
        printf("┌──────┬──────────────┬──────────────┐\n");
        printf(  "│      │ BSearchAll2: поиск всех вхождений│\n");
        printf(  "│  N   ├──────────────┬──────────────┤\n");
        printf(  "│      │  Сф (avg)     │  Сф (max)     │\n");
        printf(  "├──────┼──────────────┼──────────────┤\n");
        for (int s = 0; s < ns; s++)
            printf("│ %4d │ %12.2f │ %12d │\n",
                sizes[s], avg2[s], max2[s]);
        printf("└──────┴──────────────┴──────────────┘\n");
    }

    return 0;
}
