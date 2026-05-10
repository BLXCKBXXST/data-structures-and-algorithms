#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include "list.h"
#include "sorting.h"

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

/* ============================================================
 * Параметризованный LSD-radix по произвольному числу байтов.
 * Используется только для 2.3.7* (timing vs QuickSort).
 * ============================================================ */
static void ListDigitalSortBytes(List *L, int bytes, int *M) {
    *M = 0;
    if (bytes <= 0) return;
    for (int b = 0; b < bytes; b++) {
        List buckets[256];
        for (int i = 0; i < 256; i++) ListInit(&buckets[i]);
        while (!QueueIsEmpty(L)) {
            int val = QueueDequeue(L, M);
            int bucket = ((unsigned int)val >> (b * 8)) & 0xFF;
            QueueEnqueue(&buckets[bucket], val, M);
        }
        for (int i = 0; i < 256; i++)
            while (!QueueIsEmpty(&buckets[i]))
                QueueEnqueue(L, QueueDequeue(&buckets[i], M), M);
    }
}

/* ============================================================
 * ASCII-бар-чарт (для 2.3.5*)
 * ============================================================ */
static void DrawBar(const char *label, double value, double max_value, int width) {
    int len = (int)((value / max_value) * width + 0.5);
    if (len > width) len = width;
    printf("%-12s │", label);
    for (int i = 0; i < len; i++) printf("█");
    for (int i = len; i < width; i++) printf(" ");
    printf("│ %.0f\n", value);
}

