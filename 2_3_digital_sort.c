#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include "list.h"

static void ListFillRand16(List *L, int n) {
    ListFree(L);
    int M = 0;
    for (int i = 0; i < n; i++)
        QueueEnqueue(L, (int)(int16_t)(rand() % 65536 - 32768), &M);
}

static void ListFillRand32(List *L, int n) {
    ListFree(L);
    int M = 0;
    for (int i = 0; i < n; i++) {
        int32_t v = (int32_t)(((unsigned)rand() << 16) ^ (unsigned)rand());
        QueueEnqueue(L, (int)v, &M);
    }
}

int main(void) {
    srand(time(NULL));
    int sizes[] = {100, 200, 300, 400, 500};

    /* ══════════════════════════════════════════
     * 2.3.2 — DigitalSort для int (4 байта)
     * Теория: M = const * n = 8 * n  (4 байта * 2 операции за проход)
     * Сравнений нет (C = 0)
     * ══════════════════════════════════════════ */
    printf("╔════════════════════════════════════════════════════════════════════╗\n");
    printf("║      2.3.2 — Трудоёмкость DigitalSort (int, 4 байта)            ║\n");
    printf("╠══════╦════════════╦═════════════╦═════════════╦══════════════╣\n");
    printf("║  N   ║  M теор.   ║   Убыв.     ║   Случ.     ║    Возр.     ║\n");
    printf("║      ║  (8*n)     ║   Мфакт     ║   Мфакт     ║    Мфакт     ║\n");
    printf("╠══════╬════════════╬═════════════╬═════════════╬══════════════╣\n");

    for (int s = 0; s < 5; s++) {
        int n = sizes[s];
        int Md, Mr, Mi;
        List L;

        ListInit(&L); ListFillDec(&L, n);
        ListDigitalSort(&L, &Md); ListFree(&L);

        ListInit(&L); ListFillRand(&L, n);
        ListDigitalSort(&L, &Mr); ListFree(&L);

        ListInit(&L); ListFillInc(&L, n);
        ListDigitalSort(&L, &Mi); ListFree(&L);

        /* const = 4 байта * 2 (dequeue + enqueue) */
        int theory = 4 * 2 * n;
        printf("║ %4d ║ %10d ║ %11d ║ %11d ║ %12d ║\n",
               n, theory, Md, Mr, Mi);
    }
    printf("╚══════╩════════════╩═════════════╩═════════════╩══════════════╝\n");

    /* ══════════════════════════════════════════
     * 2.3.3 — int16_t и int32_t, прямой и обратный
     * ══════════════════════════════════════════ */
    printf("\n╔══════════════════════════════════════════════════════════════════════╗\n");
    printf("║     2.3.3 — DigitalSort для int16_t и int32_t (n=100)             ║\n");
    printf("╠══════════╦══════════╦═══════╦══════════╦═════════╦══════════════╣\n");
    printf("║   Тип    ║ Порядок  ║   n   ║   Мф     ║ Верно?  ║ Серий после  ║\n");
    printf("╠══════════╬══════════╬═══════╬══════════╬═════════╬══════════════╣\n");

    int n16 = 100, M;
    List L; ListInit(&L);

    /* int16_t по возрастанию */
    ListFillRand16(&L, n16);
    long long cs_before = ListChecksum(&L);
    ListDigitalSort16(&L, &M);
    long long cs_after = ListChecksum(&L);
    printf("║ int16_t  ║  возр.   ║ %5d ║ %8d ║  %-6s ║ %12d ║\n",
           n16, M, cs_before == cs_after ? "OK" : "FAIL", ListSeriesCount(&L));
    ListFree(&L);

    /* int16_t по убыванию */
    ListFillRand16(&L, n16);
    cs_before = ListChecksum(&L);
    ListDigitalSort16D(&L, &M);
    cs_after = ListChecksum(&L);
    printf("║ int16_t  ║  убыв.   ║ %5d ║ %8d ║  %-6s ║ %12d ║\n",
           n16, M, cs_before == cs_after ? "OK" : "FAIL", ListSeriesCount(&L));
    ListFree(&L);

    /* int32_t по возрастанию */
    int n32 = 100;
    ListFillRand32(&L, n32);
    cs_before = ListChecksum(&L);
    ListDigitalSort32(&L, &M);
    cs_after = ListChecksum(&L);
    printf("║ int32_t  ║  возр.   ║ %5d ║ %8d ║  %-6s ║ %12d ║\n",
           n32, M, cs_before == cs_after ? "OK" : "FAIL", ListSeriesCount(&L));
    ListFree(&L);

    /* int32_t по убыванию */
    ListFillRand32(&L, n32);
    cs_before = ListChecksum(&L);
    ListDigitalSort32D(&L, &M);
    cs_after = ListChecksum(&L);
    printf("║ int32_t  ║  убыв.   ║ %5d ║ %8d ║  %-6s ║ %12d ║\n",
           n32, M, cs_before == cs_after ? "OK" : "FAIL", ListSeriesCount(&L));
    ListFree(&L);

    printf("╚══════════╩══════════╩═══════╩══════════╩═════════╩══════════════╝\n");

    /* ══════════════════════════════════════════
     * 2.3.4 — Зависимость Мф от упорядоченности
     * ══════════════════════════════════════════ */
    printf("\n╔══════════════════════════════════════════════════════════════════════╗\n");
    printf("║    2.3.4 — DigitalSort: зависимость от упорядоченности            ║\n");
    printf("╠══════╦════════════╦═════════════╦═════════════╦══════════════╣\n");
    printf("║  N   ║  M теор.   ║   Убыв.     ║   Случ.     ║    Возр.     ║\n");
    printf("╠══════╬════════════╬═════════════╬═════════════╬══════════════╣\n");

    for (int s = 0; s < 5; s++) {
        int n = sizes[s];
        int Md, Mr, Mi;

        ListInit(&L); ListFillDec(&L, n);
        ListDigitalSort(&L, &Md); ListFree(&L);

        ListInit(&L); ListFillRand(&L, n);
        ListDigitalSort(&L, &Mr); ListFree(&L);

        ListInit(&L); ListFillInc(&L, n);
        ListDigitalSort(&L, &Mi); ListFree(&L);

        int theory = 4 * 2 * n;
        printf("║ %4d ║ %10d ║ %11d ║ %11d ║ %12d ║\n",
               n, theory, Md, Mr, Mi);
    }
    printf("╚══════╩════════════╩═════════════╩═════════════╩══════════════╝\n");

    printf("\nВывод: DigitalSort имеет строго линейную трудоёмкость O(d*n), C=0 (сравнений нет).\n");
    printf("Вывод: Мф не зависит от начальной упорядоченности — всегда const*n перемещений.\n");
    printf("Вывод: DigitalSort выгоднее QuickSort/HeapSort при малом числе байт в ключе.\n");

    return 0;
}
