# Objectif

Permettre d’expérimenter l’interface des timers POSIX et les limites de Linux
en tant que système temps réel.

<br>

----

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

    // Boucle pour capturer le temps et imprimer les valeurs de temps stockées
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

Les résultats obtenus des deux versions du programme sur la DE1 mettent en évidence l'impact des opérations d'entrée/sortie, telles que `printf`, sur la mesure du temps. 

La première version montre une discontinuité des valeurs en micro secondes due à une latence accrue liée à l'exécution de `printf` dans la boucle, ce qui suggère que l'opération d'affichage peut introduire des délais variables et affecter la précision des mesures de temps. 

En contraste, la deuxième version, qui stocke d'abord toutes les valeurs avant de les imprimer, présente des intervalles de temps plus réguliers et plus court, soulignant ainsi que l'isolement des mesures de temps des opérations d'entrée/sortie peut offrir des résultats plus cohérents. 

Cette différence indique l'importance de minimiser les opérations coûteuses dans les boucles de mesure de temps pour obtenir une précision accrue, notamment en matière de granularité après la virgule, directement affectée par ces opérations. On notera que la précision est de l'ordre de de la micro seconde.

<br>

----

# 2. Horloges Posix

### Reprenez le programme gettimeofday.c du premier point, et modifiez-le pour utiliser les horloges POSIX

```c
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
```

Résultats obtenus :

| Mesure | CLOCK_REALTIME         | CLOCK_MONOTONIC        | CLOCK_PROCESS_CPUTIME_ID | CLOCK_THREAD_CPUTIME_ID |
|--------|------------------------|------------------------|-------------------------|-------------------------|
| 0      | 1565.507550070         | 1565.526111070         | 0.045814120              | 0.052286480             |
| 1      | 1565.509335260         | 1565.526438700         | 0.045988830              | 0.052443170             |
| 2      | 1565.511523990         | 1565.526755110         | 0.046157600              | 0.052593210             |
| 3      | 1565.511706410         | 1565.527075600         | 0.046473860              | 0.052743130             |
| 4      | 1565.511981250         | 1565.527388370         | 0.046664490              | 0.052891760             |
| 5      | 1565.512136490         | 1565.528653520         | 0.046835350              | 0.053064340             |
| 6      | 1565.512421850         | 1565.528987560         | 0.047003560              | 0.053238790             |
| 7      | 1565.512747630         | 1565.529299670         | 0.047188720              | 0.053391010             |
| 8      | 1565.513107060         | 1565.529615380         | 0.047404590              | 0.053540590             |
| 9      | 1565.513428060         | 1565.529931280         | 0.047571150              | 0.054818780             |
| 10     | 1565.513740060         | 1565.531265110         | 0.047734600              | 0.054978480             |
| 11     | 1565.514204190         | 1565.531592400         | 0.047898880              | 0.055128360             |
| 12     | 1565.515381130         | 1565.531903810         | 0.048084650              | 0.055276560             |
| 13     | 1565.515704330         | 1565.532220770         | 0.048247190              | 0.055426110             |
| 14     | 1565.516031850         | 1565.532531480         | 0.048409110              | 0.055573930             |
| 15     | 1565.516352500         | 1565.532841050         | 0.048571210              | 0.055722750             |
| 16     | 1565.516816040         | 1565.533154850         | 0.048733950              | 0.057268510             |
| 17     | 1565.517916730         | 1565.533465670         | 0.048896450              | 0.057432440             |
| 18     | 1565.518237450         | 1565.533772740         | 0.049058420              | 0.057584640             |
| 19     | 1565.518560020         | 1565.534083460         | 0.049219800              | 0.057735150             |
| 20     | 1565.518873270         | 1565.534400330         | 0.049380430              | 0.058031260             |
| 21     | 1565.519330070         | 1565.534709350         | 0.049544380              | 0.058205900             |
| 22     | 1565.520742580         | 1565.535017180         | 0.049705760              | 0.058355020             |
| 23     | 1565.521071200         | 1565.535331050         | 0.049872620              | 0.058519520             |
| 24     | 1565.521383400         | 1565.535640010         | 0.050054870              | 0.058669670             |
| 25     | 1565.521713700         | 1565.535949360         | 0.050218850              | 0.058819110             |
| 26     | 1565.522028340         | 1565.536264850         | 0.050382440              | 0.058967230             |
| 27     | 1565.522498730         | 1565.536578470         | 0.050544000              | 0.059114310             |
| 28     | 1565.523686360         | 1565.536891900         | 0.050706280              | 0.059399230             |
| 29     | 1565.524002340         | 1565.537206190         | 0.051025500              | 0.060153970             |

