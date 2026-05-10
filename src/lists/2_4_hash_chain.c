/*
 * 2.4 — Хеширование методом прямого связывания (chaining)
 * Тестовые данные: 12 неповторяющихся символов из "IvanovAleksePetr".
 * Ключ хеширования = ASCII-код символа.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hash.h"

/* 12 уникальных символов */
static const char SYMBOLS[] = {'I','v','a','n','o','A','l','e','k','s','P','t'};
static const int  N = sizeof(SYMBOLS) / sizeof(SYMBOLS[0]);

/* 10 простых чисел в диапазоне 11..101 */
static const int PRIMES[] = {11, 17, 23, 31, 41, 53, 61, 71, 83, 97};
static const int NP = sizeof(PRIMES) / sizeof(PRIMES[0]);

static void PrintBuckets(const HashChain *H) {
    for (int i = 0; i < H->m; i++) {
        printf("  bucket[%2d]: ", i);
        for (Node *n = H->buckets[i].head; n; n = n->next)
            printf("%c ", (char)n->data);
        printf("\n");
    }
}

int main(void) {
    /* ===================================================================
     * 2.4.2 — Реализация хеширования с цепочками
     * Размер хеш-таблицы выбран так, чтобы поиск был быстрее BSearch:
     *   BSearch: C ≈ 2·log2(12) ≈ 7
     *   Hash chain (m=13): средняя длина цепочки ≈ 1 → C ≈ 1-2
     * =================================================================== */
    printf("┌──────────────────────────────────────────────────────────────┐\n");
    printf("│  2.4.2 — Хеширование с цепочками (m=13, n=%2d)                │\n",  N);
    printf("└──────────────────────────────────────────────────────────────┘\n");

    HashChain H;
    HashChainInit(&H, 13);
    int collisions = 0;
    for (int i = 0; i < N; i++)
        HashChainInsert(&H, (int)SYMBOLS[i], &collisions);

    printf("Исходные символы: ");
    for (int i = 0; i < N; i++) printf("%c ", SYMBOLS[i]);
    printf("\n\nЗаполненная хеш-таблица (m=13):\n");
    PrintBuckets(&H);
    printf("\nФактическое количество коллизий Кф = %d\n", collisions);
    printf("Сравнение с BSearch: C(BSearch1) ≈ 2·log2(%d) ≈ %d\n", N, 2 * 4);

    HashChainFree(&H);

    /* ===================================================================
     * 2.4.3 — Зависимость количества коллизий от размера хеш-таблицы
     * =================================================================== */
    printf("\n        2.4.3 — Зависимость количества коллизий от размера хеш-таблицы\n");
    printf("┌──────────────────┬──────────────────┬────────────────────┐\n");
    printf("│ Размер хеш-табл. │ Кол-во символов  │ Кол-во коллизий Кф │\n");
    printf("├──────────────────┼──────────────────┼────────────────────┤\n");
    for (int p = 0; p < NP; p++) {
        int m = PRIMES[p];
        HashChainInit(&H, m);
        int cls = 0;
        for (int i = 0; i < N; i++)
            HashChainInsert(&H, (int)SYMBOLS[i], &cls);
        printf("│ %16d │ %16d │ %18d │\n", m, N, cls);
        HashChainFree(&H);
    }
    printf("└──────────────────┴──────────────────┴────────────────────┘\n");

    /* ===================================================================
     * 2.4.4* — Поиск элемента с заданным ключом
     * =================================================================== */
    printf("\n          2.4.4* — Поиск элемента (m=13)\n");
    HashChainInit(&H, 13);
    collisions = 0;
    for (int i = 0; i < N; i++)
        HashChainInsert(&H, (int)SYMBOLS[i], &collisions);

    printf("┌────────┬──────────────┬──────────────┬──────────────┐\n");
    printf("│ Символ │ Хеш (k mod m)│ Номер списка │   Позиция    │\n");
    printf("├────────┼──────────────┼──────────────┼──────────────┤\n");
    for (int i = 0; i < N; i++) {
        int b, pos;
        int found = HashChainSearch(&H, (int)SYMBOLS[i], &b, &pos);
        int h = (unsigned)SYMBOLS[i] % 13u;
        if (found)
            printf("│   %c    │ %12d │ %12d │ %12d │\n", SYMBOLS[i], h, b, pos);
        else
            printf("│   %c    │ %12d │ %12s │ %12s │\n", SYMBOLS[i], h, "—", "не найден");
    }
    printf("└────────┴──────────────┴──────────────┴──────────────┘\n");

    /* Поиск отсутствующего символа */
    int b, pos;
    int found = HashChainSearch(&H, (int)'Z', &b, &pos);
    printf("\nПоиск 'Z' (отсутствует): %s\n", found ? "найден" : "не найден");

    HashChainFree(&H);
    return 0;
}
