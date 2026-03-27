#include <stdlib.h>
#include "sorting.h"

void FillInc(int n, int arr[n]) {
    for (int i = 0; i < n; i++)
        arr[i] = i;
}

void FillDec(int n, int arr[n]) {
    for (int i = 0; i < n; i++)
        arr[i] = n - 1 - i;
}

void FillRand(int n, int arr[n]) {
    for (int i = 0; i < n; i++)
        arr[i] = rand() % (n * n);
}

void CopyArray(int n, int src[n], int dst[n]) {
    for (int i = 0; i < n; i++)
        dst[i] = src[i];
}

void SelectSort(int n, int arr[n], int *M, int *C) {
    *M = 0; *C = 0;
    for (int i = 0; i < n - 1; i++) {
        int minidx = i;
        for (int j = i + 1; j < n; j++) {
            (*C)++;
            if (arr[j] < arr[minidx]) minidx = j;
        }
        int tmp = arr[i]; arr[i] = arr[minidx]; arr[minidx] = tmp;
        *M += 3;
    }
}

void SelectSortImproved(int n, int arr[n], int *M, int *C) {
    *M = 0; *C = 0;
    for (int i = 0; i < n - 1; i++) {
        int minidx = i;
        for (int j = i + 1; j < n; j++) {
            (*C)++;
            if (arr[j] < arr[minidx]) minidx = j;
        }
        if (minidx != i) {
            int tmp = arr[i]; arr[i] = arr[minidx]; arr[minidx] = tmp;
            *M += 3;
        }
    }
}

void BubbleSort(int n, int arr[n], int *M, int *C) {
    *M = 0; *C = 0;
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - 1 - i; j++) {
            (*C)++;
            if (arr[j] > arr[j + 1]) {
                int tmp = arr[j]; arr[j] = arr[j + 1]; arr[j + 1] = tmp;
                *M += 3;
            }
        }
    }
}

void ShakerSort(int n, int arr[n], int *M, int *C) {
    *M = 0; *C = 0;
    int left = 0, right = n - 1;
    while (left < right) {
        int swapped = 0;
        for (int i = left; i < right; i++) {
            (*C)++;
            if (arr[i] > arr[i + 1]) {
                int tmp = arr[i]; arr[i] = arr[i + 1]; arr[i + 1] = tmp;
                *M += 3; swapped = 1;
            }
        }
        right--;
        if (!swapped) break;
        for (int i = right; i > left; i--) {
            (*C)++;
            if (arr[i - 1] > arr[i]) {
                int tmp = arr[i - 1]; arr[i - 1] = arr[i]; arr[i] = tmp;
                *M += 3; swapped = 1;
            }
        }
        left++;
        if (!swapped) break;
    }
}

void InsertSort(int n, int arr[n], int *M, int *C) {
    *M = 0; *C = 0;
    for (int i = 1; i < n; i++) {
        int key = arr[i];
        (*M)++;
        int j = i - 1;
        while (j >= 0) {
            (*C)++;
            if (arr[j] > key) {
                arr[j + 1] = arr[j];
                (*M)++; j--;
            } else break;
        }
        arr[j + 1] = key;
        (*M)++;
    }
}

void ShellSort(int ns, int steps[ns], int n, int arr[n], int *M, int *C) {
    *M = 0; *C = 0;
    for (int si = 0; si < ns; si++) {
        int h = steps[si];
        for (int i = h; i < n; i++) {
            int key = arr[i];
            (*M)++;
            int j = i - h;
            while (j >= 0) {
                (*C)++;
                if (arr[j] > key) {
                    arr[j + h] = arr[j];
                    (*M)++; j -= h;
                } else break;
            }
            arr[j + h] = key;
            (*M)++;
        }
    }
}

/* --- HeapSort --- */

/* Просеивание элемента i вниз в куче размером n */
static void SiftDown(int n, int arr[], int i, int *M, int *C) {
    while (1) {
        int largest = i;
        int l = 2 * i + 1;
        int r = 2 * i + 2;
        if (l < n) {
            (*C)++;
            if (arr[l] > arr[largest]) largest = l;
        }
        if (r < n) {
            (*C)++;
            if (arr[r] > arr[largest]) largest = r;
        }
        if (largest == i) break;
        int tmp = arr[i]; arr[i] = arr[largest]; arr[largest] = tmp;
        *M += 3;
        i = largest;
    }
}

