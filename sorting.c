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
        /* Своп всегда — намеренно, для сравнения с SelectSortImproved */
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
