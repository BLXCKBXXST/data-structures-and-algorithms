/*
 * 2.5 — Хеширование методом открытой адресации
 * Тестовые данные: 52 неповторяющихся символа — латиница A–Z и a–z.
 * Линейные пробы: h(k,i) = (h0 + i)   mod m
 * Квадратичные:   h(k,i) = (h0 + i²)  mod m
 */
#include <stdio.h>
#include <stdlib.h>
#include "hash.h"

/* 52 уникальных символа: латиница A–Z и a–z */
static const char SYMBOLS[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
static const int  N = (int)sizeof(SYMBOLS) - 1;   /* без завершающего '\0' */

/* Вывод хеш-таблицы вертикально, ячейка за ячейкой: при больших m
 * горизонтальная таблица 0..m-1 не умещается по ширине экрана. */
static void PrintHashTable(const char *title, const HashOpen *H) {
    printf("%s\n", title);
    for (int i = 0; i < H->m; i++) {
        if (H->table[i] == -1) printf("  ячейка[%3d]: .\n", i);
        else                   printf("  ячейка[%3d]: %c\n", i, (char)H->table[i]);
    }
}

int main(void) {
    /* ===================================================================
     * 2.5.2 — Реализация хеширования методом открытой адресации (m=107)
     * Линейные и квадратичные пробы; вывод обеих заполненных таблиц.
     * =================================================================== */
    printf("┌──────────────────────────────────────────────────────────────┐\n");
    printf("│  2.5.2 — Открытая адресация (m=107, n=%2d)                    │\n", N);
    printf("└──────────────────────────────────────────────────────────────┘\n");

    HashOpen Hl, Hq;
    HashOpenInit(&Hl, 107, PROBE_LINEAR);
    HashOpenInit(&Hq, 107, PROBE_QUADRATIC);

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
     * для нечётных размеров таблицы от 11 до 101 (шаг 2)
     * =================================================================== */
    printf("\n        2.5.3 — Сравнение количества коллизий\n");
    printf("┌──────────────┬──────────────────┬──────────────────┬──────────────────┐\n");
    printf("│ Размер хеш-  │ Кол-во исходных  │ Линейные пробы   │ Квадрат. пробы   │\n");
    printf("│   таблицы    │     символов     │     Кф           │     Кф           │\n");
    printf("├──────────────┼──────────────────┼──────────────────┼──────────────────┤\n");
    /* размеры хеш-таблицы — нечётные числа от 11 до 101 с шагом 2 */
    for (int m = 11; m <= 101; m += 2) {
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
    printf("\n              2.5.4* — Поиск элемента (m=107)\n");

    HashOpenInit(&Hl, 107, PROBE_LINEAR);
    HashOpenInit(&Hq, 107, PROBE_QUADRATIC);
    Kl = 0; Kq = 0;
    for (int i = 0; i < N; i++) HashOpenInsert(&Hl, (int)SYMBOLS[i], &Kl);
    for (int i = 0; i < N; i++) HashOpenInsert(&Hq, (int)SYMBOLS[i], &Kq);

    printf("┌────────┬──────────────┬───────────────┬────────────────┐\n");
    printf("│ Символ │ Хеш (k mod m)│ Лин. позиция  │ Квадр. позиция │\n");
    printf("├────────┼──────────────┼───────────────┼────────────────┤\n");
    for (int i = 0; i < N; i++) {
        int posL = HashOpenSearch(&Hl, (int)SYMBOLS[i]);
        int posQ = HashOpenSearch(&Hq, (int)SYMBOLS[i]);
        int h = (unsigned)SYMBOLS[i] % 107u;
        printf("│   %c    │ %12d │ %13d │ %14d │\n", SYMBOLS[i], h, posL, posQ);
    }
    printf("└────────┴──────────────┴───────────────┴────────────────┘\n");

    /* Поиск отсутствующего */
    int posL = HashOpenSearch(&Hl, (int)'?');
    int posQ = HashOpenSearch(&Hq, (int)'?');
    printf("\nПоиск '?' (отсутствует): линейные → %d, квадратичные → %d\n",
           posL, posQ);

    HashOpenFree(&Hl);
    HashOpenFree(&Hq);
    return 0;
}
