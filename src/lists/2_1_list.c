#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "list.h"

#define DEMO_N 10

static void DemoListOps(const char *label, const List *L) {
    printf("── %s\n", label);
    printf("  Список:            "); ListPrint(L);
    printf("  Контрольная сумма: %lld\n", ListChecksum(L));
    printf("  Количество серий:  %d\n",   ListSeriesCount(L));
    printf("  Прямой порядок:    "); ListPrint(L);
    printf("  Обратный порядок:  "); ListPrintRev(L);
    printf("\n");
}

static void DemoListFree(const char *label, List *L) {
    printf("── %s\n", label);
    printf("  До очистки  (размер=%d): ", L->size); ListPrint(L);
    ListFree(L);
    printf("  После очистки (размер=%d): %s\n\n",
           L->size, L->head == NULL ? "(пусто)" : "???");
}

int main(void) {
    srand(time(NULL));

    /* ===================================================================
     * 2.1.1 — Стек: три варианта заполнения
     * =================================================================== */
    printf("┌──────────────────────────────────────────┐\n");
    printf("│         2.1.1 — Стек                     │\n");
    printf("└──────────────────────────────────────────┘\n");

    List S_inc, S_dec, S_rnd;
    int M;

    ListInit(&S_inc); M = 0;
    StackFillInc(&S_inc, DEMO_N, &M);
    printf("Стек (возр., M=%d): ", M); ListPrint(&S_inc);

    ListInit(&S_dec); M = 0;
    StackFillDec(&S_dec, DEMO_N, &M);
    printf("Стек (убыв., M=%d): ", M); ListPrint(&S_dec);

    ListInit(&S_rnd); M = 0;
    StackFillRand(&S_rnd, DEMO_N, &M);
    printf("Стек (случ., M=%d): ", M); ListPrint(&S_rnd);

    /* ===================================================================
     * 2.1.2 — Очередь: три варианта заполнения
     * =================================================================== */
    printf("\n┌──────────────────────────────────────────┐\n");
    printf("│         2.1.2 — Очередь                  │\n");
    printf("└──────────────────────────────────────────┘\n");

    List Q_inc, Q_dec, Q_rnd;

    ListInit(&Q_inc); M = 0;
    QueueFillInc(&Q_inc, DEMO_N, &M);
    printf("Очередь (возр., M=%d): ", M); ListPrint(&Q_inc);

    ListInit(&Q_dec); M = 0;
    QueueFillDec(&Q_dec, DEMO_N, &M);
    printf("Очередь (убыв., M=%d): ", M); ListPrint(&Q_dec);

    ListInit(&Q_rnd); M = 0;
    QueueFillRand(&Q_rnd, DEMO_N, &M);
    printf("Очередь (случ., M=%d): ", M); ListPrint(&Q_rnd);

    /* ===================================================================
     * 2.1.3 + 2.1.5* — Работа со списком
     *   печать, контрольная сумма, число серий, рекурсивная печать
     * =================================================================== */
    printf("\n┌──────────────────────────────────────────────────────────┐\n");
    printf("│  2.1.3 + 2.1.5* — Работа со списком                      │\n");
    printf("└──────────────────────────────────────────────────────────┘\n\n");

    DemoListOps("Стек возрастающий",    &S_inc);
    DemoListOps("Стек убывающий",       &S_dec);
    DemoListOps("Стек случайный",       &S_rnd);
    DemoListOps("Очередь возрастающая", &Q_inc);
    DemoListOps("Очередь убывающая",    &Q_dec);
    DemoListOps("Очередь случайная",    &Q_rnd);

    /* ===================================================================
     * 2.1.4* — Очистка списков
     * =================================================================== */
    printf("┌──────────────────────────────────────────┐\n");
    printf("│         2.1.4* — Очистка списков         │\n");
    printf("└──────────────────────────────────────────┘\n\n");

    DemoListFree("Стек возрастающий",    &S_inc);
    DemoListFree("Стек убывающий",       &S_dec);
    DemoListFree("Стек случайный",       &S_rnd);
    DemoListFree("Очередь возрастающая", &Q_inc);
    DemoListFree("Очередь убывающая",    &Q_dec);
    DemoListFree("Очередь случайная",    &Q_rnd);

    return 0;
}
