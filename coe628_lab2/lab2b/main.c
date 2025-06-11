#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define N_REPS 50
#define DEFAULT_SLOWDOWN 10000

int main(int argc, char * argv[]) {
    int i;
    int slow_down = DEFAULT_SLOWDOWN;

    if (argc == 1) {
        fprintf(stderr, "Usage: %s string [delay]\n", argv[0]);
        return 1;
    }
    if (argc >= 3) {
        slow_down = atoi(argv[2]);
    }

    for (i = 0; i < N_REPS; i++) {
        char * cp = argv[1];
        
        //Creates directory making sure process waits until it has done so, and once the directory is created the process can move to the next part
        //I used mkdri .mutex 2>/dev/null and rmdir .mutex 2>/dev/null instead of mkdir junk and rmdir junk due to file creating errors and to prevent other errors due to multi-process executions
        while (system("mkdir .mutex 2>/dev/null") != 0); 
        
        while (*cp) {
            printf("%c", *cp++);
            fflush(stdout);
            usleep(random() % slow_down);
        }
        
        //Removes directory 
        system("rmdir .mutex 2>/dev/null"); //
        
        usleep(5000);
    }
    return EXIT_SUCCESS;
}