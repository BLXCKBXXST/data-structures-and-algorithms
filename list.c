#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "list.h"

/* ════════════════════════════════════════
 *  Вспомогательные функции
 * ════════════════════════════════════════ */

void ListInit(List *L) {
    L->head = L->tail = NULL;
    L->size = 0;
}

static Node *NewNode(int val) {
    Node *n = malloc(sizeof(Node));
    if (!n) { fprintf(stderr, "malloc failed\n"); exit(1); }
    n->data = val;
    n->next = NULL;
    return n;
}

/* ════════════════════════════════════════
 *  Стек (LIFO): push / pop на head
 * ════════════════════════════════════════ */

void StackPush(List *S, int val, int *M) {
    Node *n  = NewNode(val);
    n->next  = S->head;
    S->head  = n;
    S->size++;
    if (M) (*M)++;
}

int StackPop(List *S, int *M) {
    if (!S->head) { fprintf(stderr, "StackPop: empty\n"); exit(1); }
    Node *n   = S->head;
    int   val = n->data;
    S->head   = n->next;
    free(n);
    S->size--;
    if (M) (*M)++;
    return val;
}

int StackIsEmpty(const List *S) { return S->head == NULL; }

/* ════════════════════════════════════════
 *  Очередь (FIFO): enqueue на tail, dequeue с head
 * ════════════════════════════════════════ */

void QueueEnqueue(List *Q, int val, int *M) {
    Node *n = NewNode(val);
    if (Q->tail) Q->tail->next = n;
    else         Q->head       = n;
    Q->tail = n;
    Q->size++;
    if (M) (*M)++;
}

int QueueDequeue(List *Q, int *M) {
    if (!Q->head) { fprintf(stderr, "QueueDequeue: empty\n"); exit(1); }
    Node *n   = Q->head;
    int   val = n->data;
    Q->head   = n->next;
    if (!Q->head) Q->tail = NULL;
    free(n);
    Q->size--;
    if (M) (*M)++;
    return val;
}

int QueueIsEmpty(const List *Q) { return Q->head == NULL; }

/* ════════════════════════════════════════
 *  Общие операции со списком
 * ════════════════════════════════════════ */

void ListPrint(const List *L) {
    for (Node *p = L->head; p; p = p->next)
        printf("%d ", p->data);
    printf("\n");
}

static void PrintRevRec(const Node *p) {
    if (!p) return;
    PrintRevRec(p->next);
    printf("%d ", p->data);
}

/* 5*: рекурсивная печать в обратном порядке */
void ListPrintRev(const List *L) {
    PrintRevRec(L->head);
    printf("\n");
}

long long ListChecksum(const List *L) {
    long long s = 0;
    for (Node *p = L->head; p; p = p->next)
        s += p->data;
    return s;
}

int ListSeriesCount(const List *L) {
    if (!L->head) return 0;
    int count = 1;
    for (Node *p = L->head; p->next; p = p->next)
        if (p->next->data < p->data) count++;
    return count;
}

/* 4*: удалить все элементы */
void ListFree(List *L) {
    Node *p = L->head;
    while (p) {
        Node *t = p->next;
        free(p);
        p = t;
    }
    L->head = L->tail = NULL;
    L->size = 0;
}

/* ════════════════════════════════════════
 *  Заполнение стека
 * ════════════════════════════════════════ */

void StackFillInc(List *S, int n, int *M) {
    for (int i = n - 1; i >= 0; i--)
        StackPush(S, i, M);
}

void StackFillDec(List *S, int n, int *M) {
    for (int i = 0; i < n; i++)
        StackPush(S, i, M);
}

void StackFillRand(List *S, int n, int *M) {
    for (int i = 0; i < n; i++)
        StackPush(S, rand() % (n * n), M);
}

/* ════════════════════════════════════════
 *  Заполнение очереди
 * ════════════════════════════════════════ */

void QueueFillInc(List *Q, int n, int *M) {
    for (int i = 0; i < n; i++)
        QueueEnqueue(Q, i, M);
}

