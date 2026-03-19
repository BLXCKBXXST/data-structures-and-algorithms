#include "search.h"

/* ------------------------------------------------------------------ */
/* I версия: на каждом шаге 3 ветки (<, >, ==)                        */
/* Теория: C_avg = log2(n), C_worst = floor(log2(n)) + 1              */
/* ------------------------------------------------------------------ */
int BSearch1(int n, int arr[n], int key, int *C) {
    *C = 0;
    int lo = 0, hi = n - 1;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        (*C)++;
        if (key < arr[mid]) {
            hi = mid - 1;
        } else {
            (*C)++;
            if (key > arr[mid]) {
                lo = mid + 1;
            } else {
                return mid;   /* нашли */
            }
        }
    }
    return -1;
}

/* ------------------------------------------------------------------ */
/* II версия: только 1 сравнение на шаг (<), проверка == в конце      */
/* Всегда делает ровно ceil(log2(n+1)) итераций                       */
/* ------------------------------------------------------------------ */
int BSearch2(int n, int arr[n], int key, int *C) {
    *C = 0;
    int lo = 0, hi = n; /* hi — исключающая граница */
    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        (*C)++;
        if (key < arr[mid]) {
            hi = mid;
        } else {
            lo = mid + 1;
        }
    }
    /* lo-1 — кандидат */
    int pos = lo - 1;
    (*C)++;
    if (pos >= 0 && arr[pos] == key) return pos;
    return -1;
}

/* ------------------------------------------------------------------ */
/* BSearchAll1: найти первое вхождение через BSearch1, затем          */
/* линейно расширить влево/вправо                                     */
/* ------------------------------------------------------------------ */
int BSearchAll1(int n, int arr[n], int key, int found[], int *C) {
    *C = 0;
    /* Найти любое вхождение */
    int lo = 0, hi = n - 1, any = -1;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        (*C)++;
        if (key < arr[mid]) {
            hi = mid - 1;
        } else {
            (*C)++;
            if (key > arr[mid]) {
                lo = mid + 1;
            } else {
                any = mid; break;
            }
        }
    }
    if (any == -1) return 0;
    /* Расширить влево */
    int l = any, r = any;
    while (l > 0) {
        (*C)++;
        if (arr[l - 1] == key) l--; else break;
    }
    while (r < n - 1) {
        (*C)++;
        if (arr[r + 1] == key) r++; else break;
    }
    int cnt = 0;
    for (int i = l; i <= r; i++) found[cnt++] = i;
    return cnt;
}

/* ------------------------------------------------------------------ */
/* BSearchAll2: найти левую границу (lower_bound) и правую (upper_bound) */
/* с помощью двух двоичных поисков — чисто логарифмически             */
/* ------------------------------------------------------------------ */
static int LowerBound(int n, int arr[], int key, int *C) {
    int lo = 0, hi = n;
    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        (*C)++;
        if (arr[mid] < key) lo = mid + 1;
        else                hi = mid;
    }
    return lo;
}

static int UpperBound(int n, int arr[], int key, int *C) {
    int lo = 0, hi = n;
    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        (*C)++;
        if (arr[mid] <= key) lo = mid + 1;
        else                 hi = mid;
    }
    return lo;
}

int BSearchAll2(int n, int arr[n], int key, int found[], int *C) {
    *C = 0;
    int l = LowerBound(n, arr, key, C);
    int r = UpperBound(n, arr, key, C);
    /* Проверяем, есть ли хоть одно вхождение */
    if (l >= r) return 0;
    int cnt = 0;
    for (int i = l; i < r; i++) found[cnt++] = i;
    return cnt;
}
