#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
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

/* ╔══════════════════════════════════════════════════════════════╗
 * ║   НАЧАЛО: Стек — push/pop на Head (методичка 5.2)            ║
 * ╚══════════════════════════════════════════════════════════════╝ */

/*  Добавление элемента p в стек (методичка рис. 14):
 *      1) p → Next := Head
 *      2) Head := p
 */
void StackPush(List *S, int val, int *M) {
    Node *p  = NewNode(val);
    p->next  = S->head;                          /* p → Next := Head */
    S->head  = p;                                /* Head := p */
    S->size++;
    if (M) (*M)++;
}

/*  Удаление элемента из стека (методичка рис. 15):
 *      1) p := Head
 *      2) Head := p → Next
 *      <освободить память по адресу p>
 */
int StackPop(List *S, int *M) {
    if (!S->head) { fprintf(stderr, "StackPop: empty\n"); exit(1); }
    Node *p   = S->head;                         /* p := Head */
    int   val = p->data;
    S->head   = p->next;                         /* Head := p → Next */
    free(p);
    S->size--;
    if (M) (*M)++;
    return val;
}

int StackIsEmpty(const List *S) { return S->head == NULL; }

/* ╔══════════════════════════════════════════════════════════════╗
 * ║   КОНЕЦ: Стек                                                ║
 * ╚══════════════════════════════════════════════════════════════╝ */


/* ╔══════════════════════════════════════════════════════════════╗
 * ║   НАЧАЛО: Очередь — enqueue на Tail, dequeue с Head          ║
 * ║            (методичка 5.2)                                   ║
 * ╚══════════════════════════════════════════════════════════════╝ */

/*  Добавление элемента p в очередь (методичка рис. 16):
 *      1) p → Next := NIL
 *      2) IF (Head ≠ NIL) Tail → Next := p  ELSE Head := p  FI
 *      3) Tail := p
 */
void QueueEnqueue(List *Q, int val, int *M) {
    Node *p = NewNode(val);
    p->next = NULL;                              /* p → Next := NIL */
    if (Q->tail) Q->tail->next = p;              /* Tail → Next := p (Head ≠ NIL) */
    else         Q->head       = p;              /* Head := p */
    Q->tail = p;                                 /* Tail := p */
    Q->size++;
    if (M) (*M)++;
}

/*  Удаление из очереди (та же схема, что для стека — рис. 15):
 *      1) p := Head
 *      2) Head := p → Next
 *      <если очередь опустела, Tail := NIL>
 */
int QueueDequeue(List *Q, int *M) {
    if (!Q->head) { fprintf(stderr, "QueueDequeue: empty\n"); exit(1); }
    Node *p   = Q->head;                         /* p := Head */
    int   val = p->data;
    Q->head   = p->next;                         /* Head := p → Next */
    if (!Q->head) Q->tail = NULL;
    free(p);
    Q->size--;
    if (M) (*M)++;
    return val;
}

int QueueIsEmpty(const List *Q) { return Q->head == NULL; }

/* ╔══════════════════════════════════════════════════════════════╗
 * ║   КОНЕЦ: Очередь                                             ║
 * ╚══════════════════════════════════════════════════════════════╝ */

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

/* ╔══════════════════════════════════════════════════════════════╗
 * ║   НАЧАЛО: Метод прямого слияния (методичка 6.1)              ║
 * ╚══════════════════════════════════════════════════════════════╝ */

/*  Перемещение элемента из начала списка *src в конец очереди dst
 *  (методичка стр. 30, рис. 19):
 *      1) Queue.Tail → Next := List
 *      2) Queue.Tail := List
 *      3) List := List → Next
 *  Узел не копируется — перевязывается через next. Одна пересылка (M+1). */
static void MoveHead(Node **src, List *dst, int *M) {
    Node *p = *src;
    *src = p->next;                                  /* List := List → Next */
    p->next = NULL;
    if (dst->tail) dst->tail->next = p;              /* Queue.Tail → Next := List */
    else           dst->head       = p;
    dst->tail = p;                                   /* Queue.Tail := List */
    dst->size++;
    (*M)++;
}

