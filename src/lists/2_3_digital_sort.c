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

static void ListFillInc16(List *L, int n) {
    ListFree(L);
    int M = 0;
    /* Значения 0..n-1 укладываются в int16 при n ≤ 32768 */
    for (int i = 0; i < n; i++)
        QueueEnqueue(L, (int)(int16_t)i, &M);
}

static void ListFillDec16(List *L, int n) {
    ListFree(L);
    int M = 0;
    for (int i = n - 1; i >= 0; i--)
        QueueEnqueue(L, (int)(int16_t)i, &M);
}

static void ListFillRand32(List *L, int n) {
    ListFree(L);
    int M = 0;
    for (int i = 0; i < n; i++) {
        int32_t v = (int32_t)(((unsigned)rand() << 16) ^ (unsigned)rand());
        QueueEnqueue(L, (int)v, &M);
    }
}

/* ╔══════════════════════════════════════════════════════════════╗
 * ║  НАЧАЛО: Цифровая сортировка по N байтам (LSD radix, база 256) ║
 * ║  Параметризованный in-place вариант для 2.3.7* (timing).       ║
 * ║  Узлы перевязываются через next, без malloc/free.              ║
 * ║  M = bytes·n.                                                  ║
 * ╚══════════════════════════════════════════════════════════════╝ */
