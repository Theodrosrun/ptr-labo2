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
        printf("%ld\n", elapsed_nsec);
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