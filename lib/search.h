#ifndef SEARCH_H
#define SEARCH_H

/*
 * BSearch1 — классический двоичный поиск (3-сравнения: <, >, ==).
 * BSearch2 — двоичный поиск с 2-сравнениями на шаг (только <),
 *            финальная проверка равенства в конце.
 * Обе возвращают индекс найденного элемента или -1.
 * *C — счётчик сравнений.
 */
int BSearch1(int n, int arr[n], int key, int *C);
int BSearch2(int n, int arr[n], int key, int *C);

/*
 * BSearchAll1/2 — поиск ВСЕХ вхождений ключа.
 * found[]  — выходной массив найденных индексов (caller выделяет память, размер n).
 * Возвращает количество найденных элементов.
 * *C — счётчик сравнений.
 */
int BSearchAll1(int n, int arr[n], int key, int found[], int *C);
int BSearchAll2(int n, int arr[n], int key, int found[], int *C);

#endif