### Comparez-les en termes de résolution d’horloge. Commentez votre code et vos résultats.

#### CLOCK_REALTIME 
Représente l'heure actuelle en se basant sur l'heure mondiale, c'est-à-dire l'heure réelle. Elle est sujette à des modifications, comme les ajustements manuels de l'heure ou les secondes intercalaires, ce qui peut effectivement impacter les calculs de durée ou la planification d'événements nécessitant une précision temporelle stricte.

#### CLOCK_MONOTONIC
Mesure le temps écoulé depuis un point arbitraire dans le passé, qui ne change pas après son initialisation. Cette horloge n'est pas affectée par les modifications de l'heure système, ce qui la rend idéale pour mesurer des durées précises sans se soucier des ajustements d'heure.

#### CLOCK_PROCESS_CPUTIME_ID 
Mesure le temps CPU consommé par tous les threads d'un processus. Ce temps inclut le temps passé en mode utilisateur et en mode noyau, ce qui est utile pour analyser la consommation de ressources d'un processus.

#### CLOCK_THREAD_CPUTIME_ID 
Mesure le temps CPU utilisé par un thread spécifique du processus, en incluant le temps en mode utilisateur et en mode noyau. Cela permet d'évaluer précisément l'utilisation des ressources par des threads individuels.

On notera que la précision est de l'ordre de de la nano seconde pour toutes les clocks.

<br>

----

# 3. Développement : timers

```c
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>

void timer_handler(int signum) {
    static int count = 0;
    printf("timer expired %d times\n", ++count);
}

int main() {
    struct sigaction sa;
    struct itimerval timer;

    // Install timer_handler as the signal handler for SIGVTALRM.
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = &timer_handler;
    sigaction(SIGVTALRM, &sa, NULL);

    // Configure the timer to expire after 250 msec...
    timer.it_value.tv_sec = 0;
    timer.it_value.tv_usec = 250000;
    // ... and every 250 msec after that.
    timer.it_interval.tv_sec = 0;
    timer.it_interval.tv_usec = 250000;
    // Start a virtual timer. It counts down whenever this process is executing.
    setitimer(ITIMER_VIRTUAL, &timer, NULL);

    // Do busy work.
    while (1);
}
```

### Etudiez le code, Pouvez-vous expliquer comment il fonctionne ? (pour quitter utilisez CTRL+C, ou envoyez un signal au processus avec la commande kill)

1. **Configuration du Timer :** Le programme configure un timer virtuel (`ITIMER_VIRTUAL`) qui est déclenché pour expirer après 250 millisecondes, et ensuite, il est configuré pour expirer à nouveau toutes les 250 millisecondes de manière répétitive. Ce comportement est établi par les champs `it_value` pour la première expiration et `it_interval` pour les expirations périodiques.

2. **Association d'un Signal :** À chaque expiration du timer, le signal `SIGVTALRM` est émis. Ce signal est spécifiquement conçu pour être utilisé avec des timers virtuels et permet au programme de réagir à ces expirations de timer.

3. **Gestion du Signal :** Le programme définit une fonction de gestion de signal (`timer_handler`) en utilisant `sigaction` pour intercepter et gérer `SIGVTALRM`. Cette fonction est exécutée automatiquement à chaque fois que le signal est reçu, c'est-à-dire à chaque expiration du timer.

4. **Action du Gestionnaire de Signal :** Le gestionnaire de signal (`timer_handler`) incrémente un compteur à chaque fois qu'il est appelé, indiquant combien de fois le timer a expiré. Il imprime ensuite cette information, permettant de suivre le nombre d'expirations et donc la durée écoulée depuis le début du programme, à travers le nombre d'intervalles de 250 millisecondes comptés.

