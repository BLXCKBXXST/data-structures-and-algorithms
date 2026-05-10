# Часть 1 — Алгоритмы обработки массивов

Задачи 1.1–1.10 из методички [docs/arrays.pdf](../../docs/arrays.pdf).
Каждая программа сравнивает фактические M (пересылки) и C (сравнения) с
теоретическими оценками из [docs/labor_intensity.pdf](../../docs/labor_intensity.pdf).

## Список задач

| Файл                                            | Бинарник            | Алиас     | Что делает                                                                |
|-------------------------------------------------|---------------------|-----------|---------------------------------------------------------------------------|
| [1_1_select_sort.c](1_1_select_sort.c)          | `1_1_select_sort`   | `select`  | SelectSort + улучшенный SelectSort (без фиктивных перестановок)           |
| [1_2_bubble_sort.c](1_2_bubble_sort.c)          | `1_2_bubble_sort`   | `bubble`  | BubbleSort, разбор M и C по типам массивов                                |
| [1_3_shaker_sort.c](1_3_shaker_sort.c)          | `1_3_shaker_sort`   | `shaker`  | ShakerSort с флагом swapped + сравнение с BubbleSort                      |
| [1_4_insert_sort.c](1_4_insert_sort.c)          | `1_4_insert_sort`   | `insert`  | InsertSort + сводная таблица 4 квадратичных методов                       |
| [1_5_shell_sort.c](1_5_shell_sort.c)            | `1_5_shell_sort`    | `shell`   | ShellSort с шагами Кнута и Хиббарда (`O(n^1.2)`)                          |
| [1_6_binary_search.c](1_6_binary_search.c)      | `1_6_binary_search` | `bsearch` | Двоичный поиск (2 версии) + BSearchAll1/All2                              |
| [1_7_struct_sort.c](1_7_struct_sort.c)          | `1_7_struct_sort`   | `struct`  | Сортировка структур (телефонный справочник) по составному ключу           |
| [1_8_index_sort.c](1_8_index_sort.c)            | `1_8_index_sort`    | `index`   | Индексация: два индексных массива указателей по разным полям              |
| [1_9_heap_sort.c](1_9_heap_sort.c)              | `1_9_heap_sort`     | `heap`    | HeapSort + отдельно `BuildHeap` (фаза построения пирамиды)                |
| [1_10_quick_sort.c](1_10_quick_sort.c)          | `1_10_quick_sort`   | `quick`   | QuickSort двух версий (опорный = `arr[lo]` vs медиана из трёх) + глубина рекурсии |

## Сборка и запуск

```sh
make                       # из корня проекта — собрать всё
make 1_1_select_sort       # собрать одну программу (или: make select)
./build/bin/1_1_select_sort
```

## Используемые модули

- [lib/sorting.h](../../lib/sorting.h) — алгоритмы сортировки и заполнители (`FillInc/Dec/Rand`, `RunSort`).
- [lib/search.h](../../lib/search.h) — `BSearch1`, `BSearch2`, `BSearchAll1`, `BSearchAll2`.

## Что выводит каждая программа

1. **Главная ТЗ-таблица** — по форме методички, с колонкой «M+C теоретич.»
   (одна или три колонки, в зависимости от того, зависит ли теория от типа массива).
2. **`=== Доп. ===`** — детальный разбор Mt/Ct теории отдельно vs Мф/Сф факта по
   убыв./случ./возр.

На убыв./возр. факт **точно совпадает** с теорией; на случайных — близко.
