/*
 * 2.5 — Хеширование методом открытой адресации
 * Тестовые данные: 12 неповторяющихся символов из "IvanovAleksePetr".
 * Линейные пробы: h(k,i) = (h0 + i)   mod m
 * Квадратичные:   h(k,i) = (h0 + i²)  mod m
 */
#include <stdio.h>
#include <stdlib.h>
#include "hash.h"

static const char SYMBOLS[] = {'I','v','a','n','o','A','l','e','k','s','P','t'};
static const int  N = sizeof(SYMBOLS) / sizeof(SYMBOLS[0]);

static const int PRIMES[] = {11, 17, 23, 31, 41, 53, 61, 71, 83, 97};
static const int NP = sizeof(PRIMES) / sizeof(PRIMES[0]);

static void PrintHashTable(const char *title, const HashOpen *H) {
    printf("%s\n", title);
    printf("┌──────────────┬");
    for (int i = 0; i < H->m; i++) printf("─────┬");
    printf("\b┐\n");
    printf("│ Номер ячейки │");
    for (int i = 0; i < H->m; i++) printf(" %3d │", i);
    printf("\n├──────────────┼");
    for (int i = 0; i < H->m; i++) printf("─────┼");
    printf("\b┤\n");
    printf("│   Символ     │");
    for (int i = 0; i < H->m; i++) {
        if (H->table[i] == -1) printf("  .  │");
        else                   printf("  %c  │", (char)H->table[i]);
    }
    printf("\n└──────────────┴");
    for (int i = 0; i < H->m; i++) printf("─────┴");
    printf("\b┘\n");
}

int main(void) {
    /* ===================================================================
     * 2.5.2 — Реализация хеширования методом открытой адресации (m=11)
     * Линейные и квадратичные пробы; вывод обеих заполненных таблиц.
     * =================================================================== */
    printf("┌──────────────────────────────────────────────────────────────┐\n");
    printf("│  2.5.2 — Открытая адресация (m=11, n=%2d)                     │\n", N);
    printf("└──────────────────────────────────────────────────────────────┘\n");

    HashOpen Hl, Hq;
    HashOpenInit(&Hl, 11, PROBE_LINEAR);
    HashOpenInit(&Hq, 11, PROBE_QUADRATIC);

    int Kl = 0, Kq = 0;
    for (int i = 0; i < N; i++) HashOpenInsert(&Hl, (int)SYMBOLS[i], &Kl);
    for (int i = 0; i < N; i++) HashOpenInsert(&Hq, (int)SYMBOLS[i], &Kq);

    printf("Исходные символы: ");
    for (int i = 0; i < N; i++) printf("%c ", SYMBOLS[i]);
    printf("\n\n");

    PrintHashTable("Линейные пробы:", &Hl);
    printf("Кф (линейные)    = %d\n\n", Kl);

    PrintHashTable("Квадратичные пробы:", &Hq);
    printf("Кф (квадратичные) = %d\n", Kq);

    HashOpenFree(&Hl);
    HashOpenFree(&Hq);

    /* ===================================================================
     * 2.5.3 — Сравнение коллизий: линейные vs квадратичные пробы
     * для 10 простых чисел в диапазоне 11..101
     * =================================================================== */
    printf("\n        2.5.3 — Сравнение количества коллизий\n");
    printf("┌──────────────┬──────────────────┬──────────────────┬──────────────────┐\n");
    printf("│ Размер хеш-  │ Кол-во исходных  │ Линейные пробы   │ Квадрат. пробы   │\n");
    printf("│   таблицы    │     символов     │     Кф           │     Кф           │\n");
    printf("├──────────────┼──────────────────┼──────────────────┼──────────────────┤\n");
    for (int p = 0; p < NP; p++) {
        int m = PRIMES[p];
        HashOpenInit(&Hl, m, PROBE_LINEAR);
        HashOpenInit(&Hq, m, PROBE_QUADRATIC);
        int kl = 0, kq = 0;
        for (int i = 0; i < N; i++) HashOpenInsert(&Hl, (int)SYMBOLS[i], &kl);
        for (int i = 0; i < N; i++) HashOpenInsert(&Hq, (int)SYMBOLS[i], &kq);
        printf("│ %12d │ %16d │ %16d │ %16d │\n", m, N, kl, kq);
        HashOpenFree(&Hl);
        HashOpenFree(&Hq);
    }
    printf("└──────────────┴──────────────────┴──────────────────┴──────────────────┘\n");

    /* ===================================================================
     * 2.5.4* — Поиск элемента с заданным ключом для обеих стратегий
     * =================================================================== */
    printf("\n              2.5.4* — Поиск элемента (m=11)\n");

    HashOpenInit(&Hl, 11, PROBE_LINEAR);
    HashOpenInit(&Hq, 11, PROBE_QUADRATIC);
    Kl = 0; Kq = 0;
    for (int i = 0; i < N; i++) HashOpenInsert(&Hl, (int)SYMBOLS[i], &Kl);
    for (int i = 0; i < N; i++) HashOpenInsert(&Hq, (int)SYMBOLS[i], &Kq);

    printf("┌────────┬──────────────┬───────────────┬────────────────┐\n");
    printf("│ Символ │ Хеш (k mod m)│ Лин. позиция  │ Квадр. позиция │\n");
    printf("├────────┼──────────────┼───────────────┼────────────────┤\n");
    for (int i = 0; i < N; i++) {
        int posL = HashOpenSearch(&Hl, (int)SYMBOLS[i]);
        int posQ = HashOpenSearch(&Hq, (int)SYMBOLS[i]);
        int h = (unsigned)SYMBOLS[i] % 11u;
        printf("│   %c    │ %12d │ %13d │ %14d │\n", SYMBOLS[i], h, posL, posQ);
    }
    printf("└────────┴──────────────┴───────────────┴────────────────┘\n");

    /* Поиск отсутствующего */
    int posL = HashOpenSearch(&Hl, (int)'Z');
    int posQ = HashOpenSearch(&Hq, (int)'Z');
    printf("\nПоиск 'Z' (отсутствует): линейные → %d, квадратичные → %d\n",
           posL, posQ);

    HashOpenFree(&Hl);
    HashOpenFree(&Hq);
    return 0;
}