void HeapSort(int n, int arr[n], int *M, int *C) {
    *M = 0; *C = 0;
    /* Построение кучи (фаза 1) */
    for (int i = n / 2 - 1; i >= 0; i--)
        SiftDown(n, arr, i, M, C);
    /* Извлечение элементов (фаза 2) */
    for (int end = n - 1; end > 0; end--) {
        int tmp = arr[0]; arr[0] = arr[end]; arr[end] = tmp;
        *M += 3;
        SiftDown(end, arr, 0, M, C);
    }
}

/* --- QuickSort --- */

/* Версия 1: опорный элемент — средний (arr[(lo+hi)/2]) */
static void qs1(int arr[], int lo, int hi, int *M, int *C, int d, int *maxd) {
    if (d > *maxd) *maxd = d;
    if (lo >= hi) return;
    int i = lo, j = hi;
    int pivot = arr[(lo + hi) / 2];
    (*M)++;  /* чтение опорного */
    while (i <= j) {
        while (1) { (*C)++; if (arr[i] >= pivot) break; i++; }
        while (1) { (*C)++; if (arr[j] <= pivot) break; j--; }
        if (i <= j) {
            int tmp = arr[i]; arr[i] = arr[j]; arr[j] = tmp;
            *M += 3;
            i++; j--;
        }
    }
    if (lo < j)  qs1(arr, lo, j, M, C, d + 1, maxd);
    if (i < hi)  qs1(arr, i, hi, M, C, d + 1, maxd);
}

void QuickSort1(int n, int arr[n], int *M, int *C, int *depth) {
    *M = 0; *C = 0; *depth = 0;
    if (n > 1) qs1(arr, 0, n - 1, M, C, 1, depth);
}

/* Версия 2: опорный элемент — медиана из трёх (arr[lo], arr[mid], arr[hi]) */
static int median3(int arr[], int lo, int hi, int *C) {
    int mid = (lo + hi) / 2;
    /* Сортируем три элемента и возвращаем средний */
    (*C)++;
    if (arr[lo] > arr[mid]) { int t = arr[lo]; arr[lo] = arr[mid]; arr[mid] = t; }
    (*C)++;
    if (arr[lo] > arr[hi])  { int t = arr[lo]; arr[lo] = arr[hi];  arr[hi]  = t; }
    (*C)++;
    if (arr[mid] > arr[hi]) { int t = arr[mid]; arr[mid] = arr[hi]; arr[hi]  = t; }
    return arr[mid];
}

static void qs2(int arr[], int lo, int hi, int *M, int *C, int d, int *maxd) {
    if (d > *maxd) *maxd = d;
    if (lo >= hi) return;
    int i = lo, j = hi;
    int pivot = (hi - lo >= 2) ? median3(arr, lo, hi, C) : arr[(lo + hi) / 2];
    (*M)++;  /* чтение опорного */
    while (i <= j) {
        while (1) { (*C)++; if (arr[i] >= pivot) break; i++; }
        while (1) { (*C)++; if (arr[j] <= pivot) break; j--; }
        if (i <= j) {
            int tmp = arr[i]; arr[i] = arr[j]; arr[j] = tmp;
            *M += 3;
            i++; j--;
        }
    }
    if (lo < j)  qs2(arr, lo, j, M, C, d + 1, maxd);
    if (i < hi)  qs2(arr, i, hi, M, C, d + 1, maxd);
}

void QuickSort2(int n, int arr[n], int *M, int *C, int *depth) {
    *M = 0; *C = 0; *depth = 0;
    if (n > 1) qs2(arr, 0, n - 1, M, C, 1, depth);
}

void RunSort(int n, void(*sort)(int, int*, int*, int*),
             int *Mdec, int *Cdec,
             int *Mrnd,  int *Crnd,
             int *Minc,  int *Cinc) {
    int A[n], B[n], C[n], arr[n];
    FillInc(n, A); FillDec(n, B); FillRand(n, C);
    CopyArray(n, B, arr); sort(n, arr, Mdec, Cdec);
    CopyArray(n, C, arr); sort(n, arr, Mrnd, Crnd);
    CopyArray(n, A, arr); sort(n, arr, Minc, Cinc);
}

void RunShellSort(int n, int ns, int steps[ns],
                  int *Mdec, int *Cdec,
                  int *Mrnd,  int *Crnd,
                  int *Minc,  int *Cinc) {
    int A[n], B[n], C[n], arr[n];
    FillInc(n, A); FillDec(n, B); FillRand(n, C);
    CopyArray(n, B, arr); ShellSort(ns, steps, n, arr, Mdec, Cdec);
    CopyArray(n, C, arr); ShellSort(ns, steps, n, arr, Mrnd, Crnd);
    CopyArray(n, A, arr); ShellSort(ns, steps, n, arr, Minc, Cinc);
}
