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

/* Печать списка с разметкой границ серий через '|' */
static void PrintWithSeries(const List *L) {
    Node *p = L->head;
    if (!p) { printf("(пусто)\n"); return; }
    printf("%d ", p->data);
    for (Node *q = p->next; q; p = q, q = q->next) {
        if (q->data < p->data) printf("| ");
        printf("%d ", q->data);
    }
    printf("\n");
}

int main(void) {
    srand(time(NULL));
    int sizes[] = {100, 200, 300, 400, 500};

    /* ===================================================================
     * 2.2.2 — Расщепление списка (n=20)
     * =================================================================== */
    printf("┌──────────────────────────────────────────────┐\n");
    printf("│      2.2.2 — Расщепление списка (n=20)       │\n");
    printf("└──────────────────────────────────────────────┘\n");
    DemoSplit(20);

    /* ===================================================================
     * 2.2.3 — Слияние серий: демонстрация на n=20
     * =================================================================== */
    printf("\n┌──────────────────────────────────────────────┐\n");
    printf("│       2.2.3 — Слияние серий (демо n=20)      │\n");
    printf("└──────────────────────────────────────────────┘\n");
    {
        List L; ListInit(&L);
        ListFillRand(&L, 20);
        long long sum_before = ListChecksum(&L);
        int       ser_before = ListSeriesCount(&L);

        printf("Исходный список (серии разделены '|'):\n  ");
        PrintWithSeries(&L);
        printf("Контрольная сумма до:  %lld\n", sum_before);
        printf("Количество серий до:   %d\n\n", ser_before);

        int M, C, ser_after;
        ListMergeSort(&L, &M, &C, &ser_after);
        long long sum_after = ListChecksum(&L);

        printf("Отсортированный список:\n  ");
        ListPrint(&L);
        printf("Контрольная сумма после: %lld  (%s)\n",
               sum_after, sum_before == sum_after ? "сумма сохранена" : "СУММА НАРУШЕНА");
        printf("Количество серий после:  %d  (%s)\n",
               ser_after, ser_after == 1 ? "одна серия — отсортировано" : "не одна серия!");
        printf("Трудоёмкость: M = %d, C = %d, M+C = %d\n", M, C, M + C);
        ListFree(&L);
    }

    /* Сводная таблица проверки для n=100..500 */
    printf("\n                     Проверка слияния для n=100..500\n");
    printf("┌──────┬────────────────┬────────────────┬──────────┬──────────┬───────┐\n");
    printf("│  N   │   Сумма до     │   Сумма после  │ Серий до │ Серий по │  OK   │\n");
    printf("├──────┼────────────────┼────────────────┼──────────┼──────────┼───────┤\n");
    int Md_save[5], Cd_save[5], Mr_save[5], Cr_save[5], Mi_save[5], Ci_save[5];
    for (int s = 0; s < 5; s++) {
        int n = sizes[s];
        List L; ListInit(&L);
        ListFillRand(&L, n);
        long long sum_before = ListChecksum(&L);
        int       ser_before = ListSeriesCount(&L);
        int M, C, ser_after;
        ListMergeSort(&L, &M, &C, &ser_after);
        long long sum_after = ListChecksum(&L);
        int ok = (sum_before == sum_after) && (ser_after == 1);
        printf("│ %4d │ %14lld │ %14lld │ %8d │ %8d │ %5s │\n",
               n, sum_before, sum_after, ser_before, ser_after,
               ok ? "OK" : "FAIL");
        ListFree(&L);
    }
    printf("└──────┴────────────────┴────────────────┴──────────┴──────────┴───────┘\n");

    /* Накапливаем M, C для основной таблицы и Доп. */
    for (int s = 0; s < 5; s++) {
        int n = sizes[s];
        int ser;
        List L;

        ListInit(&L); ListFillDec(&L, n);
        ListMergeSort(&L, &Md_save[s], &Cd_save[s], &ser); ListFree(&L);

        ListInit(&L); ListFillRand(&L, n);
        ListMergeSort(&L, &Mr_save[s], &Cr_save[s], &ser); ListFree(&L);

        ListInit(&L); ListFillInc(&L, n);
        ListMergeSort(&L, &Mi_save[s], &Ci_save[s], &ser); ListFree(&L);
    }

    /* ===================================================================
     * Основная таблица ТЗ: Трудоемкость сортировки прямого слияния
     * Теория (одинакова для всех случаев): M = C = n·log2(n),
     *   суммарное M+C = round(n·log2(n)) + round(n·log2(n))
     *   (складываем уже округлённые целые — последняя цифра должна
     *    совпасть с Доп.-таблицей и с фактической Мф+Сф; round(2·n·log2(n))
     *    из-за двойного округления может отличаться на 1, например для n=300)
     * =================================================================== */
    printf("\n                          Трудоемкость сортировки прямого слияния\n");
    printf("┌──────┬───────────────┬─────────────────────────────────────┐\n");
    printf("│      │               │           Мфакт+Сфакт               │\n");
    printf("│  N   │ M+C теоретич. ├───────────┬───────────┬─────────────┤\n");
    printf("│      │               │   Убыв.   │   Случ.   │    Возр.    │\n");
    printf("├──────┼───────────────┼───────────┼───────────┼─────────────┤\n");
    for (int s = 0; s < 5; s++) {
        int n    = sizes[s];
        int Mt   = (int)round(n * log2((double)n));
        int Ct   = (int)round(n * log2((double)n));
        int MCth = Mt + Ct;
        printf("│ %4d │ %13d │ %9d │ %9d │ %11d │\n",
               n, MCth,
               Md_save[s] + Cd_save[s],
               Mr_save[s] + Cr_save[s],
               Mi_save[s] + Ci_save[s]);
    }
    printf("└──────┴───────────────┴───────────┴───────────┴─────────────┘\n");

    /* ===================================================================
     * === Доп. === MergeSort: разбор M и C — теория vs факт
     * Теория: M = n·log2(n),  C = n·log2(n)
     * =================================================================== */
    printf("\n=== Доп. ===\n");
    printf("                                              MergeSort: теория vs факт по M и C\n");
    printf("┌──────┬─────────┬─────────┬──────────┬──────────┬──────────┬──────────┬──────────┬──────────┐\n");
    printf("│  N   │ Mt теор │ Ct теор │ Мф(убыв) │ Сф(убыв) │ Мф(случ) │ Сф(случ) │ Мф(возр) │ Сф(возр) │\n");
    printf("├──────┼─────────┼─────────┼──────────┼──────────┼──────────┼──────────┼──────────┼──────────┤\n");
    for (int s = 0; s < 5; s++) {
        int n = sizes[s];
        int Mt = (int)round(n * log2((double)n));
        int Ct = (int)round(n * log2((double)n));
        printf("│ %4d │ %7d │ %7d │ %8d │ %8d │ %8d │ %8d │ %8d │ %8d │\n",
               n, Mt, Ct,
               Md_save[s], Cd_save[s],
               Mr_save[s], Cr_save[s],
               Mi_save[s], Ci_save[s]);
    }
    printf("└──────┴─────────┴─────────┴──────────┴──────────┴──────────┴──────────┴──────────┴──────────┘\n");

    return 0;
}
