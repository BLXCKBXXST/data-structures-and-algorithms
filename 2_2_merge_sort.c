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
    printf("\n╔══════════════════════════════════════════════════════════════════════╗\n");
    printf("║   2.2.3 — Проверка слияния серий                                     ║\n");
    printf("╠══════╦════════════════╦════════════════╦══════════╦══════════╦═══════╣\n");
    printf("║  N   ║  Сумма до      ║  Сумма после   ║ Серий до ║ Серий по ║  OK   ║\n");
    printf("╠══════╬════════════════╬════════════════╬══════════╬══════════╬═══════╣\n");
    for (int s = 0; s < 5; s++) {
        int n = sizes[s];
        List L; ListInit(&L);
        ListFillRand(&L, n);
        long long sum_before  = ListChecksum(&L);
        int       ser_before  = ListSeriesCount(&L);
        int M, C, ser_after;
        ListMergeSort(&L, &M, &C, &ser_after);
        long long sum_after = ListChecksum(&L);
        int ok = (sum_before == sum_after) && (ser_after == 1);
        printf("║ %4d ║ %14lld ║ %14lld ║ %8d ║ %8d ║ %5s ║\n",
               n, sum_before, sum_after, ser_before, ser_after,
               ok ? "OK" : "FAIL");
        ListFree(&L);
    }
    printf("╚══════╩════════════════╩════════════════╩══════════╩══════════╩═══════╝\n");

    /* ══════════════════════════════════════════
     * 2.2.4-5 — Трудоёмкость MergeSort
     *
     * Теория (из таблицы трудоёмкости):
     *   M = n * ceil(log2(n))   — каждый элемент пересылается ровно
     *                             ceil(log2(n)) раз (по одному проходу)
     *   C ≤ M, строго меньше:   сравнение происходит только когда ОБА
     *                             блока не исчерпаны; хвост идёт без
     *                             сравнений. Зависит от данных:
     *                             убыв → C минимально,
     *                             случ → C среднее,
     *                             возр → C ≈ n/2 * ceil(log2(n)).
     * ══════════════════════════════════════════ */
    printf("\n╔══════════════════════════════════════════════════════════════════════════════════════════╗\n");
    printf("║               2.2.4-5 — Трудоёмкость MergeSort                                        ║\n");
    printf("╠══════╦══════════╦══════════╦══════════╦══════════╦══════════╦══════════╦══════════╗\n");
    printf("║  N   ║ Mt теор  ║ Мф(убыв) ║ Сф(убыв) ║ Мф(случ) ║ Сф(случ) ║ Мф(возр) ║ Сф(возр) ║\n");
    printf("╠══════╬══════════╬══════════╬══════════╬══════════╬══════════╬══════════╬══════════╣\n");

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

        /* Точная теоретическая формула: M = n * ceil(log2(n)) */
        int Mt = n * (int)ceil(log2((double)n));

        printf("║ %4d ║ %8d ║ %8d ║ %8d ║ %8d ║ %8d ║ %8d ║ %8d ║\n",
               n, Mt, Md, Cd, Mr, Cr, Mi, Ci);
    }
    printf("╚══════╩══════════╩══════════╩══════════╩══════════╩══════════╩══════════╩══════════╝\n");

    /* Сводная таблица: Мф vs Mt, и диапазон Сф */
    printf("\n╔══════════════════════════════════════════════════════════════════════════╗\n");
    printf("║     Сводная таблица: Мф = Mt (всегда); C_min ≤ Сф ≤ C_max             ║\n");
    printf("╠══════╦══════════╦══════════╦════════════════════════╦══════════╗\n");
    printf("║  N   ║ Mt теор  ║ Мф(факт) ║     Сф: убыв / случ / возр       ║\n");
    printf("╠══════╬══════════╬══════════╬════════════════════════════════════╣\n");
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

        int Mt = n * (int)ceil(log2((double)n));

        printf("║ %4d ║ %8d ║ %8d ║  %6d / %6d / %6d  ║\n",
               n, Mt, Md, Cd, Cr, Ci);
    }
    printf("╚══════╩══════════╩══════════╩════════════════════════════════════╝\n");

    return 0;
}
