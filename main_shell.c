#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <string.h>
#include <stdarg.h>
#include "sorting.h"

#define MAX_STEPS 20

static int knuth_steps(int n, int steps[MAX_STEPS]) {
    int h = 1, cnt = 0;
    while (h <= n / 3) h = h * 3 + 1;
    while (h >= 1) { steps[cnt++] = h; h /= 3; }
    return cnt;
}

static void steps_str_asc(int ns, int steps[MAX_STEPS], char out[32]) {
    int pos = 0;
    for (int i = ns - 1; i >= 0 && pos < 28; i--)
        pos += snprintf(out + pos, 32 - pos, "%d%s", steps[i], i > 0 ? "," : "");
}

int main(void) {
    srand(time(NULL));
    int sizes[] = {100, 200, 300, 400, 500};

    printf("┌──────┬──────────────────────┬──────┬────────────┬──────────────┬──────────────┐\n");
    printf("│  N   │        Шаги          │ Тип  │  O(n^1.2)  │  Shell M+C   │  Insert M+C  │\n");
    printf("├──────┼──────────────────────┼──────┼────────────┼──────────────┼──────────────┤\n");

    for (int s = 0; s < 5; s++) {
        int n = sizes[s];
        int steps[MAX_STEPS];
        int nsteps = knuth_steps(n, steps);
        char ststr[32];
        steps_str_asc(nsteps, steps, ststr);
        int theory = (int)pow((double)n, 1.2);

        int Msdec, Csdec, Msrnd, Csrnd, Msinc, Csinc;
        int Midec, Cidec, Mirnd, Cirnd, Miinc, Ciinc;
        RunShellSort(n, nsteps, steps, &Msdec, &Csdec, &Msrnd, &Csrnd, &Msinc, &Csinc);
        RunSort(n, InsertSort, &Midec, &Cidec, &Mirnd, &Cirnd, &Miinc, &Ciinc);

        printf("│ %4d │ %-20s │ inc  │ %10d │ %12d │ %12d │\n",
            n, ststr, theory, Msinc + Csinc, Miinc + Ciinc);
        printf("│      │                      │ dec  │            │ %12d │ %12d │\n",
            Msdec + Csdec, Midec + Cidec);
        printf("│      │                      │ rnd  │            │ %12d │ %12d │\n",
            Msrnd + Csrnd, Mirnd + Cirnd);
        if (s < 4)
            printf("├──────┼──────────────────────┼──────┼────────────┼──────────────┼──────────────┤\n");
    }
    printf("└──────┴──────────────────────┴──────┴────────────┴──────────────┴──────────────┘\n");
    return 0;
}