void QueueFillDec(List *Q, int n, int *M) {
    for (int i = n - 1; i >= 0; i--)
        QueueEnqueue(Q, i, M);
}

void QueueFillRand(List *Q, int n, int *M) {
    for (int i = 0; i < n; i++)
        QueueEnqueue(Q, rand() % (n * n), M);
}

/* ════════════════════════════════════════
 *  Заполнение списка (для сортировок)
 * ════════════════════════════════════════ */

void ListFillInc(List *L, int n) {
    ListFree(L);
    int M = 0;
    for (int i = 0; i < n; i++)
        QueueEnqueue(L, i, &M);
}

void ListFillDec(List *L, int n) {
    ListFree(L);
    int M = 0;
    for (int i = n - 1; i >= 0; i--)
        QueueEnqueue(L, i, &M);
}

void ListFillRand(List *L, int n) {
    ListFree(L);
    int M = 0;
    for (int i = 0; i < n; i++)
        QueueEnqueue(L, rand() % (n * n), &M);
}

/* ════════════════════════════════════════
 *  MergeSort (прямое слияние)
 *
 *  M = перемещения (enqueue/dequeue из стека/очереди)
 *  C = сравнения при слиянии серий
 *  Теория: M = n*log2(n),  C = n*log2(n)
 * ════════════════════════════════════════ */

static void Split(List *src, List *A, List *B) {
    ListInit(A); ListInit(B);
    int toggle = 0;
    while (!QueueIsEmpty(src)) {
        int val = QueueDequeue(src, NULL);
        if (toggle == 0) QueueEnqueue(A, val, NULL);
        else             QueueEnqueue(B, val, NULL);
        toggle ^= 1;
    }
}

static int MergeSeries(List *A, List *B, List *dst, int *M, int *C) {
    int series = 0;
    while (!QueueIsEmpty(A) || !QueueIsEmpty(B)) {
        series++;
        int inA = !QueueIsEmpty(A);
        int inB = !QueueIsEmpty(B);
        int has_lastA = 0, has_lastB = 0;

        while (inA || inB) {
            int takeA = 0;
            if (inA && inB) {
                (*C)++;
                takeA = (A->head->data <= B->head->data);
            } else {
                takeA = inA;
            }

            if (takeA) {
                int val = QueueDequeue(A, M);
                QueueEnqueue(dst, val, M);
                if (has_lastA && A->head && A->head->data < val)
                    inA = 0;
                else if (!A->head)
                    inA = 0;
                has_lastA = 1;
            } else {
                int val = QueueDequeue(B, M);
                QueueEnqueue(dst, val, M);
                if (has_lastB && B->head && B->head->data < val)
                    inB = 0;
                else if (!B->head)
                    inB = 0;
                has_lastB = 1;
            }
        }
    }
    return series;
}

void ListMergeSort(List *L, int *M, int *C, int *series) {
    *M = 0; *C = 0;
    *series = ListSeriesCount(L);
    if (*series <= 1) return;

    while (1) {
        List A, B, tmp;
        Split(L, &A, &B);
        *M += A.size + B.size;

        ListInit(&tmp);
        int s = MergeSeries(&A, &B, &tmp, M, C);
        *series = s;
        *L = tmp;
        if (s <= 1) break;
    }
}

/* ════════════════════════════════════════
 *  DigitalSort (LSD Radix Sort, база 256)
 *  M = const * n  (сравнений нет)
 *  Константа = кол-во байт * 2 (dequeue + enqueue за проход)
 * ════════════════════════════════════════ */

#define RADIX 256
#define BITS    8

