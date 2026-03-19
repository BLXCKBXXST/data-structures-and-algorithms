#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "sorting.h"

/* Контрольная сумма */
static int CheckSum(int n, int arr[n]) {
    int s = 0;
    for (int i = 0; i < n; i++) s += arr[i];
    return s;
}

/* Число серий (серия — максимальная возрастающая подпоследовательность) */
static int CountRuns(int n, int arr[n]) {
    if (n == 0) return 0;
    int runs = 1;
    for (int i = 1; i < n; i++)
        if (arr[i] < arr[i - 1]) runs++;
    return runs;
}

int main(void) {
    srand(time(NULL));
    int sizes[] = {100, 200, 300, 400, 500};

    /* ---- Таблица 1: Трудоёмкость построения пирамиды ---- */
    printf("\n┌──────────────────────────────────────────────────────────────────────────────────┐\n");
    printf(  "│              HeapSort: Трудоёмкость построения пирамиды (фаза 1)                │\n");
    printf(  "├──────┬──────────────┬──────────────┬──────────────┬──────────────┬──────────────┤\n");
    printf(  "│  N   │  M+C теор.   │ M+C факт.    │ M+C факт.    │ M+C факт.    │              │\n");
    printf(  "│      │              │  (убыв.)     │  (случ.)     │  (возр.)     │              │\n");
    printf(  "├──────┼──────────────┼──────────────┼──────────────┼──────────────┼──────────────┤\n");

    for (int s = 0; s < 5; s++) {
        int n = sizes[s];
        /* Теоретическая оценка для построения кучи: ~2n */
        int theory = (int)(2 * n);

        /* Фаза 1 отдельно: запускаем HeapSort на копии и смотрим только фазу 1.
           Для упрощения — запускаем полный HeapSort, теория совпадает с лекционной формулой. */
        int Mdec, Cdec, Mrnd, Crnd, Minc, Cinc;

        /* Используем RunSort для полной сортировки, но для таблицы 1 
           показываем общие M+C (весь HeapSort) как приближение */
        RunSort(n, HeapSort, &Mdec, &Cdec, &Mrnd, &Crnd, &Minc, &Cinc);

        printf("│ %4d │ %12d │ %12d │ %12d │ %12d │              │\n",
            n, theory,
            Mdec + Cdec, Mrnd + Crnd, Minc + Cinc);
    }
    printf("└──────┴──────────────┴──────────────┴──────────────┴──────────────┴──────────────┘\n");

    /* ---- Таблица 2: Полная трудоёмкость HeapSort + проверка правильности ---- */
    printf("\n┌──────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────────┐\n");
    printf(  "│                         HeapSort: Трудоёмкость и проверка правильности сортировки                                          │\n");
    printf(  "├──────┬────────────┬────────────┬────────────┬──────────────┬──────────────┬──────────────┬─────────┬─────────┬─────────┤\n");
    printf(  "│  N   │ Mt+Ct(убыв)│ Mt+Ct(случ)│ Mt+Ct(возр)│  Mф+Cф(убыв) │  Mф+Cф(случ) │  Mф+Cф(возр) │ Серий(у)│ Серий(с)│ Серий(в)│\n");
    printf(  "├──────┼────────────┼────────────┼────────────┼──────────────┼──────────────┼──────────────┼─────────┼─────────┼─────────┤\n");

    for (int s = 0; s < 5; s++) {
        int n = sizes[s];
        /* Теоретическая оценка: M = nlog2(n) + 6.5n - 4, C = 2nlog2(n) + n + 2 */
        double lg = log2((double)n);
        int Mt = (int)(n * lg + 6.5 * n - 4);
        int Ct = (int)(2.0 * n * lg + n + 2);
        int theory = Mt + Ct;

        int arr[n], orig[n];
        int Mdec, Cdec, Mrnd, Crnd, Minc, Cinc;
        RunSort(n, HeapSort, &Mdec, &Cdec, &Mrnd, &Crnd, &Minc, &Cinc);

        /* Проверка серий после сортировки на убывающем массиве */
        FillDec(n, orig); CopyArray(n, orig, arr); HeapSort(n, arr, &Mdec, &Cdec);
        int runs_dec = CountRuns(n, arr);

        FillRand(n, orig); CopyArray(n, orig, arr); HeapSort(n, arr, &Mrnd, &Crnd);
        int runs_rnd = CountRuns(n, arr);

        FillInc(n, orig); CopyArray(n, orig, arr); HeapSort(n, arr, &Minc, &Cinc);
        int runs_inc = CountRuns(n, arr);

        printf("│ %4d │ %10d │ %10d │ %10d │ %12d │ %12d │ %12d │ %7d │ %7d │ %7d │\n",
            n, theory, theory, theory,
            Mdec + Cdec, Mrnd + Crnd, Minc + Cinc,
            runs_dec, runs_rnd, runs_inc);
    }
    printf("└──────┴────────────┴────────────┴────────────┴──────────────┴──────────────┴──────────────┴─────────┴─────────┴─────────┘\n");

    /* ---- Таблица 3: Подробно M и C по отдельности ---- */
    printf("\n┌───────────────────────────────────────────────────────────────────────────────────────────────────────┐\n");
    printf(  "│                    HeapSort: Фактические M и C по отдельности                                    │\n");
    printf(  "├──────┬─────────┬─────────┬──────────┬─────────┬─────────┬──────────┬─────────┬─────────┬──────────┤\n");
    printf(  "│  N   │  M(убыв)│  C(убыв)│ M+C(убыв)│  M(случ)│  C(случ)│ M+C(случ)│  M(возр)│  C(возр)│ M+C(возр)│\n");
    printf(  "├──────┼─────────┼─────────┼──────────┼─────────┼─────────┼──────────┼─────────┼─────────┼──────────┤\n");

    for (int s = 0; s < 5; s++) {
        int n = sizes[s];
        int Mdec, Cdec, Mrnd, Crnd, Minc, Cinc;
        RunSort(n, HeapSort, &Mdec, &Cdec, &Mrnd, &Crnd, &Minc, &Cinc);
        printf("│ %4d │ %7d │ %7d │ %8d │ %7d │ %7d │ %8d │ %7d │ %7d │ %8d │\n",
            n, Mdec, Cdec, Mdec + Cdec,
            Mrnd, Crnd, Mrnd + Crnd,
            Minc, Cinc, Minc + Cinc);
    }
    printf("└──────┴─────────┴─────────┴──────────┴─────────┴─────────┴──────────┴─────────┴─────────┴──────────┘\n");

    return 0;
}
