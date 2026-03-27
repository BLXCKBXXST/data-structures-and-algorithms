/*
 * Раздел 1.8 — Индексация массивов (телефонный справочник)
 * Строим индексные массивы для двух полей (Фамилия и Адрес)
 * Сортировка: InsertSort индексного массива (не BubbleSort!)
 * Бонус: двоичный поиск по двум ключам с использованием индексов
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Запись телефонного справочника (5 полей) */
typedef struct {
    char surname[32];
    char name[32];
    char patronymic[32];
    char phone[16];
    char address[64];
} PhoneEntry;

/* Глобальный массив данных (для доступа из функций сравнения) */
static PhoneEntry *g_book;

/* Сравнение по фамилии → имя через индексы */
static int cmp_idx_surname(const void *a, const void *b) {
    int ia = *(const int *)a;
    int ib = *(const int *)b;
    int r = strcmp(g_book[ia].surname, g_book[ib].surname);
    if (r != 0) return r;
    return strcmp(g_book[ia].name, g_book[ib].name);
}

/* Сравнение по адресу → фамилия через индексы */
static int cmp_idx_address(const void *a, const void *b) {
    int ia = *(const int *)a;
    int ib = *(const int *)b;
    int r = strcmp(g_book[ia].address, g_book[ib].address);
    if (r != 0) return r;
    return strcmp(g_book[ia].surname, g_book[ib].surname);
}

typedef int (*CmpIdxFunc)(const void *, const void *);

/* InsertSort для индексного массива с подсчётом M и C */
static void InsertSortIndex(int idx[], int n, CmpIdxFunc cmp, int *M, int *C) {
    *M = 0; *C = 0;
    for (int i = 1; i < n; i++) {
        int key = idx[i];
        (*M)++;
        int j = i - 1;
        while (j >= 0) {
            (*C)++;
            if (cmp(&idx[j], &key) > 0) {
                idx[j + 1] = idx[j];
                (*M)++;
                j--;
            } else break;
        }
        idx[j + 1] = key;
        (*M)++;
    }
}

/* Двоичный поиск по фамилии через индексный массив */
static int BSearchIdxSurname(PhoneEntry book[], int idx[], int n,
                             const char *surname, int *C) {
    *C = 0;
    int lo = 0, hi = n - 1;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        (*C)++;
        int r = strcmp(surname, book[idx[mid]].surname);
        if (r < 0) {
            hi = mid - 1;
        } else if (r > 0) {
            (*C)++;
            lo = mid + 1;
        } else {
            return mid; /* позиция в индексном массиве */
        }
    }
    return -1;
}

/* Двоичный поиск по адресу через индексный массив */
static int BSearchIdxAddress(PhoneEntry book[], int idx[], int n,
                             const char *address, int *C) {
    *C = 0;
    int lo = 0, hi = n - 1;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        (*C)++;
        int r = strcmp(address, book[idx[mid]].address);
        if (r < 0) {
            hi = mid - 1;
        } else if (r > 0) {
            (*C)++;
            lo = mid + 1;
        } else {
            return mid;
        }
    }
    return -1;
}

/* Вывод справочника по индексу */
static void PrintBookByIndex(const char *title, PhoneEntry book[], int idx[], int n) {
    printf("\n%s\n", title);
    printf("┌────┬─────┬──────────────┬──────────────┬──────────────┬──────────────┬──────────────────────┐\n");
    printf("│  # │ Idx │   Фамилия    │     Имя      │  Отчество    │   Телефон    │        Адрес         │\n");
    printf("├────┼─────┼──────────────┼──────────────┼──────────────┼──────────────┼──────────────────────┤\n");
    for (int i = 0; i < n; i++) {
        int k = idx[i];
        printf("│ %2d │  %2d │ %-12s │ %-12s │ %-12s │ %-12s │ %-20s │\n",
            i + 1, k, book[k].surname, book[k].name, book[k].patronymic,
            book[k].phone, book[k].address);
    }
    printf("└────┴─────┴──────────────┴──────────────┴──────────────┴──────────────┴──────────────────────┘\n");
}

/* Вывод исходного справочника */
static void PrintBook(const char *title, PhoneEntry book[], int n) {
    printf("\n%s\n", title);
    printf("┌────┬──────────────┬──────────────┬──────────────┬──────────────┬──────────────────────┐\n");
    printf("│ N  │   Фамилия    │     Имя      │  Отчество    │   Телефон    │        Адрес         │\n");
    printf("├────┼──────────────┼──────────────┼──────────────┼──────────────┼──────────────────────┤\n");
    for (int i = 0; i < n; i++) {
        printf("│ %2d │ %-12s │ %-12s │ %-12s │ %-12s │ %-20s │\n",
            i + 1, book[i].surname, book[i].name, book[i].patronymic,
            book[i].phone, book[i].address);
    }
    printf("└────┴──────────────┴──────────────┴──────────────┴──────────────┴──────────────────────┘\n");
}

