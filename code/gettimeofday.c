#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NB_MESURES 30

int main (int argc, char **argv)
{
    struct timespec tp;
    int i;

    // Exemple avec CLOCK_MONOTONIC
    printf("Mesures avec CLOCK_MONOTONIC\n");
    for (i = 0; i < NB_MESURES; ++i) {
        clock_gettime(CLOCK_MONOTONIC, &tp);
        printf("%2d : %ld.%09ld\n", i, tp.tv_sec, tp.tv_nsec);
    }

    // Afficher la résolution de l'horloge
    if (clock_getres(CLOCK_MONOTONIC, &tp) == 0) {
        printf("Résolution de CLOCK_MONOTONIC : %ld.%09ld secondes\n", tp.tv_sec, tp.tv_nsec);
    } else {
        perror("clock_getres");
    }

    return EXIT_SUCCESS;
}