5. **Répétition :** Le timer continue d'expirer à intervalles réguliers (toutes les 250 millisecondes), et le programme reste en exécution indéfiniment dans une boucle infinie (`while (1);`), réagissant à chaque expiration par l'incrémentation et l'affichage du compteur.

Ce programme montre une utilisation des timers et des signaux pour exécuter des actions à intervalles réguliers, ici pour compter et afficher le nombre d'expirations du timer. Il démontre comment des mécanismes intégrés aux systèmes d'exploitation basés sur Unix/Linux peuvent être utilisés pour gérer le temps et les événements périodiques de manière précise et efficace.

<br>

----

# 4. Modification

### Ecrivez un programme signal_timer2.c qui prend en entrée le nombre de mesures à faire et un temps en microsecondes. Programmez un timer périodique CLOCK_REALTIME qui affiche sur la sortie standard le temps écoulé entre deux différentes occurences.

```c
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <inttypes.h>

int nbMeasures;
struct timespec lastTime;

void handler_signal(int signum) {
    struct timespec currentTime;
    if (clock_gettime(CLOCK_REALTIME, &currentTime) != 0) {
        perror("clock_gettime failed");
        exit(EXIT_FAILURE);
    }

    if (lastTime.tv_sec != 0 || lastTime.tv_nsec != 0) { // Skip the first signal
        long elapsed_nsec = (currentTime.tv_sec - lastTime.tv_sec) * 1000000000L + (currentTime.tv_nsec - lastTime.tv_nsec);
        printf("Elapsed time: %ld ns\n", elapsed_nsec);
    }
    lastTime = currentTime;

    if (--nbMeasures <= 0) {
        exit(EXIT_SUCCESS);
    }
}

int main(int argc, char *argv[]) {
    long usec, nsec;
    timer_t timer;
    struct sigevent event;
    struct itimerspec spec;

    if (argc != 3) {
        fprintf(stderr, "Usage: %s <number of measures> <time in us>\n", argv[0]);
        return EXIT_FAILURE;
    }

    nbMeasures = atoi(argv[1]);
    usec = atol(argv[2]);
    if (nbMeasures <= 0 || usec <= 0) {
        fprintf(stderr, "Both the number of measures and the time (in us) must be > 0\n");
        return EXIT_FAILURE;
    }

    nsec = usec * 1000; // Convert microseconds to nanoseconds

    // Configure the signal handler
    signal(SIGRTMIN, handler_signal);
    event.sigev_notify = SIGEV_SIGNAL;
    event.sigev_signo = SIGRTMIN;
    spec.it_interval.tv_sec = nsec / 1000000000;
    spec.it_interval.tv_nsec = nsec % 1000000000;
    spec.it_value = spec.it_interval; // Set the timer to expire after 'nsec' nanoseconds

    // Create and start the timer
    if (timer_create(CLOCK_REALTIME, &event, &timer) != 0) {
        perror("timer_create failed");
        return EXIT_FAILURE;
    }

    if (timer_settime(timer, 0, &spec, NULL) != 0) {
        perror("timer_settime failed");
        return EXIT_FAILURE;
    }

    // Initialize lastTime to zero
    lastTime.tv_sec = 0;
    lastTime.tv_nsec = 0;

    // Keep the program alive until all measures are done
    while (1) {
        pause(); // Wait for signals
    }

    return EXIT_SUCCESS;
}
```
### Etudiez le code, Pouvez-vous expliquer comment il fonctionne ? (pour quitter utilisez CTRL+C, ou envoyez un signal au processus avec la commande kill)

1. **Configuration du Timer** : Le programme configure un timer pour expirer après un intervalle de temps défini par l'utilisateur (en microsecondes), converti en nanosecondes pour la précision. Cet intervalle est à la fois initial (avec `it_value`) et périodique (avec `it_interval`), ce qui signifie que le timer expirera et émettra le signal associé à cet intervalle de manière répétitive.

2. **Association d'un Signal** : Le signal associé à l'expiration du timer est `SIGRTMIN`, un signal temps réel utilisé pour des applications nécessitant une granularité fine et un traitement prioritaire des signaux. Le programme utilise `sigevent` pour lier l'événement d'expiration du timer à l'émission de ce signal.

