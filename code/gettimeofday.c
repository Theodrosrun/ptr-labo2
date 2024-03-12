#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NB_MESURES 30

int main (int argc, char **argv) {
    struct timespec tp;
    int i, j;

    clockid_t clocks[] = {CLOCK_REALTIME, CLOCK_MONOTONIC, CLOCK_PROCESS_CPUTIME_ID, CLOCK_THREAD_CPUTIME_ID};
    const char* clock_names[] = {"CLOCK_REALTIME", "CLOCK_MONOTONIC", "CLOCK_PROCESS_CPUTIME_ID", "CLOCK_THREAD_CPUTIME_ID"};
    int num_clocks = sizeof(clocks) / sizeof(clocks[0]);

    for (j = 0; j < num_clocks; ++j) {
        printf("Mesures avec %s\n", clock_names[j]);
        for (i = 0; i < NB_MESURES; ++i) {
            if (clock_gettime(clocks[j], &tp) == 0) {
                printf("%2d : %ld.%09ld\n", i, tp.tv_sec, tp.tv_nsec);
            } else {
                perror("clock_gettime");
                break;
            }
        }

        if (clock_getres(clocks[j], &tp) == 0) {
            printf("Résolution de %s : %ld.%09ld secondes\n", clock_names[j], tp.tv_sec, tp.tv_nsec);
        } else {
            perror("clock_getres");
        }
        printf("\n");
    }

    return EXIT_SUCCESS;
}
