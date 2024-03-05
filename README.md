### Exécutez le programme sur la DE1

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



### Exécutez le programme sur la DE1
