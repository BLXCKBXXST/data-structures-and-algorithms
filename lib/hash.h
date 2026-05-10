#ifndef HASH_H
#define HASH_H

#include "list.h"

/* ─── Хеширование методом прямого связывания (chaining) ─── */
typedef struct HashChain {
    List *buckets;   /* массив списков размера m */
    int   m;
} HashChain;

void HashChainInit (HashChain *H, int m);
void HashChainFree (HashChain *H);

/* Вставка ключа.
 * Возвращает индекс корзины.
 * *collisions инкрементируется на 1, если корзина уже была непуста. */
int  HashChainInsert(HashChain *H, int key, int *collisions);

/* Поиск ключа. Возвращает 1 если найден, 0 если нет.
 * При находке *bucket — индекс корзины, *pos — позиция в цепочке (1-based). */
int  HashChainSearch(const HashChain *H, int key, int *bucket, int *pos);

/* ─── Хеширование методом открытой адресации ─── */
typedef enum { PROBE_LINEAR, PROBE_QUADRATIC } ProbeKind;

typedef struct HashOpen {
    int      *table;  /* -1 — пустой слот */
    int       m;
    ProbeKind probe;
} HashOpen;

void HashOpenInit (HashOpen *H, int m, ProbeKind p);
void HashOpenFree (HashOpen *H);

/* Вставка. Возвращает позицию или -1 если таблица полна.
 * *collisions инкрементируется на каждый занятый слот, через который пришлось пройти. */
int  HashOpenInsert(HashOpen *H, int key, int *collisions);

/* Поиск. Возвращает позицию или -1 если не найден. */
int  HashOpenSearch(const HashOpen *H, int key);

#endif /* HASH_H */