static void ListDigitalSortBytes(List *L, int bytes, int *M) {
    *M = 0;
    if (bytes <= 0) return;
    for (int b = 0; b < bytes; b++) {
        List buckets[256];
        for (int i = 0; i < 256; i++) ListInit(&buckets[i]);

        /* ── Раздача узлов по корзинам ── */
        Node *p = L->head;
        while (p) {
            Node *next = p->next;
            int d = ((unsigned int)p->data >> (b * 8)) & 0xFF;
            p->next = NULL;
            if (buckets[d].tail) buckets[d].tail->next = p;
            else                 buckets[d].head       = p;
            buckets[d].tail = p;
            buckets[d].size++;
            (*M)++;
            p = next;
        }

        /* ── Сборка результата: одна склейка на непустую корзину ── */
        L->head = L->tail = NULL;
        L->size = 0;
        for (int i = 0; i < 256; i++) {
            if (!buckets[i].head) continue;
            if (L->tail) L->tail->next = buckets[i].head;
            else         L->head       = buckets[i].head;
            L->tail = buckets[i].tail;
            L->size += buckets[i].size;
        }
        if (L->tail) L->tail->next = NULL;
    }
}
/* ╔══════════════════════════════════════════════════════════════╗
 * ║   КОНЕЦ: ListDigitalSortBytes                                  ║
 * ╚══════════════════════════════════════════════════════════════╝ */

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

    /* Накапливаем Mф для int16 (2 байта) — те же размеры, те же три типа входа */
    int Md16[5], Mr16[5], Mi16[5];
    for (int s = 0; s < 5; s++) {
        int n = sizes[s];
        ListInit(&L); ListFillDec16(&L, n);
        ListDigitalSort16(&L, &Md16[s]); ListFree(&L);
        ListInit(&L); ListFillRand16(&L, n);
        ListDigitalSort16(&L, &Mr16[s]); ListFree(&L);
        ListInit(&L); ListFillInc16(&L, n);
        ListDigitalSort16(&L, &Mi16[s]); ListFree(&L);
    }

    /* ===================================================================
     * Основная таблица ТЗ: Трудоемкость цифровой сортировки DigitalSort
     * Теория (одинаково для всех случаев входа): M = bytes · n,  C = 0
     *   для int   (4 байта): Mt = 4·n
     *   для int16 (2 байта): Mt = 2·n
     * (одна перевязка next на узел при раздаче в корзину;
     *  склейка непустых корзин при сборке — служебная и в счёт
     *  пересылок элементов не входит)
     * =================================================================== */
    printf("              Трудоемкость DigitalSort: теория и факт для int + int16\n");
    printf("┌──────┬───────────────────────────────────┬───────────────────────────────────┐\n");
    printf("│      │           int (4 байта)           │          int16 (2 байта)          │\n");
    printf("│      ├────────┬────────┬────────┬────────┼────────┬────────┬────────┬────────┤\n");
    printf("│  N   │  Mt=4n │ Мф(уб) │ Мф(сл) │ Мф(вз) │  Mt=2n │ Мф(уб) │ Мф(сл) │ Мф(вз) │\n");
    printf("├──────┼────────┼────────┼────────┼────────┼────────┼────────┼────────┼────────┤\n");
    for (int s = 0; s < 5; s++) {
        int n       = sizes[s];
        int Mth_int = 4 * n;
        int Mth_16  = 2 * n;
        printf("│ %4d │ %6d │ %6d │ %6d │ %6d │ %6d │ %6d │ %6d │ %6d │\n",
               n,
               Mth_int, Md[s],   Mr[s],   Mi[s],
               Mth_16,  Md16[s], Mr16[s], Mi16[s]);
    }
    printf("└──────┴────────┴────────┴────────┴────────┴────────┴────────┴────────┴────────┘\n");

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
     * Теория: M = bytes · n
     * (на каждый байтовый проход узел перевязывается один раз через
     *  next-указатель при раздаче в корзину; сборка склеивает цепочки
     *  непустых корзин — служебная фаза, в M пересылок элементов
     *  не учитывается)
     * =================================================================== */
    printf("\n=== Доп. ===\n");
    printf("              DigitalSort: теория vs факт по M (int = 4n, int16 = 2n)\n");
    printf("┌──────┬───────────────────────────────────┬───────────────────────────────────┐\n");
    printf("│      │           int (4 байта)           │          int16 (2 байта)          │\n");
    printf("│      ├────────┬────────┬────────┬────────┼────────┬────────┬────────┬────────┤\n");
    printf("│  N   │  Mt=4n │ Мф(уб) │ Мф(сл) │ Мф(вз) │  Mt=2n │ Мф(уб) │ Мф(сл) │ Мф(вз) │\n");
    printf("├──────┼────────┼────────┼────────┼────────┼────────┼────────┼────────┼────────┤\n");
    for (int s = 0; s < 5; s++) {
        int n       = sizes[s];
        int Mth_int = 4 * n;
        int Mth_16  = 2 * n;
        printf("│ %4d │ %6d │ %6d │ %6d │ %6d │ %6d │ %6d │ %6d │ %6d │\n",
               n,
               Mth_int, Md[s],   Mr[s],   Mi[s],
               Mth_16,  Md16[s], Mr16[s], Mi16[s]);
    }
    printf("└──────┴────────┴────────┴────────┴────────┴────────┴────────┴────────┴────────┘\n");

    /* int16/int32 разбор */
    printf("\n  Разбор для разных размеров типа (n=100):\n");
    printf("┌──────────┬──────────┬─────────────┬──────────┐\n");
    printf("│   Тип    │  bytes   │  Mt = b·n   │   Мф     │\n");
    printf("├──────────┼──────────┼─────────────┼──────────┤\n");
    int n_d = 100;

    ListInit(&L);
    for (int i = 0; i < n_d; i++)
        QueueEnqueue(&L, (int)(int16_t)(rand() % 65536 - 32768), NULL);
    ListDigitalSort16(&L, &M);
    printf("│ int16_t  │ %8d │ %11d │ %8d │\n", 2, 2 * n_d, M);
    ListFree(&L);

    ListInit(&L);
    for (int i = 0; i < n_d; i++)
        QueueEnqueue(&L, (int)(int32_t)(((unsigned)rand() << 16) ^ (unsigned)rand()), NULL);
    ListDigitalSort32(&L, &M);
    printf("│ int32_t  │ %8d │ %11d │ %8d │\n", 4, 4 * n_d, M);
    ListFree(&L);

    printf("└──────────┴──────────┴─────────────┴──────────┘\n");

    return 0;
}
