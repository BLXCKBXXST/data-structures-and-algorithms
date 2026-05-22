/*
 * 2.4 — Хеширование методом прямого связывания (chaining)
 * Тестовые данные: 52 неповторяющихся символа — латиница A–Z и a–z.
 * Ключ хеширования = ASCII-код символа.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "hash.h"

/* 52 уникальных символа: латиница A–Z и a–z */
static const char SYMBOLS[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
static const int  N = (int)sizeof(SYMBOLS) - 1;   /* без завершающего '\0' */

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
     *   BSearch: C ≈ 2·log2(52) ≈ 11
     *   Hash chain (m=53): средняя длина цепочки ≈ 1 → C ≈ 1-2
     * =================================================================== */
    printf("┌──────────────────────────────────────────────────────────────┐\n");
    printf("│  2.4.2 — Хеширование с цепочками (m=53, n=%2d)                │\n",  N);
    printf("└──────────────────────────────────────────────────────────────┘\n");

    HashChain H;
    HashChainInit(&H, 53);
    int collisions = 0;
    for (int i = 0; i < N; i++)
        HashChainInsert(&H, (int)SYMBOLS[i], &collisions);

    printf("Исходные символы: ");
    for (int i = 0; i < N; i++) printf("%c ", SYMBOLS[i]);
    printf("\n\nЗаполненная хеш-таблица (m=53):\n");
    PrintBuckets(&H);
    printf("\nФактическое количество коллизий Кф = %d\n", collisions);
    printf("Сравнение с BSearch: C(BSearch1) ≈ 2·log2(%d) ≈ %d\n",
           N, (int)(2.0 * log2((double)N) + 0.5));

    HashChainFree(&H);

    /* ===================================================================
     * 2.4.3 — Зависимость количества коллизий от размера хеш-таблицы
     * =================================================================== */
    printf("\n        2.4.3 — Зависимость количества коллизий от размера хеш-таблицы\n");
    printf("┌──────────────────┬──────────────────┬────────────────────┐\n");
    printf("│ Размер хеш-табл. │ Кол-во символов  │ Кол-во коллизий Кф │\n");
    printf("├──────────────────┼──────────────────┼────────────────────┤\n");
    /* размеры хеш-таблицы — нечётные числа от 11 до 101 с шагом 2 */
    for (int m = 11; m <= 101; m += 2) {
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
    printf("\n          2.4.4* — Поиск элемента (m=53)\n");
    HashChainInit(&H, 53);
    collisions = 0;
    for (int i = 0; i < N; i++)
        HashChainInsert(&H, (int)SYMBOLS[i], &collisions);

    printf("┌────────┬──────────────┬──────────────┬──────────────┐\n");
    printf("│ Символ │ Хеш (k mod m)│ Номер списка │   Позиция    │\n");
    printf("├────────┼──────────────┼──────────────┼──────────────┤\n");
    for (int i = 0; i < N; i++) {
        int b, pos;
        int found = HashChainSearch(&H, (int)SYMBOLS[i], &b, &pos);
        int h = (unsigned)SYMBOLS[i] % 53u;
        if (found)
            printf("│   %c    │ %12d │ %12d │ %12d │\n", SYMBOLS[i], h, b, pos);
        else
            printf("│   %c    │ %12d │ %12s │ %12s │\n", SYMBOLS[i], h, "—", "не найден");
    }
    printf("└────────┴──────────────┴──────────────┴──────────────┘\n");

    /* Поиск отсутствующего символа */
    int b, pos;
    int found = HashChainSearch(&H, (int)'?', &b, &pos);
    printf("\nПоиск '?' (отсутствует): %s\n", found ? "найден" : "не найден");

    HashChainFree(&H);
    return 0;
}
