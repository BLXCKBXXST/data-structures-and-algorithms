#ifndef SORTING_H
#define SORTING_H

void FillInc(int n, int arr[n]);
void FillDec(int n, int arr[n]);
void FillRand(int n, int arr[n]);
void CopyArray(int n, int src[n], int dst[n]);

void SelectSort(int n, int arr[n], int *M, int *C);
void SelectSortImproved(int n, int arr[n], int *M, int *C);
void BubbleSort(int n, int arr[n], int *M, int *C);
void ShakerSort(int n, int arr[n], int *M, int *C);
void InsertSort(int n, int arr[n], int *M, int *C);
void ShellSort(int ns, int steps[ns], int n, int arr[n], int *M, int *C);

/* Заполняет dec/rnd/inc массивы и прогоняет sort, возвращает M и C для каждого случая */
void RunSort(int n, void(*sort)(int, int*, int*, int*),
             int *Mdec, int *Cdec,
             int *Mrnd,  int *Crnd,
             int *Minc,  int *Cinc);

/* Аналог RunSort для ShellSort (отдельная сигнатура из-за параметров шагов) */
void RunShellSort(int n, int ns, int steps[ns],
                  int *Mdec, int *Cdec,
                  int *Mrnd,  int *Crnd,
                  int *Minc,  int *Cinc);

#endif