/*  Слияние q-серии из списка *a и r-серии из *b в очередь c.
 *  Псевдокод (методичка 6.1):
 *      DO (q ≠ 0 и r ≠ 0)
 *          IF (a → Data ≤ b → Data)
 *              <Переместить элемент из a в c>;  q := q - 1
 *          ELSE
 *              <Переместить элемент из b в c>;  r := r - 1
 *          FI
 *      OD
 *      DO (q > 0)  <переместить из a в c>;  q := q - 1  OD
 *      DO (r > 0)  <переместить из b в c>;  r := r - 1  OD
 *  Трудоёмкость одного слияния: C ≤ q + r - 1,  M = q + r. */
static void MergeAB(Node **a, int q, Node **b, int r,
                    List *c, int *M, int *C) {
    while (q != 0 && r != 0) {                        /* DO (q ≠ 0 и r ≠ 0) */
        (*C)++;
        if ((*a)->data <= (*b)->data) {               /* IF (a → Data ≤ b → Data) */
            MoveHead(a, c, M);
            q = q - 1;
        } else {
            MoveHead(b, c, M);
            r = r - 1;
        }
    }
    while (q > 0) { MoveHead(a, c, M); q = q - 1; }   /* DO (q > 0) */
    while (r > 0) { MoveHead(b, c, M); r = r - 1; }   /* DO (r > 0) */
}

/*  Главный алгоритм. Псевдокод (методичка 6.1):
 *      <Расщепление (S, a, b, n)>
 *      p := 1
 *      DO (p < n)
 *          <инициализация очередей c_0, c_1>
 *          i := 0,  m := n
 *          DO (m > 0)
 *              IF (m ≥ p) q := p ELSE q := m FI;  m := m - q
 *              IF (m ≥ p) r := p ELSE r := m FI;  m := m - r
 *              <слияние(a, q, b, r, c_i)>
 *              i := 1 - i
 *          OD
 *          a := c_0.Head,  b := c_1.Head
 *          p := 2p
 *      OD
 *      c_0.Tail → next := NIL
 *      S := c_0.Head
 *
 *  Расщепление (методичка стр. 33):
 *      a := S,  b := S → Next,  n := 1
 *      k := a,  p := b
 *      DO (p ≠ NIL)
 *          n := n + 1
 *          k → next := p → next
 *          k := p
 *          p := p → next
 *      OD
 *
 *  Трудоёмкость:  C < n⌈log₂n⌉,  M = n⌈log₂n⌉ + n
 *  (дополнительные n пересылок — на начальном расщеплении). */
void ListMergeSort(List *L, int *M, int *C, int *series) {
    *M = 0;
    *C = 0;

    if (L->size == 0) { *series = 0; return; }
    if (L->size == 1) { *series = 1; return; }

    /* ── Расщепление S → a, b; считаем n ── */
    Node *a = L->head;                               /* a := S */
    Node *b = L->head->next;                         /* b := S → Next */
    int n = 1;
    Node *k = a, *p = b;
    while (p) {                                       /* DO (p ≠ NIL) */
        n = n + 1;
        k->next = p->next;                            /* k → next := p → next */
        (*M)++;
        k = p;                                        /* k := p */
        p = p->next;                                  /* p := p → next */
    }
    L->head = L->tail = NULL;
    L->size = 0;

    /* ── Основной цикл сортировки прямым слиянием ── */
    List c0, c1;
    ListInit(&c0); ListInit(&c1);
    int psize = 1;                                    /* p := 1 */
    while (psize < n) {                               /* DO (p < n) */
        ListInit(&c0); ListInit(&c1);                 /* инициализация c_0, c_1 */
        int i = 0;
        int m = n;
        while (m > 0) {                               /* DO (m > 0) */
            int q = (m >= psize) ? psize : m;          /* IF (m ≥ p) q := p ELSE q := m */
            m = m - q;
            int r = (m >= psize) ? psize : m;          /* IF (m ≥ p) r := p ELSE r := m */
            m = m - r;
            List *ci = (i == 0) ? &c0 : &c1;
            MergeAB(&a, q, &b, r, ci, M, C);          /* <слияние(a, q, b, r, c_i)> */
            i = 1 - i;
        }
        a = c0.head;                                   /* a := c_0.Head */
        b = c1.head;                                   /* b := c_1.Head */
        psize = 2 * psize;                             /* p := 2p */
    }

    /* c_0.Tail → next := NIL;  S := c_0.Head */
    if (c0.tail) c0.tail->next = NULL;
    L->head = c0.head;
    L->tail = c0.tail;
    L->size = c0.size;

    *series = ListSeriesCount(L);
}

