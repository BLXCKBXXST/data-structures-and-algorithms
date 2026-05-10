#ifndef LIST_H
#define LIST_H

#include <stdint.h>

/* ─── Узел односвязного списка ─── */
typedef struct Node {
    int          data;
    struct Node *next;
} Node;

/* ─── Список / Стек / Очередь ─── */
typedef struct {
    Node *head;   /* вершина стека / голова очереди  */
    Node *tail;   /* NULL для стека / хвост очереди  */
    int   size;
} List;

/* ── Инициализация ── */
void ListInit(List *L);

/* ── Стек ── */
void StackPush(List *S, int val, int *M);
int  StackPop (List *S, int *M);
int  StackIsEmpty(const List *S);

/* ── Очередь ── */
void QueueEnqueue(List *Q, int val, int *M);
int  QueueDequeue(List *Q, int *M);
int  QueueIsEmpty(const List *Q);

/* ── Общие операции со списком ── */
void      ListPrint    (const List *L);
void      ListPrintRev (const List *L);        /* 5*: рекурсивно в обратном порядке */
long long ListChecksum (const List *L);
int       ListSeriesCount(const List *L);
void      ListFree(List *L);                   /* 4*: удалить все элементы */

/* ── Заполнение стека ── */
void StackFillInc (List *S, int n, int *M);
void StackFillDec (List *S, int n, int *M);
void StackFillRand(List *S, int n, int *M);

/* ── Заполнение очереди ── */
void QueueFillInc (List *Q, int n, int *M);
void QueueFillDec (List *Q, int n, int *M);
void QueueFillRand(List *Q, int n, int *M);

/* ── Заполнение списка (для сортировок) ── */
void ListFillInc (List *L, int n);
void ListFillDec (List *L, int n);
void ListFillRand(List *L, int n);

/* ── MergeSort на списке ── */
void ListMergeSort(List *L, int *M, int *C, int *series);

/* ── DigitalSort (LSD Radix Sort) на списке ── */
void ListDigitalSort   (List *L, int *M);   /* int, по возрастанию  */
void ListDigitalSortDes(List *L, int *M);   /* int, по убыванию     */
void ListDigitalSort16 (List *L, int *M);   /* int16_t, по возрастанию */
void ListDigitalSort16D(List *L, int *M);   /* int16_t, по убыванию    */
void ListDigitalSort32 (List *L, int *M);   /* int32_t, по возрастанию */
void ListDigitalSort32D(List *L, int *M);   /* int32_t, по убыванию    */

/* ─── Список строк (для DigitalSort фамилий, 2.3.6*) ─── */
typedef struct StrNode {
    char            *data;
    struct StrNode  *next;
} StrNode;

typedef struct {
    StrNode *head;
    StrNode *tail;
    int      size;
} StrList;

void StrListInit  (StrList *L);
void StrListFree  (StrList *L);
void StrListAppend(StrList *L, const char *s);   /* enqueue (копия строки) */
void StrListPrint (const StrList *L);

/* DigitalSort строк (LSD по байтам справа налево):
 *   asc=1 — по возрастанию, asc=0 — по убыванию
 *   все строки приводятся к фиксированной длине = max длине + дополняются 0 справа */
void StrListDigitalSort(StrList *L, int asc, int *M);

#endif /* LIST_H */
