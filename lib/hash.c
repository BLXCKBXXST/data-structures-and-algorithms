#include <stdio.h>
#include <stdlib.h>
#include "hash.h"

/* ╔══════════════════════════════════════════════════════════════╗
 * ║   НАЧАЛО: Метод прямого связывания / chaining               ║
 * ║            (методичка 15.2)                                  ║
 * ╚══════════════════════════════════════════════════════════════╝ */

/*  Хэш-таблица — массив списков (m списков). Коллизии разрешаются
 *  включением всех элементов с одинаковым H(k) в один список.
 *
 *  Включение элемента (методичка 15.2):
 *      1) вычисление i := H(k)
 *      2) добавление элемента k в конец i-того списка
 *
 *  Поиск элемента:
 *      1) вычисление i := H(k)
 *      2) последовательный просмотр i-того списка
 *
 *  В качестве H используем H(k) = k mod m  (метод деления, при простом m).
 *  Средняя длина списка n/m, средняя трудоёмкость C_ср = n/(2m).
 */

void HashChainInit(HashChain *H, int m) {
    H->m = m;
    H->buckets = malloc(sizeof(List) * m);
    if (!H->buckets) { fprintf(stderr, "malloc failed\n"); exit(1); }
    for (int i = 0; i < m; i++) ListInit(&H->buckets[i]);
}

void HashChainFree(HashChain *H) {
    for (int i = 0; i < H->m; i++) ListFree(&H->buckets[i]);
    free(H->buckets);
    H->buckets = NULL;
    H->m = 0;
}

int HashChainInsert(HashChain *H, int key, int *collisions) {
    int i = ((unsigned)key) % (unsigned)H->m;     /* i := H(k) = k mod m */
    if (H->buckets[i].size > 0 && collisions) (*collisions)++;
    QueueEnqueue(&H->buckets[i], key, NULL);       /* добавить в конец i-того списка */
    return i;
}

int HashChainSearch(const HashChain *H, int key, int *bucket, int *pos) {
    int i = ((unsigned)key) % (unsigned)H->m;     /* i := H(k) */
    int p = 1;
    for (Node *n = H->buckets[i].head; n; n = n->next, p++) {
        if (n->data == key) {                      /* последовательный просмотр */
            if (bucket) *bucket = i;
            if (pos)    *pos    = p;
            return 1;
        }
    }
    return 0;
}

/* ╔══════════════════════════════════════════════════════════════╗
 * ║   КОНЕЦ: Метод прямого связывания                            ║
 * ╚══════════════════════════════════════════════════════════════╝ */


/* ╔══════════════════════════════════════════════════════════════╗
 * ║   НАЧАЛО: Метод открытой адресации (методичка 15.3)          ║
 * ╚══════════════════════════════════════════════════════════════╝ */

/*  Алгоритм на псевдокоде (методичка 15.3, поиск и вставка):
 *      h := x mod m
 *      d := 1
 *      DO
 *          IF (a_h = x)  элемент найден  OD  FI
 *          IF (a_h = 0)  ячейка пуста, a_h := x  OD  FI
 *          IF (d ≥ m)    переполнение таблицы  OD  FI
 *          h := h + d
 *          IF (h ≥ m)    h := h - m  FI
 *          d := d + 2
 *      OD
 *
 *  d-последовательность 1, 3, 5, ... даёт квадратичные пробы:
 *      h_0 = h(x),  h_{i+1} = h_i + d_i,  d_{i+1} = d_i + 2
 *  что эквивалентно h_i = h_0 + i² (mod m).
 *
 *  В этой реализации «пустая» ячейка маркирована -1 (а не 0, как в
 *  методичке) — чтобы ключ 0 можно было хранить наравне с прочими.
 *
 *  Для линейных проб шаг d фиксирован = 1 (g(i) = i, см. методичку).
 */

void HashOpenInit(HashOpen *H, int m, ProbeKind p) {
    H->m = m;
    H->probe = p;
    H->table = malloc(sizeof(int) * m);
    if (!H->table) { fprintf(stderr, "malloc failed\n"); exit(1); }
    for (int i = 0; i < m; i++) H->table[i] = -1;
}

void HashOpenFree(HashOpen *H) {
    free(H->table);
    H->table = NULL;
    H->m = 0;
}

int HashOpenInsert(HashOpen *H, int key, int *collisions) {
    int h = ((unsigned)key) % (unsigned)H->m;     /* h := x mod m */
    int d = 1;                                     /* d := 1 */
    int probes = 0;
    while (1) {                                    /* DO */
        if (H->table[h] == key) return h;          /* IF (a_h = x) элемент найден OD FI */
        if (H->table[h] == -1) {                   /* IF (a_h = 0) ячейка пуста */
            H->table[h] = key;                      /*   a_h := x */
            return h;                               /*   OD */
        }
        if (collisions) (*collisions)++;
        if (probes >= H->m) return -1;             /* IF (d ≥ m) переполнение OD FI */
        probes++;
        if (H->probe == PROBE_LINEAR) {
            h = h + 1;                              /* линейные: d ≡ 1 */
        } else {
            h = h + d;                              /* h := h + d */
            d = d + 2;                              /* d := d + 2 */
        }
        if (h >= H->m) h = h - H->m;               /* IF (h ≥ m) h := h - m FI */
    }
}

int HashOpenSearch(const HashOpen *H, int key) {
    int h = ((unsigned)key) % (unsigned)H->m;     /* h := x mod m */
    int d = 1;
    int probes = 0;
    while (1) {
        if (H->table[h] == key) return h;          /* IF (a_h = x) найден */
        if (H->table[h] == -1) return -1;          /* IF (a_h = 0) пуста → не найден */
        if (probes >= H->m) return -1;
        probes++;
        if (H->probe == PROBE_LINEAR) {
            h = h + 1;
        } else {
            h = h + d;
            d = d + 2;
        }
        if (h >= H->m) h = h - H->m;
    }
}

/* ╔══════════════════════════════════════════════════════════════╗
 * ║   КОНЕЦ: Метод открытой адресации                            ║
 * ╚══════════════════════════════════════════════════════════════╝ */
