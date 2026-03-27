#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "sorting.h"

#define MAX_STEPS 20

static int knuth_steps(int n, int steps[MAX_STEPS]) {
    int h = 1, cnt = 0;
    while (h <= n / 3) h = h * 3 + 1;
    while (h >= 1) { steps[cnt++] = h; h /= 3; }
    return cnt;
}

static int hibbard_steps(int n, int steps[MAX_STEPS]) {
    /* h(k) = 2^k - 1: 1, 3, 7, 15, 31, 63, 127, 255, ... */
    int tmp[MAX_STEPS], cnt = 0;
    int h = 1;
    while (h < n) { tmp[cnt++] = h; h = h * 2 + 1; }
    /* Reverse to descending order */
    for (int i = 0; i < cnt; i++) steps[i] = tmp[cnt - 1 - i];
    return cnt;
}

static void steps_str_asc(int ns, int steps[MAX_STEPS], char out[64]) {
    int pos = 0;
    for (int i = ns - 1; i >= 0 && pos < 58; i--)
        pos += snprintf(out + pos, 64 - pos, "%d%s", steps[i], i > 0 ? "," : "");
}

int main(void) {
    srand(time(NULL));
    int sizes[] = {100, 200, 300, 400, 500};

    /* ===================================================================
     * Таблица 1: Трудоемкость метода Шелла (ТОЧНО ПО ТЗ)
     * Только СЛУЧАЙНЫЙ массив для Insert и Shell
     * =================================================================== */
    printf("┌──────────────────────────────────────────────────────────────────────────────────┐\n");
    printf("│                        Трудоемкость метода Шелла                                 │\n");
    printf("├──────┬────────────────────────────────┬──────────────┬──────────────┤\n");
    printf("│  n   │ h1 ... hm по формуле Д.Кнута   │ Insert Мф+Сф │  Shell Мф+Сф │\n");
    printf("├──────┼────────────────────────────────┼──────────────┼──────────────┤\n");

    int Shell_rnd[5]; /* сохраняем для таблицы 2 */
    int knuth_ns[5];
    int knuth_st[5][MAX_STEPS];
    char knuth_str[5][64];

    for (int s = 0; s < 5; s++) {
        int n = sizes[s];
        int steps[MAX_STEPS];
        int nsteps = knuth_steps(n, steps);
        knuth_ns[s] = nsteps;
        for (int i = 0; i < nsteps; i++) knuth_st[s][i] = steps[i];
        char ststr[64] = {0};
        steps_str_asc(nsteps, steps, ststr);
        snprintf(knuth_str[s], 64, "%s", ststr);

        int Msd, Csd, Msr, Csr, Msi, Csi;
        int Mid, Cid, Mir, Cir, Mii, Cii;
        RunShellSort(n, nsteps, steps, &Msd, &Csd, &Msr, &Csr, &Msi, &Csi);
        RunSort(n, InsertSort, &Mid, &Cid, &Mir, &Cir, &Mii, &Cii);
        Shell_rnd[s] = Msr + Csr;

        printf("│ %4d │ %-30s │ %12d │ %12d │\n",
            n, ststr, Mir + Cir, Msr + Csr);
    }
    printf("└──────┴────────────────────────────────┴──────────────┴──────────────┘\n");

    /* ===================================================================
     * Таблица 2: Исследование трудоемкости — Кнут vs Хиббард (ТОЧНО ПО ТЗ)
     * =================================================================== */
    printf("\n┌──────────────────────────────────────────────────────────────────────────────────────────────────┐\n");
    printf("│                       Исследование трудоемкости метода Шелла                                    │\n");
    printf("├──────┬──────────────────┬─────────────┬─────────────────────────────┬─────────────┤\n");
    printf("│  n   │h1...hm по Кнуту  │ Shell Мф+Сф │h1...hm по другой формуле    │ Shell Мф+Сф │\n");
    printf("├──────┼──────────────────┼─────────────┼─────────────────────────────┼─────────────┤\n");
    for (int s = 0; s < 5; s++) {
        int n = sizes[s];
        int hib_steps[MAX_STEPS];
        int hib_ns = hibbard_steps(n, hib_steps);
        char hib_str[64] = {0};
        steps_str_asc(hib_ns, hib_steps, hib_str);

        int Msd, Csd, Msr, Csr, Msi, Csi;
        RunShellSort(n, hib_ns, hib_steps, &Msd, &Csd, &Msr, &Csr, &Msi, &Csi);

        printf("│ %4d │ %-16s │ %11d │ %-27s │ %11d │\n",
            n, knuth_str[s], Shell_rnd[s], hib_str, Msr + Csr);
    }
    printf("└──────┴──────────────────┴─────────────┴─────────────────────────────┴─────────────┘\n");

    /* ===================================================================
     * Таблица 3: Разбор M и C отдельно (Shell с шагами Кнута)
     * =================================================================== */
    printf("\n┌───────────────────────────────────────────────────────────────────────────────────────────┐\n");
    printf("│                    ShellSort (Кнут): Разбор M и C по типам массивов                       │\n");
    printf("├──────┬──────────┬──────────┬──────────┬──────────┬──────────┬──────────┤\n");
    printf("│  N   │ Мф(убыв) │ Сф(убыв) │ Мф(случ) │ Сф(случ) │ Мф(возр) │ Сф(возр) │\n");
    printf("├──────┼──────────┼──────────┼──────────┼──────────┼──────────┼──────────┤\n");
    for (int s = 0; s < 5; s++) {
        int n = sizes[s];
        int Msd, Csd, Msr, Csr, Msi, Csi;
        RunShellSort(n, knuth_ns[s], knuth_st[s], &Msd, &Csd, &Msr, &Csr, &Msi, &Csi);
        printf("│ %4d │ %8d │ %8d │ %8d │ %8d │ %8d │ %8d │\n",
            n, Msd, Csd, Msr, Csr, Msi, Csi);
    }
    printf("└──────┴──────────┴──────────┴──────────┴──────────┴──────────┴──────────┘\n");

    /* Выводы */
    printf("\nВывод: ShellSort значительно быстрее InsertSort: O(n^1.2) vs O(n²) на случайных данных.\n");
    printf("Вывод: Последовательность Кнута (1,4,13,40,121...) даёт хорошие результаты на практике.\n");
    printf("Вывод: Последовательность Хиббарда (2^k-1) сравнима по эффективности с формулой Кнута.\n");

    return 0;
}
