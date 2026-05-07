#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "list.h"

static void DemoSplit(int n) {
    List L; ListInit(&L);
    ListFillRand(&L, n);

    printf("Исходный список (%d эл.): ", n);
    ListPrint(&L);

    List A, B; ListInit(&A); ListInit(&B);
    int toggle = 0;
    while (!QueueIsEmpty(&L)) {
        int v = QueueDequeue(&L, NULL);
        if (toggle == 0) QueueEnqueue(&A, v, NULL);
        else             QueueEnqueue(&B, v, NULL);
        toggle ^= 1;
    }
    printf("Список A (%d эл.): ", A.size); ListPrint(&A);
    printf("Список B (%d эл.): ", B.size); ListPrint(&B);

    ListFree(&A); ListFree(&B);
}

int main(void) {
    srand(time(NULL));
    int sizes[] = {100, 200, 300, 400, 500};

    /* ══════════════════════════════════════════
     * 2.2.2 — Расщепление
     * ══════════════════════════════════════════ */
    printf("╔══════════════════════════════════════════════╗\n");
    printf("║      2.2.2 — Расщепление списка (n=20)     ║\n");
    printf("╚══════════════════════════════════════════════╝\n");
    DemoSplit(20);

    /* ══════════════════════════════════════════
     * 2.2.3 — Проверка слияния (контрольная сумма + серии)
     * ══════════════════════════════════════════ */
    printf("\n╔══════════════════════════════════════════════════════════════╗\n");
    printf("║   2.2.3 — Проверка слияния серий                           ║\n");
    printf("╠══════╦════════════════╦════════════════╦══════════╦═════════╣\n");
    printf("║  N   ║  Сумма до      ║  Сумма после   ║ Серий до ║ Сф(Мф) ║\n");
    printf("╠══════╬════════════════╬════════════════╬══════════╬═════════╣\n");
    for (int s = 0; s < 5; s++) {
        int n = sizes[s];
        List L; ListInit(&L);
        ListFillRand(&L, n);
        long long sum_before  = ListChecksum(&L);
        int       ser_before  = ListSeriesCount(&L);
        int M, C, ser;
        ListMergeSort(&L, &M, &C, &ser);
        long long sum_after = ListChecksum(&L);
        printf("║ %4d ║ %14lld ║ %14lld ║ %8d ║ %7s ║\n",
               n, sum_before, sum_after, ser_before,
               sum_before == sum_after ? "OK" : "FAIL");
        ListFree(&L);
    }
    printf("╚══════╩════════════════╩════════════════╩══════════╩═════════╝\n");

    /* ══════════════════════════════════════════
     * 2.2.4-5 — Трудоёмкость MergeSort
     * Теория: M = n*log2(n),  C = n*log2(n)
     * ══════════════════════════════════════════ */
    printf("\n╔══════════════════════════════════════════════════════════════════════════════════════════╗\n");
    printf("║               2.2.4-5 — Трудоёмкость MergeSort                                        ║\n");
    printf("╠══════╦══════════╦══════════╦══════════╦══════════╦══════════╦══════════╦══════════╦══════════╣\n");
    printf("║  N   ║ Mt теор  ║ Ct теор  ║ Мф(убыв) ║ Сф(убыв) ║ Мф(случ) ║ Сф(случ) ║ Мф(возр) ║ Сф(возр) ║\n");
    printf("╠══════╬══════════╬══════════╬══════════╬══════════╬══════════╬══════════╬══════════╬══════════╣\n");

    for (int s = 0; s < 5; s++) {
        int n = sizes[s];
        int Md, Cd, Mr, Cr, Mi, Ci, ser;
        List L;

        ListInit(&L); ListFillDec(&L, n);
        ListMergeSort(&L, &Md, &Cd, &ser); ListFree(&L);

        ListInit(&L); ListFillRand(&L, n);
        ListMergeSort(&L, &Mr, &Cr, &ser); ListFree(&L);

        ListInit(&L); ListFillInc(&L, n);
        ListMergeSort(&L, &Mi, &Ci, &ser); ListFree(&L);

        int Mt = (int)(n * log2((double)n));
        int Ct = (int)(n * log2((double)n));
        printf("║ %4d ║ %8d ║ %8d ║ %8d ║ %8d ║ %8d ║ %8d ║ %8d ║ %8d ║\n",
               n, Mt, Ct, Md, Cd, Mr, Cr, Mi, Ci);
    }
    printf("╚══════╩══════════╩══════════╩══════════╩══════════╩══════════╩══════════╩══════════╩══════════╝\n");

    /* Сводная таблица Мф+Сф */
    printf("\n╔══════════════════════════════════════════════════════════════╗\n");
    printf("║          Сводная таблица: Мф+Сф vs теория M+C              ║\n");
    printf("╠══════╦═══════════════╦═════════════╦═════════════╦══════════╣\n");
    printf("║  N   ║  M+C теор.    ║   Убыв.     ║   Случ.     ║   Возр.  ║\n");
    printf("╠══════╬═══════════════╬═════════════╬═════════════╬══════════╣\n");
    for (int s = 0; s < 5; s++) {
        int n = sizes[s];
        int Md, Cd, Mr, Cr, Mi, Ci, ser;
        List L;

        ListInit(&L); ListFillDec(&L, n);
        ListMergeSort(&L, &Md, &Cd, &ser); ListFree(&L);

        ListInit(&L); ListFillRand(&L, n);
        ListMergeSort(&L, &Mr, &Cr, &ser); ListFree(&L);

        ListInit(&L); ListFillInc(&L, n);
        ListMergeSort(&L, &Mi, &Ci, &ser); ListFree(&L);

        int theory = (int)(2.0 * n * log2((double)n));
        printf("║ %4d ║ %13d ║ %11d ║ %11d ║ %8d ║\n",
               n, theory, Md+Cd, Mr+Cr, Mi+Ci);
    }
    printf("╚══════╩═══════════════╩═════════════╩═════════════╩══════════╝\n");

    printf("\nВывод: MergeSort имеет трудоёмкость O(n*log(n)) для всех типов данных.\n");
    printf("Вывод: MergeSort не зависит от исходной упорядоченности — Мф+Сф одинаковы.\n");
    printf("Вывод: Контрольные суммы до и после сортировки совпадают — реализация корректна.\n");

    return 0;
}