/* ╔══════════════════════════════════════════════════════════════╗
 * ║   КОНЕЦ: Метод прямого слияния                               ║
 * ╚══════════════════════════════════════════════════════════════╝ */

/* ╔══════════════════════════════════════════════════════════════╗
 * ║   НАЧАЛО: Цифровая сортировка (LSD radix, база 256)          ║
 * ║   in-place перевязка указателей по псевдокоду методички:     ║
 * ║     раздача — Q_d.Tail->Next := p   (одно присваивание на    ║
 * ║                                      узел = одна пересылка)  ║
 * ║     сборка  — p->Next := Q_i.Head   (склейка цепочки         ║
 * ║                                      непустой корзины)       ║
 * ║   Узлы НЕ копируются: никаких malloc/free в сортировке.      ║
 * ║   M = bytes·n (по числу перевязок в раздаче), C = 0.         ║
 * ╚══════════════════════════════════════════════════════════════╝ */

#define RADIX 256
#define BITS    8

/* Сборка результирующего списка из 256 корзин в порядке
 * 0..255 (или 128..255, затем 0..127 — на знаковом проходе).
 * Склейка цепочек: служебное действие, в счёт M пересылок
 * элементов не входит. */
static void CollectBuckets(List *L, List buckets[RADIX], int sign_pass) {
    L->head = L->tail = NULL;
    L->size = 0;

    int order[RADIX], k = 0;
    if (sign_pass) {
        for (int i = 128; i < RADIX; i++) order[k++] = i;
        for (int i = 0;   i < 128;   i++) order[k++] = i;
    } else {
        for (int i = 0;   i < RADIX; i++) order[k++] = i;
    }

    for (int j = 0; j < RADIX; j++) {
        int i = order[j];
        if (!buckets[i].head) continue;
        if (L->tail) L->tail->next = buckets[i].head;       /* p->Next := Q_i.Head */
        else         L->head       = buckets[i].head;
        L->tail = buckets[i].tail;
        L->size += buckets[i].size;
    }
    if (L->tail) L->tail->next = NULL;                      /* p->Next := NIL */
}

/* Один байтовый проход для int-ключа: byte_pos = 0 — младший. */
static void RadixPass(List *L, int byte_pos, int sign_pass, int *M) {
    List buckets[RADIX];
    for (int i = 0; i < RADIX; i++) ListInit(&buckets[i]);

    /* ── Раздача узлов по корзинам ── */
    Node *p = L->head;
    while (p) {
        Node *next = p->next;                               /* сохранить ДО перевязки */
        int d = ((unsigned int)p->data >> (byte_pos * BITS)) & 0xFF;
        p->next = NULL;
        if (buckets[d].tail) buckets[d].tail->next = p;     /* Q_d.Tail->Next := p */
        else                 buckets[d].head       = p;
        buckets[d].tail = p;                                /* Q_d.Tail := p */
        buckets[d].size++;
        (*M)++;
        p = next;
    }

    /* ── Сборка результата ── */
    CollectBuckets(L, buckets, sign_pass);
}

/* То же, но ключ интерпретируется как int16_t (2 значащих байта
 * со знаковым расширением из 16 бит). */
static void RadixPass16(List *L, int byte_pos, int sign_pass, int *M) {
    List buckets[RADIX];
    for (int i = 0; i < RADIX; i++) ListInit(&buckets[i]);

    Node *p = L->head;
    while (p) {
        Node *next = p->next;
        uint16_t uv = (uint16_t)(int16_t)(p->data & 0xFFFF);
        int d = (uv >> (byte_pos * BITS)) & 0xFF;
        p->next = NULL;
        if (buckets[d].tail) buckets[d].tail->next = p;
        else                 buckets[d].head       = p;
        buckets[d].tail = p;
        buckets[d].size++;
        (*M)++;
        p = next;
    }

    CollectBuckets(L, buckets, sign_pass);
}

/* Реверс списка для *Des-вариантов. В счёт M сортировки не входит. */
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