static void RadixPassSigned(List *L, int byte_pos, int total_bytes, int *M) {
    List buckets[RADIX];
    for (int i = 0; i < RADIX; i++) ListInit(&buckets[i]);

    while (!QueueIsEmpty(L)) {
        int val    = QueueDequeue(L, M);
        int bucket = ((unsigned int)val >> (byte_pos * BITS)) & 0xFF;
        QueueEnqueue(&buckets[bucket], val, M);
    }

    if (byte_pos == total_bytes - 1) {
        /* Знаковый старший байт: сначала 128-255 (отрицательные), потом 0-127 */
        for (int i = 128; i < RADIX; i++)
            while (!QueueIsEmpty(&buckets[i]))
                QueueEnqueue(L, QueueDequeue(&buckets[i], M), M);
        for (int i = 0; i < 128; i++)
            while (!QueueIsEmpty(&buckets[i]))
                QueueEnqueue(L, QueueDequeue(&buckets[i], M), M);
    } else {
        for (int i = 0; i < RADIX; i++)
            while (!QueueIsEmpty(&buckets[i]))
                QueueEnqueue(L, QueueDequeue(&buckets[i], M), M);
    }
}

void ListDigitalSort(List *L, int *M) {
    *M = 0;
    int bytes = (int)sizeof(int);
    for (int b = 0; b < bytes; b++)
        RadixPassSigned(L, b, bytes, M);
}

static void ReverseList(List *L) {
    Node *prev = NULL, *cur = L->head, *next;
    L->tail = L->head;
    while (cur) {
        next      = cur->next;
        cur->next = prev;
        prev      = cur;
        cur       = next;
    }
    L->head = prev;
}

void ListDigitalSortDes(List *L, int *M) {
    ListDigitalSort(L, M);
    ReverseList(L);
}

static void RadixPassSigned16(List *L, int byte_pos, int *M) {
    List buckets[RADIX];
    for (int i = 0; i < RADIX; i++) ListInit(&buckets[i]);

    while (!QueueIsEmpty(L)) {
        int      val    = QueueDequeue(L, M);
        uint16_t uv     = (uint16_t)(int16_t)(val & 0xFFFF);
        int      bucket = (uv >> (byte_pos * BITS)) & 0xFF;
        QueueEnqueue(&buckets[bucket], val, M);
    }

    if (byte_pos == 1) {
        for (int i = 128; i < RADIX; i++)
            while (!QueueIsEmpty(&buckets[i]))
                QueueEnqueue(L, QueueDequeue(&buckets[i], M), M);
        for (int i = 0; i < 128; i++)
            while (!QueueIsEmpty(&buckets[i]))
                QueueEnqueue(L, QueueDequeue(&buckets[i], M), M);
    } else {
        for (int i = 0; i < RADIX; i++)
            while (!QueueIsEmpty(&buckets[i]))
                QueueEnqueue(L, QueueDequeue(&buckets[i], M), M);
    }
}

void ListDigitalSort16(List *L, int *M) {
    *M = 0;
    for (int b = 0; b < 2; b++)
        RadixPassSigned16(L, b, M);
}

void ListDigitalSort16D(List *L, int *M) {
    ListDigitalSort16(L, M);
    ReverseList(L);
}

static void RadixPassSigned32(List *L, int byte_pos, int *M) {
    List buckets[RADIX];
    for (int i = 0; i < RADIX; i++) ListInit(&buckets[i]);

    while (!QueueIsEmpty(L)) {
        int val    = QueueDequeue(L, M);
        int bucket = ((unsigned int)val >> (byte_pos * BITS)) & 0xFF;
        QueueEnqueue(&buckets[bucket], val, M);
    }

    if (byte_pos == 3) {
        for (int i = 128; i < RADIX; i++)
            while (!QueueIsEmpty(&buckets[i]))
                QueueEnqueue(L, QueueDequeue(&buckets[i], M), M);
        for (int i = 0; i < 128; i++)
            while (!QueueIsEmpty(&buckets[i]))
                QueueEnqueue(L, QueueDequeue(&buckets[i], M), M);
    } else {
        for (int i = 0; i < RADIX; i++)
            while (!QueueIsEmpty(&buckets[i]))
                QueueEnqueue(L, QueueDequeue(&buckets[i], M), M);
    }
}

void ListDigitalSort32(List *L, int *M) {
    *M = 0;
    for (int b = 0; b < 4; b++)
        RadixPassSigned32(L, b, M);
}

void ListDigitalSort32D(List *L, int *M) {
    ListDigitalSort32(L, M);
    ReverseList(L);
}