int main(void) {
    /* Исходные данные справочника */
    PhoneEntry book[] = {
        {"Петров",   "Иван",    "Сергеевич",  "+7-900-111", "ул. Ленина, 10"},
        {"Иванов",   "Алексей", "Петрович",   "+7-900-222", "ул. Мира, 5"},
        {"Сидоров",  "Дмитрий", "Иванович",   "+7-900-333", "пр. Победы, 22"},
        {"Козлов",   "Сергей",  "Александр.",  "+7-900-444", "ул. Ленина, 3"},
        {"Иванов",   "Борис",   "Дмитриевич", "+7-900-555", "ул. Мира, 12"},
        {"Андреев",  "Павел",   "Николаевич", "+7-900-666", "пр. Победы, 1"},
        {"Петров",   "Андрей",  "Викторович", "+7-900-777", "ул. Садовая, 7"},
        {"Козлов",   "Алексей", "Игоревич",   "+7-900-888", "ул. Садовая, 15"},
    };
    int n = sizeof(book) / sizeof(book[0]);
    g_book = book;
    int M, C;

    printf("=== Раздел 1.8: Индексация массивов (телефонный справочник) ===\n");

    /* Вывод исходного справочника */
    PrintBook("Исходный справочник:", book, n);

    /* Индексный массив 1: по Фамилия → Имя */
    int idx_surname[n];
    for (int i = 0; i < n; i++) idx_surname[i] = i;
    InsertSortIndex(idx_surname, n, cmp_idx_surname, &M, &C);
    PrintBookByIndex("Индекс 1: по Фамилия → Имя (возрастание)", book, idx_surname, n);
    printf("  Трудоёмкость сортировки индекса: M = %d, C = %d, M+C = %d\n", M, C, M + C);

    /* Индексный массив 2: по Адрес → Фамилия */
    int idx_address[n];
    for (int i = 0; i < n; i++) idx_address[i] = i;
    InsertSortIndex(idx_address, n, cmp_idx_address, &M, &C);
    PrintBookByIndex("Индекс 2: по Адрес → Фамилия (возрастание)", book, idx_address, n);
    printf("  Трудоёмкость сортировки индекса: M = %d, C = %d, M+C = %d\n", M, C, M + C);

    /* Проверка: исходный массив не изменился */
    PrintBook("Исходный справочник (не изменился):", book, n);

    /* Показываем содержимое индексных массивов */
    printf("\nИндексный массив (Фамилия→Имя): ");
    for (int i = 0; i < n; i++) printf("%d ", idx_surname[i]);
    printf("\n");

    printf("Индексный массив (Адрес→Фамилия): ");
    for (int i = 0; i < n; i++) printf("%d ", idx_address[i]);
    printf("\n");

    /* Бонус: двоичный поиск по двум ключам */
    printf("\n--- Двоичный поиск по фамилии (через индекс) ---\n");
    const char *search_surnames[] = {"Иванов", "Петров", "Козлов", "Смирнов"};
    for (int i = 0; i < 4; i++) {
        int Cs;
        int pos = BSearchIdxSurname(book, idx_surname, n, search_surnames[i], &Cs);
        if (pos >= 0) {
            int k = idx_surname[pos];
            printf("  Поиск \"%s\": найден (запись %d: %s %s), C = %d\n",
                search_surnames[i], k, book[k].surname, book[k].name, Cs);
        } else {
            printf("  Поиск \"%s\": не найден, C = %d\n", search_surnames[i], Cs);
        }
    }

    printf("\n--- Двоичный поиск по адресу (через индекс) ---\n");
    const char *search_addrs[] = {"ул. Мира, 5", "пр. Победы, 1", "ул. Пушкина, 1"};
    for (int i = 0; i < 3; i++) {
        int Cs;
        int pos = BSearchIdxAddress(book, idx_address, n, search_addrs[i], &Cs);
        if (pos >= 0) {
            int k = idx_address[pos];
            printf("  Поиск \"%s\": найден (запись %d: %s %s), C = %d\n",
                search_addrs[i], k, book[k].surname, book[k].name, Cs);
        } else {
            printf("  Поиск \"%s\": не найден, C = %d\n", search_addrs[i], Cs);
        }
    }

    printf("\n--- Выводы ---\n");
    printf("1. Индексация позволяет иметь несколько порядков сортировки без копирования записей.\n");
    printf("2. Исходный массив данных остаётся неизменным — перестановки только в индексном массиве.\n");
    printf("3. Сортировка индексного массива значительно дешевле, чем сортировка массива структур,\n");
    printf("   т.к. перемещаются только целые числа (индексы), а не целые записи.\n");
    printf("4. Двоичный поиск по индексу работает за O(log n) сравнений.\n");

    return 0;
}