/* ── int (sizeof(int) байт), по возрастанию ── */
void ListDigitalSort(List *L, int *M) {
    *M = 0;
    int bytes = (int)sizeof(int);
    for (int b = 0; b < bytes; b++)
        RadixPass(L, b, b == bytes - 1, M);
}

/* ── int, по убыванию ── */
void ListDigitalSortDes(List *L, int *M) {
    ListDigitalSort(L, M);
    ReverseList(L);
}

/* ── int16_t (2 байта), по возрастанию и по убыванию ── */
void ListDigitalSort16(List *L, int *M) {
    *M = 0;
    for (int b = 0; b < 2; b++)
        RadixPass16(L, b, b == 1, M);
}

void ListDigitalSort16D(List *L, int *M) {
    ListDigitalSort16(L, M);
    ReverseList(L);
}

/* ── int32_t (4 байта), по возрастанию и по убыванию ── */
void ListDigitalSort32(List *L, int *M) {
    *M = 0;
    for (int b = 0; b < 4; b++)
        RadixPass(L, b, b == 3, M);
}

void ListDigitalSort32D(List *L, int *M) {
    ListDigitalSort32(L, M);
    ReverseList(L);
}

/* ╔══════════════════════════════════════════════════════════════╗
 * ║   КОНЕЦ: Цифровая сортировка                                 ║
 * ╚══════════════════════════════════════════════════════════════╝ */

/* ════════════════════════════════════════
 *  StrList — список строк (для 2.3.6*)
 * ════════════════════════════════════════ */

void StrListInit(StrList *L) {
    L->head = L->tail = NULL;
    L->size = 0;
}

void StrListFree(StrList *L) {
    StrNode *p = L->head;
    while (p) {
        StrNode *t = p->next;
        free(p->data);
        free(p);
        p = t;
    }
    L->head = L->tail = NULL;
    L->size = 0;
}

void StrListAppend(StrList *L, const char *s) {
    StrNode *n = malloc(sizeof(StrNode));
    if (!n) { fprintf(stderr, "malloc failed\n"); exit(1); }
    n->data = strdup(s);
    if (!n->data) { fprintf(stderr, "strdup failed\n"); exit(1); }
    n->next = NULL;
    if (L->tail) L->tail->next = n;
    else         L->head       = n;
    L->tail = n;
    L->size++;
}

void StrListPrint(const StrList *L) {
    for (StrNode *p = L->head; p; p = p->next)
        printf("%s ", p->data);
    printf("\n");
}

/* DigitalSort строк по байтам справа налево.
 * Все строки дополняются 0-байтами справа до max длины.
 * asc=1 — по возрастанию (стандартный LSD); asc=0 — реверс в конце. */
void StrListDigitalSort(StrList *L, int asc, int *M) {
    *M = 0;
    int n = L->size;
    if (n <= 1) return;

    /* Найти max длину */
    int maxlen = 0;
    for (StrNode *p = L->head; p; p = p->next) {
        int len = (int)strlen(p->data);
        if (len > maxlen) maxlen = len;
    }

    /* LSD-радикс по байтам с позиции maxlen-1 до 0 */
    for (int pos = maxlen - 1; pos >= 0; pos--) {
        StrList buckets[256];
        for (int i = 0; i < 256; i++) StrListInit(&buckets[i]);

        StrNode *p = L->head;
        while (p) {
            StrNode *next = p->next;
            int len = (int)strlen(p->data);
            int b = (pos < len) ? (unsigned char)p->data[pos] : 0;
            p->next = NULL;
            if (buckets[b].tail) buckets[b].tail->next = p;
            else                 buckets[b].head       = p;
            buckets[b].tail = p;
            buckets[b].size++;
            (*M)++;
            p = next;
        }

        L->head = L->tail = NULL;
        L->size = 0;
        for (int i = 0; i < 256; i++) {
            if (!buckets[i].head) continue;
            if (L->tail) L->tail->next = buckets[i].head;
            else         L->head       = buckets[i].head;
            L->tail = buckets[i].tail;
            L->size += buckets[i].size;
        }
    }

    if (!asc) {
        /* Реверс */
        StrNode *prev = NULL, *cur = L->head, *next;
        L->tail = L->head;
        while (cur) {
            next      = cur->next;
            cur->next = prev;
            prev      = cur;
            cur       = next;
        }
        L->head = prev;
    }
}