int main(void) {
    srand(time(NULL));
    int sizes[] = {100, 200, 300, 400, 500};
    List L;

    /* Накапливаем Mф для int (4 байта) — главная таблица + Доп. */
    int Md[5], Mr[5], Mi[5];
    for (int s = 0; s < 5; s++) {
        int n = sizes[s];
        ListInit(&L); ListFillDec(&L, n);
        ListDigitalSort(&L, &Md[s]); ListFree(&L);
        ListInit(&L); ListFillRand(&L, n);
        ListDigitalSort(&L, &Mr[s]); ListFree(&L);
        ListInit(&L); ListFillInc(&L, n);
        ListDigitalSort(&L, &Mi[s]); ListFree(&L);
    }

    /* ===================================================================
     * Основная таблица ТЗ: Трудоемкость цифровой сортировки DigitalSort
     * Теория (одинаково для всех случаев): M = const · n,  C = 0
     * Для int (4 байта): const = 4·2 = 8 → M = 8n
     * =================================================================== */
    printf("                          Трудоемкость цифровой сортировки DigitalSort (int)\n");
    printf("┌──────┬───────────────┬─────────────────────────────────────┐\n");
    printf("│      │               │              Мфакт                  │\n");
    printf("│  N   │  M теоретич.  ├───────────┬───────────┬─────────────┤\n");
    printf("│      │               │   Убыв.   │   Случ.   │    Возр.    │\n");
    printf("├──────┼───────────────┼───────────┼───────────┼─────────────┤\n");
    for (int s = 0; s < 5; s++) {
        int n = sizes[s];
        int Mth = 4 * 4 * n;  /* const = bytes·4 (раздача + сборка по 2 пересылки) */
        printf("│ %4d │ %13d │ %9d │ %9d │ %11d │\n",
               n, Mth, Md[s], Mr[s], Mi[s]);
    }
    printf("└──────┴───────────────┴───────────┴───────────┴─────────────┘\n");

    /* ===================================================================
     * 2.3.3 — DigitalSort для int16_t и int32_t (прямой и обратный)
     *  Демо на n=12 с реальным выводом исходного и отсортированного списков.
     * =================================================================== */
    printf("\n┌──────────────────────────────────────────────────────────────┐\n");
    printf("│  2.3.3 — DigitalSort для int16_t и int32_t (демо n=12)       │\n");
    printf("└──────────────────────────────────────────────────────────────┘\n");

    int M;
    int demo_n = 12;

    /* int16_t — по возрастанию */
    printf("\n── int16_t, по возрастанию ──\n");
    ListInit(&L); ListFillRand16(&L, demo_n);
    long long cs_before = ListChecksum(&L);
    int ser_before = ListSeriesCount(&L);
    printf("Исходный список:        "); ListPrint(&L);
    ListDigitalSort16(&L, &M);
    long long cs_after = ListChecksum(&L);
    printf("Отсортированный (возр.): "); ListPrint(&L);
    printf("M = %d, контр.сумма: %lld → %lld (%s), серий: %d → %d\n",
           M, cs_before, cs_after,
           cs_before == cs_after ? "OK" : "FAIL",
           ser_before, ListSeriesCount(&L));
    ListFree(&L);

    /* int16_t — по убыванию */
    printf("\n── int16_t, по убыванию ──\n");
    ListInit(&L); ListFillRand16(&L, demo_n);
    cs_before = ListChecksum(&L);
    ser_before = ListSeriesCount(&L);
    printf("Исходный список:        "); ListPrint(&L);
    ListDigitalSort16D(&L, &M);
    cs_after = ListChecksum(&L);
    printf("Отсортированный (убыв.): "); ListPrint(&L);
    printf("M = %d, контр.сумма: %lld → %lld (%s), серий: %d → %d\n",
           M, cs_before, cs_after,
           cs_before == cs_after ? "OK" : "FAIL",
           ser_before, ListSeriesCount(&L));
    ListFree(&L);

    /* int32_t — по возрастанию */
    printf("\n── int32_t, по возрастанию ──\n");
    ListInit(&L); ListFillRand32(&L, demo_n);
    cs_before = ListChecksum(&L);
    ser_before = ListSeriesCount(&L);
    printf("Исходный список:        "); ListPrint(&L);
    ListDigitalSort32(&L, &M);
    cs_after = ListChecksum(&L);
    printf("Отсортированный (возр.): "); ListPrint(&L);
    printf("M = %d, контр.сумма: %lld → %lld (%s), серий: %d → %d\n",
           M, cs_before, cs_after,
           cs_before == cs_after ? "OK" : "FAIL",
           ser_before, ListSeriesCount(&L));
    ListFree(&L);

    /* int32_t — по убыванию */
    printf("\n── int32_t, по убыванию ──\n");
    ListInit(&L); ListFillRand32(&L, demo_n);
    cs_before = ListChecksum(&L);
    ser_before = ListSeriesCount(&L);
    printf("Исходный список:        "); ListPrint(&L);
    ListDigitalSort32D(&L, &M);
    cs_after = ListChecksum(&L);
    printf("Отсортированный (убыв.): "); ListPrint(&L);
    printf("M = %d, контр.сумма: %lld → %lld (%s), серий: %d → %d\n",
           M, cs_before, cs_after,
           cs_before == cs_after ? "OK" : "FAIL",
           ser_before, ListSeriesCount(&L));
    ListFree(&L);

    /* ===================================================================
     * 2.3.5* — Сравнительная таблица + ASCII-бар-чарт
     * для HeapSort, QuickSort, MergeSort, DigitalSort на случайных данных
     * =================================================================== */
    printf("\n                  2.3.5* — Сравнение четырёх методов на случайных числах (Мф+Сф)\n");
    printf("┌──────┬──────────────┬──────────────┬──────────────┬──────────────┐\n");
    printf("│      │   HeapSort   │  QuickSort   │  MergeSort   │ DigitalSort  │\n");
    printf("│  N   │   M+C        │    M+C       │    M+C       │     M (C=0)  │\n");
    printf("├──────┼──────────────┼──────────────┼──────────────┼──────────────┤\n");
    int H[5], Q[5], Mg[5], D[5];
    for (int s = 0; s < 5; s++) {
        int n = sizes[s];
        int arr[n], src[n];
        FillRand(n, src);

        int M_, C_, depth;
        memcpy(arr, src, sizeof(int) * n);
        HeapSort(n, arr, &M_, &C_);
        H[s] = M_ + C_;

        memcpy(arr, src, sizeof(int) * n);
        QuickSort1(n, arr, &M_, &C_, &depth);
        Q[s] = M_ + C_;

        ListInit(&L);
        int dM = 0; (void)dM;
        for (int i = 0; i < n; i++) QueueEnqueue(&L, src[i], NULL);
        int Mm, Cm, ser;
        ListMergeSort(&L, &Mm, &Cm, &ser);
        ListFree(&L);
        Mg[s] = Mm + Cm;

        ListInit(&L);
        for (int i = 0; i < n; i++) QueueEnqueue(&L, src[i], NULL);
        int Md_;
        ListDigitalSort(&L, &Md_);
        ListFree(&L);
        D[s] = Md_;

        printf("│ %4d │ %12d │ %12d │ %12d │ %12d │\n",
               n, H[s], Q[s], Mg[s], D[s]);
    }
    printf("└──────┴──────────────┴──────────────┴──────────────┴──────────────┘\n");

    /* ASCII-бар-чарт для n=500 */
    printf("\n  ASCII-график (n=500): длина бара пропорциональна трудоёмкости\n");
    int s5 = 4;
    double mx = H[s5];
    if (Q[s5]  > mx) mx = Q[s5];
    if (Mg[s5] > mx) mx = Mg[s5];
    if (D[s5]  > mx) mx = D[s5];
    int W = 50;
    printf("             ┌"); for (int i = 0; i < W; i++) printf("─"); printf("┐\n");
    DrawBar("HeapSort",    H[s5],  mx, W);
    DrawBar("QuickSort",   Q[s5],  mx, W);
    DrawBar("MergeSort",   Mg[s5], mx, W);
    DrawBar("DigitalSort", D[s5],  mx, W);
    printf("             └"); for (int i = 0; i < W; i++) printf("─"); printf("┘\n");

    /* ===================================================================
     * 2.3.6* — DigitalSort для списка фамилий (русский алфавит)
     *  Кириллица в UTF-8: каждая буква А..я занимает 2 байта,
     *  байтовый порядок UTF-8 совпадает с алфавитным порядком.
     * =================================================================== */
    printf("\n                          2.3.6* — DigitalSort для списка фамилий (русский алфавит)\n");
    const char *surnames[] = {
        "Петров", "Иванов", "Сидоров", "Козлов", "Андреев",
        "Смирнов", "Волков", "Михайлов", "Фёдоров", "Павлов",
    };
    int cnt = sizeof(surnames) / sizeof(surnames[0]);

    StrList SL; StrListInit(&SL);
    for (int i = 0; i < cnt; i++) StrListAppend(&SL, surnames[i]);
    printf("Исходный список:         ");
    StrListPrint(&SL);

    int Ms;
    StrListDigitalSort(&SL, 1, &Ms);
    printf("По возрастанию (M=%d): ", Ms);
    StrListPrint(&SL);

    StrListFree(&SL);
    StrListInit(&SL);
    for (int i = 0; i < cnt; i++) StrListAppend(&SL, surnames[i]);
    StrListDigitalSort(&SL, 0, &Ms);
    printf("По убыванию  (M=%d):  ", Ms);
    StrListPrint(&SL);
    StrListFree(&SL);

    /* ===================================================================
     * 2.3.7* — Точка перелома: при каком кол-ве байтов DigitalSort
     * становится медленнее QuickSort (n фиксировано)
     * =================================================================== */
    printf("\n                  2.3.7* — DigitalSort vs QuickSort по размеру байтов\n");
    int n7 = 50000;
    printf("  Размер выборки n = %d\n", n7);
    printf("┌────────┬──────────────────┬──────────────────┬──────────────┐\n");
    printf("│ Байты  │ DigitalSort, мс  │ QuickSort, мс    │  Победитель  │\n");
    printf("├────────┼──────────────────┼──────────────────┼──────────────┤\n");

    for (int b = 1; b <= 8; b++) {
        /* DigitalSort: список из случайных int */
        ListInit(&L);
        for (int i = 0; i < n7; i++) {
            int v = rand();
            QueueEnqueue(&L, v, NULL);
        }
        clock_t t1 = clock();
        int Mb;
        ListDigitalSortBytes(&L, b, &Mb);
        clock_t t2 = clock();
        double tD = 1000.0 * (double)(t2 - t1) / CLOCKS_PER_SEC;
        ListFree(&L);

        /* QuickSort: тот же массив */
        int *arr = malloc(sizeof(int) * n7);
        for (int i = 0; i < n7; i++) arr[i] = rand();
        clock_t t3 = clock();
        int Mq, Cq, depth;
        QuickSort1(n7, arr, &Mq, &Cq, &depth);
        clock_t t4 = clock();
        double tQ = 1000.0 * (double)(t4 - t3) / CLOCKS_PER_SEC;
        free(arr);

        const char *winner = (tD < tQ) ? "DigitalSort" : "QuickSort";
        printf("│ %6d │ %16.2f │ %16.2f │ %12s │\n", b, tD, tQ, winner);
    }
    printf("└────────┴──────────────────┴──────────────────┴──────────────┘\n");

    /* ===================================================================
     * === Доп. === DigitalSort: разбор Мф для int/int16/int32
     * Теория: M = const·n,  const = bytes·4
     * (на каждый байтовый проход элемент перекладывается дважды:
     *  список → корзина (2 пересылки), корзина → список (2 пересылки))
     * =================================================================== */
    printf("\n=== Доп. ===\n");
    printf("              DigitalSort: теория vs факт по M (для int = 4 байта, const = 16)\n");
    printf("┌──────┬──────────┬──────────┬──────────┬──────────┐\n");
    printf("│  N   │ Mt теор  │ Мф(убыв) │ Мф(случ) │ Мф(возр) │\n");
    printf("├──────┼──────────┼──────────┼──────────┼──────────┤\n");
    for (int s = 0; s < 5; s++) {
        int n = sizes[s];
        int Mth = 4 * 4 * n;
        printf("│ %4d │ %8d │ %8d │ %8d │ %8d │\n",
               n, Mth, Md[s], Mr[s], Mi[s]);
    }
    printf("└──────┴──────────┴──────────┴──────────┴──────────┘\n");

    /* int16/int32 разбор */
    printf("\n  Разбор для разных размеров типа (n=100):\n");
    printf("┌──────────┬──────────┬─────────────┬──────────┐\n");
    printf("│   Тип    │  bytes   │ Mt = b·4·n  │   Мф     │\n");
    printf("├──────────┼──────────┼─────────────┼──────────┤\n");
    int n_d = 100;

    ListInit(&L);
    for (int i = 0; i < n_d; i++)
        QueueEnqueue(&L, (int)(int16_t)(rand() % 65536 - 32768), NULL);
    ListDigitalSort16(&L, &M);
    printf("│ int16_t  │ %8d │ %11d │ %8d │\n", 2, 2 * 4 * n_d, M);
    ListFree(&L);

    ListInit(&L);
    for (int i = 0; i < n_d; i++)
        QueueEnqueue(&L, (int)(int32_t)(((unsigned)rand() << 16) ^ (unsigned)rand()), NULL);
    ListDigitalSort32(&L, &M);
    printf("│ int32_t  │ %8d │ %11d │ %8d │\n", 4, 4 * 4 * n_d, M);
    ListFree(&L);

    printf("└──────────┴──────────┴─────────────┴──────────┘\n");

    return 0;
}
