#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "search.h"
#include "sorting.h"

/* ------------------------------------------------------------------ */
/* Вспомогательная: заполнить отсортированный массив 0..n-1           */
/* (используем FillInc из sorting.h)                                  */
/* ------------------------------------------------------------------ */

static void PrintFound(int key, int idx, int C, int pos_check) {
    (void)pos_check;
    if (idx >= 0)
        printf("  ключ=%-5d найден на позиции %-5d  C=%d\n", key, idx, C);
    else
        printf("  ключ=%-5d НЕ НАЙДЕН                  C=%d\n", key, C);
}

int main(void) {
    srand(time(NULL));

    /* ============================================================== */
    /* Задание 1: Проверка корректности на малом массиве              */
    /* ============================================================== */
    {
        int n = 16;
        int arr[16];
        FillInc(n, arr); /* arr = {0,1,2,...,15} */

        printf("\n=== Проверка корректности (n=%d, arr=0..%d) ===\n", n, n-1);
        int C, idx;

        printf("\n--- I версия (BSearch1) ---\n");
        idx = BSearch1(n, arr, arr[0],   &C); PrintFound(arr[0],   idx, C, 0);
        idx = BSearch1(n, arr, arr[n-1], &C); PrintFound(arr[n-1], idx, C, n-1);
        idx = BSearch1(n, arr, arr[n/2], &C); PrintFound(arr[n/2], idx, C, n/2);
        idx = BSearch1(n, arr, -1,       &C); PrintFound(-1,       idx, C, -1);
        idx = BSearch1(n, arr, n,        &C); PrintFound(n,        idx, C, -1);

        printf("\n--- II версия (BSearch2) ---\n");
        idx = BSearch2(n, arr, arr[0],   &C); PrintFound(arr[0],   idx, C, 0);
        idx = BSearch2(n, arr, arr[n-1], &C); PrintFound(arr[n-1], idx, C, n-1);
        idx = BSearch2(n, arr, arr[n/2], &C); PrintFound(arr[n/2], idx, C, n/2);
        idx = BSearch2(n, arr, -1,       &C); PrintFound(-1,       idx, C, -1);
        idx = BSearch2(n, arr, n,        &C); PrintFound(n,        idx, C, -1);
    }

    /* ============================================================== */
    /* Задание 2: Таблица трудоёмкости — сравнение I и II версии      */
    /* Измеряем среднее Cф по всем ключам массива                     */
    /* ============================================================== */
    {
        /* Размеры: 100, 200, 300, 400, 500, 600, 700, 800, 900, 1000 */
        int sizes[] = {100, 200, 300, 400, 500, 600, 700, 800, 900, 1000};
        int ns = 10;

        printf("\n\n");
        printf("┌──────┬──────────────────────────────────────────────────────────────────┐\n");
        printf("│      │         Трудоёмкость двоичного поиска (среднее Сф по всем ключам)│\n");
        printf("│  N   ├────────────┬────────────┬────────────┬────────────┬──────────────┤\n");
        printf("│      │ С теор.    │ Сф I (avg) │ Сф I (max) │ Сф II(avg) │ Сф II (max)  │\n");
        printf("├──────┼────────────┼────────────┼────────────┼────────────┼──────────────┤\n");

        for (int s = 0; s < ns; s++) {
            int n = sizes[s];
            int arr[n];
            FillInc(n, arr);

            double lg = log2((double)n);
            int Ctheory = (int)(lg + 1); /* C_worst = floor(log2(n))+1 */

            long sum1 = 0, sum2 = 0;
            int max1 = 0, max2 = 0;
            for (int key = 0; key < n; key++) {
                int C1, C2;
                BSearch1(n, arr, key, &C1);
                BSearch2(n, arr, key, &C2);
                sum1 += C1; if (C1 > max1) max1 = C1;
                sum2 += C2; if (C2 > max2) max2 = C2;
            }
            double avg1 = (double)sum1 / n;
            double avg2 = (double)sum2 / n;

            printf("│ %4d │ %10d │ %10.2f │ %10d │ %10.2f │ %12d │\n",
                n, Ctheory, avg1, max1, avg2, max2);
        }
        printf("└──────┴────────────┴────────────┴────────────┴────────────┴──────────────┘\n");
    }

    /* ============================================================== */
    /* Задание 3*: Поиск ВСЕХ вхождений — таблица Сф                  */
    /* Массив заполняем повторяющимися значениями: arr[i] = i / K,    */
    /* чтобы каждый ключ встречался K раз (K = n/10)                  */
    /* ============================================================== */
    {
        int sizes[] = {100, 200, 300, 400, 500, 600, 700, 800, 900, 1000};
        int ns = 10;

        printf("\n\n");
        printf("┌──────┬───────────────────────────────────────────────────────────────────┐\n");
        printf("│      │    Трудоёмкость поиска ВСЕХ вхождений (среднее Сф, K=%s повторов) │\n", "n/10");
        printf("│  N   ├────────────────┬────────────────┬────────────────┬────────────────┤\n");
        printf("│      │ Сф All1 (avg)  │ Сф All1 (max)  │ Сф All2 (avg)  │ Сф All2 (max)  │\n");
        printf("├──────┼────────────────┼────────────────┼────────────────┼────────────────┤\n");

        for (int s = 0; s < ns; s++) {
            int n = sizes[s];
            int K = n / 10; /* количество повторений каждого ключа */
            int arr[n];
            /* arr = {0,0,...,0, 1,1,...,1, ..., 9,9,...,9} */
            for (int i = 0; i < n; i++) arr[i] = i / K;
            int unique = n / K; /* число уникальных ключей */

            int found[n];
            long sum1 = 0, sum2 = 0;
            int max1 = 0, max2 = 0;
            for (int key = 0; key < unique; key++) {
                int C1, C2;
                BSearchAll1(n, arr, key, found, &C1);
                BSearchAll2(n, arr, key, found, &C2);
                sum1 += C1; if (C1 > max1) max1 = C1;
                sum2 += C2; if (C2 > max2) max2 = C2;
            }
            double avg1 = (double)sum1 / unique;
            double avg2 = (double)sum2 / unique;

            printf("│ %4d │ %14.2f │ %14d │ %14.2f │ %14d │\n",
                n, avg1, max1, avg2, max2);
        }
        printf("└──────┴────────────────┴────────────────┴────────────────┴────────────────┘\n");
    }

    return 0;
}
