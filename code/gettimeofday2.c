#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>

#define NB_MESURES 30

int main (int argc, char **argv)
{
    struct timeval tv[NB_MESURES];
    int i;

    // Première boucle pour capturer le temps
    for (i = 0; i < NB_MESURES; ++i) {
        gettimeofday(&tv[i], NULL);
    }

    // Deuxième boucle pour imprimer les valeurs de temps stockées
    for (i = 0; i < NB_MESURES; ++i) {
        printf("%2d : %ld.%06ld\n", i, tv[i].tv_sec, tv[i].tv_usec);
    }

    return EXIT_SUCCESS;
}