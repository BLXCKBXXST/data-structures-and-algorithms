#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "list.h"

#define DEMO_N 10

int main(void) {
    srand(time(NULL));

    /* ══════════════════════════════════════════
     * 2.1.1 — Стек: три варианта заполнения
     * ══════════════════════════════════════════ */
    printf("╔══════════════════════════════════════════╗\n");
    printf("║         2.1.1 — Стек                     ║\n");
    printf("╚══════════════════════════════════════════╝\n");

    List S; int M;

    ListInit(&S); M = 0;
    StackFillInc(&S, DEMO_N, &M);
    printf("Стек (возр., M=%d): ", M); ListPrint(&S); ListFree(&S);

    ListInit(&S); M = 0;
    StackFillDec(&S, DEMO_N, &M);
    printf("Стек (убыв., M=%d): ", M); ListPrint(&S); ListFree(&S);

    ListInit(&S); M = 0;
    StackFillRand(&S, DEMO_N, &M);
    printf("Стек (случ., M=%d): ", M); ListPrint(&S); ListFree(&S);

    /* ══════════════════════════════════════════
     * 2.1.2 — Очередь: три варианта заполнения
     * ══════════════════════════════════════════ */
    printf("\n╔══════════════════════════════════════════╗\n");
    printf("║         2.1.2 — Очередь                  ║\n");
    printf("╚══════════════════════════════════════════╝\n");

    List Q;

    ListInit(&Q); M = 0;
    QueueFillInc(&Q, DEMO_N, &M);
    printf("Очередь (возр., M=%d): ", M); ListPrint(&Q); ListFree(&Q);

    ListInit(&Q); M = 0;
    QueueFillDec(&Q, DEMO_N, &M);
    printf("Очередь (убыв., M=%d): ", M); ListPrint(&Q); ListFree(&Q);

    ListInit(&Q); M = 0;
    QueueFillRand(&Q, DEMO_N, &M);
    printf("Очередь (случ., M=%d): ", M); ListPrint(&Q); ListFree(&Q);

    /* ══════════════════════════════════════════
     * 2.1.3 — Работа со списком
     * ══════════════════════════════════════════ */
    printf("\n╔══════════════════════════════════════════╗\n");
    printf("║         2.1.3 — Список                   ║\n");
    printf("╚══════════════════════════════════════════╝\n");

    List L; ListInit(&L);
    QueueFillRand(&L, DEMO_N, NULL);

    printf("Список: "); ListPrint(&L);
    printf("Контрольная сумма: %lld\n", ListChecksum(&L));
    printf("Количество серий:  %d\n",   ListSeriesCount(&L));

    /* 5*: рекурсивная печать */
    printf("\n╔══════════════════════════════════════════╗\n");
    printf("║   2.1.5* — Рекурсивная печать            ║\n");
    printf("╚══════════════════════════════════════════╝\n");
    printf("Прямой порядок:   "); ListPrint(&L);
    printf("Обратный порядок: "); ListPrintRev(&L);

    /* 4*: удаление всех элементов */
    printf("\n╔══════════════════════════════════════════╗\n");
    printf("║   2.1.4* — Очистка списка                ║\n");
    printf("╚══════════════════════════════════════════╝\n");
    ListFree(&L);
    printf("Размер после очистки: %d\n", L.size);
    printf("Список пуст: %s\n", L.head == NULL ? "да" : "нет");

    return 0;
}
