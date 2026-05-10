/*
 * Раздел 1.7 — Сортировка массивов структур (телефонный справочник)
 * Структура: Фамилия, Имя, Отчество, Телефон, Адрес
 * Сортировка: InsertSort по составному ключу.
 * Направление сортировки меняется через инверсию функции сравнения Less.
 * Бонус (п.6*): двоичный поиск по старшей части ключа.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char surname[32];
    char name[32];
    char patronymic[32];
    char phone[16];
    char address[64];
} PhoneEntry;

/* Less по ключу Фамилия → Имя (возрастание) */
static int Less_surname_name_asc(const PhoneEntry *a, const PhoneEntry *b) {
    int r = strcmp(a->surname, b->surname);
    if (r != 0) return r < 0;
    return strcmp(a->name, b->name) < 0;
}

/* Less по ключу Фамилия → Имя (убывание) — инверсия предыдущей */
static int Less_surname_name_desc(const PhoneEntry *a, const PhoneEntry *b) {
    return Less_surname_name_asc(b, a);
}

/* Less по ключу Адрес → Фамилия (возрастание) */
static int Less_address_surname_asc(const PhoneEntry *a, const PhoneEntry *b) {
    int r = strcmp(a->address, b->address);
    if (r != 0) return r < 0;
    return strcmp(a->surname, b->surname) < 0;
}

typedef int (*LessFunc)(const PhoneEntry *, const PhoneEntry *);

static void InsertSortStruct(PhoneEntry arr[], int n, LessFunc less, int *M, int *C) {
    *M = 0; *C = 0;
    for (int i = 1; i < n; i++) {
        PhoneEntry key = arr[i];
        (*M)++;
        int j = i - 1;
        while (j >= 0) {
            (*C)++;
            if (less(&key, &arr[j])) {
                arr[j + 1] = arr[j];
                (*M)++;
                j--;
            } else break;
        }
        arr[j + 1] = key;
        (*M)++;
    }
}

/* Двоичный поиск по фамилии (старшая часть ключа сортировки) */
static int BSearchBySurname(PhoneEntry arr[], int n, const char *surname, int *C) {
    *C = 0;
    int lo = 0, hi = n - 1;
    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;
        (*C)++;
        int r = strcmp(surname, arr[mid].surname);
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

static void PrintBook(const char *title, PhoneEntry arr[], int n) {
    printf("\n%s\n", title);
    printf("┌────┬──────────────┬──────────────┬──────────────┬──────────────┬──────────────────────┐\n");
    printf("│ N  │   Фамилия    │     Имя      │  Отчество    │   Телефон    │        Адрес         │\n");
    printf("├────┼──────────────┼──────────────┼──────────────┼──────────────┼──────────────────────┤\n");
    for (int i = 0; i < n; i++) {
        printf("│ %2d │ %-12s │ %-12s │ %-12s │ %-12s │ %-20s │\n",
            i + 1, arr[i].surname, arr[i].name, arr[i].patronymic,
            arr[i].phone, arr[i].address);
    }
    printf("└────┴──────────────┴──────────────┴──────────────┴──────────────┴──────────────────────┘\n");
}

int main(void) {
    PhoneEntry book[] = {
        {"Петров",   "Иван",    "Сергеевич",  "+7-900-111", "ул. Ленина, 10"},
        {"Иванов",   "Алексей", "Петрович",   "+7-900-222", "ул. Мира, 5"},
        {"Сидоров",  "Дмитрий", "Иванович",   "+7-900-333", "пр. Победы, 22"},
        {"Козлов",   "Сергей",  "Александр.",  "+7-900-444", "ул. Ленина, 3"},
        {"Иванов",   "Борис",   "Дмитриевич", "+7-900-555", "ул. Мира, 12"},
    };
    int n = sizeof(book) / sizeof(book[0]);
    int M, C;

    PrintBook("Исходный справочник:", book, n);

    /* Ключ 1: Фамилия → Имя (возрастание) */
    PhoneEntry sorted1[n];
    memcpy(sorted1, book, sizeof(book));
    InsertSortStruct(sorted1, n, Less_surname_name_asc, &M, &C);
    PrintBook("Отсортировано по: Фамилия → Имя (возрастание)", sorted1, n);
    printf("  Трудоёмкость: M = %d, C = %d, M+C = %d\n", M, C, M + C);

    /* Ключ 2: Адрес → Фамилия (возрастание) */
    PhoneEntry sorted2[n];
    memcpy(sorted2, book, sizeof(book));
    InsertSortStruct(sorted2, n, Less_address_surname_asc, &M, &C);
    PrintBook("Отсортировано по: Адрес → Фамилия (возрастание)", sorted2, n);
    printf("  Трудоёмкость: M = %d, C = %d, M+C = %d\n", M, C, M + C);

    /* Смена направления: Фамилия → Имя (убывание) — через инверсию Less */
    PhoneEntry sorted3[n];
    memcpy(sorted3, book, sizeof(book));
    InsertSortStruct(sorted3, n, Less_surname_name_desc, &M, &C);
    PrintBook("Отсортировано по: Фамилия → Имя (убывание, через инверсию Less)", sorted3, n);
    printf("  Трудоёмкость: M = %d, C = %d, M+C = %d\n", M, C, M + C);

    /* п.6* — двоичный поиск по старшей части ключа (фамилия) */
    printf("\n--- Двоичный поиск по фамилии (в отсортированном по Фамилия→Имя) ---\n");
    const char *search_keys[] = {"Иванов", "Петров", "Сидоров", "Смирнов"};
    for (int i = 0; i < 4; i++) {
        int Cs;
        int idx = BSearchBySurname(sorted1, n, search_keys[i], &Cs);
        if (idx >= 0) {
            printf("  Поиск \"%s\": найден на позиции %d (%s %s), C = %d\n",
                search_keys[i], idx + 1, sorted1[idx].surname, sorted1[idx].name, Cs);
        } else {
            printf("  Поиск \"%s\": не найден, C = %d\n", search_keys[i], Cs);
        }
    }

    return 0;
}
