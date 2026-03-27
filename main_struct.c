/*
 * Раздел 1.7 — Сортировка массивов структур (телефонный справочник)
 * Структура: Фамилия, Имя, Отчество, Телефон, Адрес
 * Сортировка: InsertSort по составному ключу (не BubbleSort!)
 * Бонус: двоичный поиск по старшей части ключа
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Запись телефонного справочника (5 полей) */
typedef struct {
    char surname[32];     /* Фамилия */
    char name[32];        /* Имя */
    char patronymic[32];  /* Отчество */
    char phone[16];       /* Телефон */
    char address[64];     /* Адрес */
} PhoneEntry;

/* Сравнение по составному ключу: Фамилия → Имя (по возрастанию) */
static int cmp_surname_name(const PhoneEntry *a, const PhoneEntry *b) {
    int r = strcmp(a->surname, b->surname);
    if (r != 0) return r;
    return strcmp(a->name, b->name);
}

/* Сравнение по составному ключу: Адрес → Фамилия (по возрастанию) */
static int cmp_address_surname(const PhoneEntry *a, const PhoneEntry *b) {
    int r = strcmp(a->address, b->address);
    if (r != 0) return r;
    return strcmp(a->surname, b->surname);
}

/* Указатель на функцию сравнения */
typedef int (*CmpFunc)(const PhoneEntry *, const PhoneEntry *);

/* InsertSort для массива структур с подсчётом M и C */
static void InsertSortStruct(PhoneEntry arr[], int n, CmpFunc cmp, int *M, int *C) {
    *M = 0; *C = 0;
    for (int i = 1; i < n; i++) {
        PhoneEntry key = arr[i];
        (*M)++;
        int j = i - 1;
        while (j >= 0) {
            (*C)++;
            if (cmp(&arr[j], &key) > 0) {
                arr[j + 1] = arr[j];
                (*M)++;
                j--;
            } else break;
        }
        arr[j + 1] = key;
        (*M)++;
    }
}

/* Двоичный поиск по фамилии (старшая часть ключа) */
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

/* Вывод справочника в виде таблицы */
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
    /* Исходные данные справочника (8 записей) */
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

    printf("=== Раздел 1.7: Сортировка массивов структур (телефонный справочник) ===\n");

    /* Вывод исходного справочника */
    PrintBook("Исходный справочник:", book, n);

    /* Копия для сортировки по ключу 1: Фамилия + Имя */
    PhoneEntry sorted1[n];
    memcpy(sorted1, book, sizeof(book));
    InsertSortStruct(sorted1, n, cmp_surname_name, &M, &C);
    PrintBook("Отсортировано по: Фамилия → Имя (возрастание)", sorted1, n);
    printf("  Трудоёмкость: M = %d, C = %d, M+C = %d\n", M, C, M + C);

    /* Копия для сортировки по ключу 2: Адрес + Фамилия */
    PhoneEntry sorted2[n];
    memcpy(sorted2, book, sizeof(book));
    InsertSortStruct(sorted2, n, cmp_address_surname, &M, &C);
    PrintBook("Отсортировано по: Адрес → Фамилия (возрастание)", sorted2, n);
    printf("  Трудоёмкость: M = %d, C = %d, M+C = %d\n", M, C, M + C);

    /* Сортировка по убыванию (Фамилия + Имя, обратный порядок) */
    PhoneEntry sorted3[n];
    memcpy(sorted3, book, sizeof(book));
    /* Используем трюк: сортируем, потом разворачиваем */
    InsertSortStruct(sorted3, n, cmp_surname_name, &M, &C);
    for (int i = 0; i < n / 2; i++) {
        PhoneEntry tmp = sorted3[i];
        sorted3[i] = sorted3[n - 1 - i];
        sorted3[n - 1 - i] = tmp;
    }
    PrintBook("Отсортировано по: Фамилия → Имя (убывание)", sorted3, n);

    /* Бонус: двоичный поиск по фамилии */
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

    printf("\n--- Выводы ---\n");
    printf("1. Сортировка структур InsertSort аналогична сортировке чисел, но сравнение\n");
    printf("   выполняется по составному ключу (сначала по фамилии, затем по имени).\n");
    printf("2. При смене ключа сортировки порядок записей меняется полностью.\n");
    printf("3. Двоичный поиск по старшей части ключа (фамилии) работает корректно\n");
    printf("   и находит одну из записей с данной фамилией за O(log n) сравнений.\n");

    return 0;
}