3. **Gestion du Signal** : Le programme définit un gestionnaire pour le signal `SIGRTMIN` (`handler_signal`). Ce gestionnaire est appelé automatiquement à chaque fois que le signal est émis, c'est-à-dire à chaque expiration du timer.

4. **Action du Gestionnaire de Signal** : Dans le gestionnaire de signal, le programme calcule le temps écoulé depuis la dernière expiration signalée en utilisant `clock_gettime` pour obtenir le temps actuel et le comparer au temps enregistré lors de la dernière expiration. Il affiche ensuite ce temps écoulé et décrémente un compteur de mesures, permettant au programme de suivre le nombre de signaux (et donc d'expirations du timer) traités.

5. **Répétition et Terminaison** : Le timer est configuré pour se répéter jusqu'à ce que le nombre spécifié de mesures soit atteint. Une fois ce nombre atteint, le gestionnaire de signal termine le programme avec `exit(EXIT_SUCCESS)`, ce qui est une illustration de la flexibilité offerte par ce mécanisme pour exécuter des actions spécifiques à intervalles réguliers ou après un certain temps.

Le programme en question utilise un timer pour mesurer des intervalles de temps précis, et à chaque expiration du timer, un signal est émis pour lequel un gestionnaire de signal spécifique (`handler_signal`) est défini.

<br>

----

# 5 Différence entre signal_timer.c et signal_timer2.c

Les différences principales entre les deux programmes sont centrées autour de trois axes majeurs : le type de timer utilisé, la gestion des signaux, et l'interface utilisateur pour la configuration du timer.

1. **Type de Timer :**
   - **signal_timer.c** utilise `ITIMER_VIRTUAL`, qui ne compte que le temps CPU consommé par le processus en mode utilisateur. Cela est spécifiquement utile pour mesurer la performance du processus en termes de temps CPU.
   - **signal_timer2.c** utilise `CLOCK_REALTIME` à travers l'API POSIX `timer_create()`, ce qui permet de mesurer le temps en temps réel, indépendamment de l'utilisation du CPU par le processus. Ce timer est plus versatile pour des applications nécessitant la mesure du temps réel, comme des temporisateurs ou des horloges.

2. **Gestion des Signaux :**
   - **signal_timer.c** utilise `sigaction()` pour configurer le gestionnaire de signal `SIGVTALRM`, permettant une certaine flexibilité dans la gestion des signaux, comme la prévention de l'interruption de certaines fonctions systèmes lors de la réception du signal.
   - **signal_timer2.c** emploie `signal()` avec `SIGRTMIN`, un signal en temps réel, offrant des capacités avancées telles que la queue de signaux et la priorisation, ce qui peut être essentiel pour les applications nécessitant une gestion précise des événements en temps réel.

<br>

----

# 6 Mesures

Maintenant que nous avons un programme implémentant un timer périodique, nous allons l’utiliser pour mesurer les capacités du système à générer des évènements périodiques.

#### Sur DE1 :

```bash
./signal_timer2 1000 500 > t500.dat
```

#### Sur machine de travail :

Récupérer vos fichiers localement avec secure copy :

```bash
scp root@192.168.0.2:/root/t500.dat .
```
Compilez le programme fourni sur votre machine de travail :

```bash
gcc -O2 summary1.c -o summary -lm
```

Et utilisez-le de la manière suivante :

```bash
./summary < t500.dat
```

<br>

### Commentez les résultats et répétez l’expérience avec un intervalle de 250us, 1ms et 10ms. A partir des résultats précédents, créez les histogrammes des fréquences des données récupérées. Vous êtes libre d’utiliser votre environment préféré (python, excel, octave, etc...).


<br>

----

# 7 Perturbations - Répétez l’expérience avec un timer de 1ms en perturbant le système des manières suivantes :

### En le lançant avec différentes valeurs de nice

Décrivez et commentez les résultats obtenus :



<br>

### En le lançant avec ./cpu_loop & ./timer 1000 1000

Décrivez et commentez les résultats obtenus :



<br>

### En le lançant et en effectuant d’autres opérations avec le système en parallèle

Décrivez et commentez les résultats obtenus :



<br>

### En le lançant en même temps que vous recevez un ping depuis une autre machine (demandez de l’aide à un autre groupe)

Décrivez et commentez les résultats obtenus :

