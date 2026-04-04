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

/* Сравнение по фамилии → имя через указатели */
static int cmp_surname(const void *a, const void *b) {
    const PhoneEntry *pa = *(const PhoneEntry **)a;
    const PhoneEntry *pb = *(const PhoneEntry **)b;
    int r = strcmp(pa->surname, pb->surname);
    if (r != 0) return r;
    return strcmp(pa->name, pb->name);
}

/* Сравнение по адресу → фамилия через указатели */
static int cmp_address(const void *a, const void *b) {
    const PhoneEntry *pa = *(const PhoneEntry **)a;
    const PhoneEntry *pb = *(const PhoneEntry **)b;
    int r = strcmp(pa->address, pb->address);
    if (r != 0) return r;
    return strcmp(pa->surname, pb->surname);
}

typedef int (*CmpFunc)(const void *, const void *);

/*
 * Заполнение индексного массива через указатели.
 *
 * idx — массив указателей PhoneEntry*.
 * Указатель p пробегает ячейки idx, указатель src —
 * элементы исходного массива book.
 * *p получает адрес элемента book[i] — именно это и есть
 * «присвоить указателю элемент исходного массива».
 */
static void fill_index(PhoneEntry **idx, PhoneEntry *book, int n) {
    PhoneEntry **p   = idx;
    PhoneEntry  *src = book;
    while (src < book + n)
        *p++ = src++;
}

/* InsertSort для индексного массива указателей с подсчётом M и C */
static void InsertSortIndex(PhoneEntry **idx, int n, CmpFunc cmp, int *M, int *C) {
    *M = 0; *C = 0;
    for (int i = 1; i < n; i++) {
        PhoneEntry *key = idx[i];
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

/* Двоичный поиск по фамилии через индексный массив указателей */
static int BSearchSurname(PhoneEntry **idx, int n, const char *surname, int *C) {
    *C = 0;
    int lo = 0, hi = n - 1;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        (*C)++;
        int r = strcmp(surname, idx[mid]->surname);
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

/* Двоичный поиск по адресу через индексный массив указателей */
static int BSearchAddress(PhoneEntry **idx, int n, const char *address, int *C) {
    *C = 0;
    int lo = 0, hi = n - 1;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        (*C)++;
        int r = strcmp(address, idx[mid]->address);
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

/* Вывод справочника через индексный массив указателей */
static void PrintBookByIndex(const char *title, PhoneEntry **idx, int n) {
    printf("\n%s\n", title);
    printf("┌────┬──────────────┬──────────────┬──────────────┬──────────────┬──────────────────────┐\n");
    printf("│  # │   Фамилия    │     Имя      │  Отчество    │   Телефон    │        Адрес         │\n");
    printf("├────┼──────────────┼──────────────┼──────────────┼──────────────┼──────────────────────┤\n");
    for (int i = 0; i < n; i++) {
        printf("│ %2d │ %-12s │ %-12s │ %-12s │ %-12s │ %-20s │\n",
            i + 1,
            idx[i]->surname, idx[i]->name, idx[i]->patronymic,
            idx[i]->phone,   idx[i]->address);
    }
    printf("└────┴──────────────┴──────────────┴──────────────┴──────────────┴──────────────────────┘\n");
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
    int M, C;

    printf("=== Раздел 1.8: Индексация массивов (телефонный справочник) ===\n");
    PrintBook("Исходный справочник:", book, n);

    /* Индексный массив 1: по Фамилия → Имя */
    PhoneEntry *idx_surname[n];
    fill_index(idx_surname, book, n);
    InsertSortIndex(idx_surname, n, cmp_surname, &M, &C);
    PrintBookByIndex("Индекс 1: по Фамилия → Имя (возрастание)", idx_surname, n);
    printf("  Трудоёмкость: M = %d, C = %d, M+C = %d\n", M, C, M + C);

    /* Индексный массив 2: по Адрес → Фамилия */
    PhoneEntry *idx_address[n];
    fill_index(idx_address, book, n);
    InsertSortIndex(idx_address, n, cmp_address, &M, &C);
    PrintBookByIndex("Индекс 2: по Адрес → Фамилия (возрастание)", idx_address, n);
    printf("  Трудоёмкость: M = %d, C = %d, M+C = %d\n", M, C, M + C);

    /* Проверка: исходный массив не изменился */
    PrintBook("Исходный справочник (не изменился):", book, n);

    /* Бонус: двоичный поиск */
    printf("\n--- Двоичный поиск по фамилии ---\n");
    const char *search_surnames[] = {"Иванов", "Петров", "Козлов", "Смирнов"};
    for (int i = 0; i < 4; i++) {
        int Cs;
        int pos = BSearchSurname(idx_surname, n, search_surnames[i], &Cs);
        if (pos >= 0)
            printf("  Поиск \"%s\": найден (%s %s), C = %d\n",
                search_surnames[i],
                idx_surname[pos]->surname, idx_surname[pos]->name, Cs);
        else
            printf("  Поиск \"%s\": не найден, C = %d\n", search_surnames[i], Cs);
    }

    printf("\n--- Двоичный поиск по адресу ---\n");
    const char *search_addrs[] = {"ул. Мира, 5", "пр. Победы, 1", "ул. Пушкина, 1"};
    for (int i = 0; i < 3; i++) {
        int Cs;
        int pos = BSearchAddress(idx_address, n, search_addrs[i], &Cs);
        if (pos >= 0)
            printf("  Поиск \"%s\": найден (%s %s), C = %d\n",
                search_addrs[i],
                idx_address[pos]->surname, idx_address[pos]->name, Cs);
        else
            printf("  Поиск \"%s\": не найден, C = %d\n", search_addrs[i], Cs);
    }

    printf("\n--- Выводы ---\n");
    printf("1. Индексация позволяет иметь несколько порядков сортировки без копирования записей.\n");
    printf("2. Исходный массив данных остаётся неизменным — переставляются только указатели в idx.\n");
    printf("3. Перемещаются указатели (PhoneEntry*), а не сами записи.\n");
    printf("4. Двоичный поиск по индексу работает за O(log n) сравнений.\n");

    return 0;
}
