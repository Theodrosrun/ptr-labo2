# 1. Développement

### Exécutez le programme gettimeofday.c sur la DE1

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>

#define NB_MESURES 30

int main (int argc, char **argv)
{
    struct timeval tv;
    int i;

    for (i = 0; i < NB_MESURES; ++i) {
        gettimeofday(&tv, NULL);
        printf("%2d : %ld.%06ld\n", i, tv.tv_sec, tv.tv_usec);
    }

    return EXIT_SUCCESS;
}
```

Résultats obtenus :

```batch
 0 : 211.267664
 1 : 211.269288
 2 : 211.269302
 3 : 211.269311
 4 : 211.269319
 5 : 211.269327
 6 : 211.269335
 7 : 211.269343
 8 : 211.269351
 9 : 211.269359
 9 : 671.483415
10 : 671.483423
11 : 671.483431
12 : 671.483439
13 : 671.483448
14 : 671.483456
15 : 671.483464
16 : 671.483472
17 : 671.483480
18 : 671.483488
19 : 671.483496
20 : 671.483504
21 : 671.483512
22 : 671.483520
23 : 671.483528
24 : 671.483536
25 : 671.483544
26 : 671.483552
27 : 671.483560
28 : 671.483568
29 : 671.483576
```



### Exécutez le programme gettimeofday2.c sur la DE1

```c
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

```

Résultats obtenus :

```batch
 0 : 1354.024203
 1 : 1354.024210
 2 : 1354.024215
 3 : 1354.024221
 4 : 1354.024227
 5 : 1354.024233
 6 : 1354.024238
 7 : 1354.024244
 8 : 1354.024250
 9 : 1354.024256
10 : 1354.024261
11 : 1354.024267
12 : 1354.024273
13 : 1354.024278
14 : 1354.024284
15 : 1354.024290
16 : 1354.024295
17 : 1354.024301
18 : 1354.024307
19 : 1354.024312
20 : 1354.024318
21 : 1354.024323
22 : 1354.024329
23 : 1354.024335
24 : 1354.024340
25 : 1354.024346
26 : 1354.024352
27 : 1354.024358
28 : 1354.024363
29 : 1354.024369
```



### Comparez les résultats avec les précédents et expliquez pourquoi il y a des différences. Quelle est la granularité, ou précision, de la fonction gettimeofday()

Les résultats obtenus des deux versions du programme sur la DE1 mettent en évidence l'impact significatif des opérations d'entrée/sortie, telles que `printf`, sur la mesure du temps. La première version montre une discontinuité notable des valeurs en micro secondes due à une latence accrue liée à l'exécution de `printf` dans la boucle, ce qui suggère que l'opération d'affichage peut introduire des délais variables et affecter la précision des mesures de temps. En contraste, la deuxième version, qui stocke d'abord toutes les valeurs avant de les imprimer, présente des intervalles de temps plus réguliers et précis, soulignant ainsi que l'isolement des mesures de temps des opérations d'entrée/sortie peut offrir des résultats plus fiables et cohérents. Cette différence indique l'importance de minimiser les opérations coûteuses dans les boucles de mesure de temps pour obtenir une précision accrue, notamment en matière de granularité après la virgule, directement affectée par ces opérations.

<br>

----

# 2. Horloges Posix

### Reprenez le programme gettimeofday.c du premier point, et modifiez-le pour utiliser les horloges POSIX

```c
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
```

Résultats obtenus :

```batch

```

### Comparez-les en termes de résolution d’horloge. Commentez votre code et vos résultats.