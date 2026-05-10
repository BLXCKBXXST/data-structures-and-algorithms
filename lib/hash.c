#include <stdio.h>
#include <stdlib.h>
#include "hash.h"

/* ════════════════════════════════════════
 *  Прямое связывание (chaining)
 * ════════════════════════════════════════ */

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
    int b = ((unsigned)key) % (unsigned)H->m;
    if (H->buckets[b].size > 0 && collisions) (*collisions)++;
    QueueEnqueue(&H->buckets[b], key, NULL);
    return b;
}

int HashChainSearch(const HashChain *H, int key, int *bucket, int *pos) {
    int b = ((unsigned)key) % (unsigned)H->m;
    int p = 1;
    for (Node *n = H->buckets[b].head; n; n = n->next, p++) {
        if (n->data == key) {
            if (bucket) *bucket = b;
            if (pos)    *pos    = p;
            return 1;
        }
    }
    return 0;
}

/* ════════════════════════════════════════
 *  Открытая адресация (linear / quadratic probing)
 * ════════════════════════════════════════ */

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

static int probe_pos(const HashOpen *H, int h0, int i) {
    int step = (H->probe == PROBE_LINEAR) ? i : i * i;
    return (h0 + step) % H->m;
}

int HashOpenInsert(HashOpen *H, int key, int *collisions) {
    int h0 = ((unsigned)key) % (unsigned)H->m;
    for (int i = 0; i < H->m; i++) {
        int pos = probe_pos(H, h0, i);
        if (H->table[pos] == -1) {
            H->table[pos] = key;
            return pos;
        }
        if (collisions) (*collisions)++;
    }
    return -1;  /* таблица полна */
}

int HashOpenSearch(const HashOpen *H, int key) {
    int h0 = ((unsigned)key) % (unsigned)H->m;
    for (int i = 0; i < H->m; i++) {
        int pos = probe_pos(H, h0, i);
        if (H->table[pos] == -1) return -1;
        if (H->table[pos] == key) return pos;
    }
    return -1;
}
