#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_SIGNAL 31

void my_handler(int nsig)
{ 
    printf("Receive signal %s\n", strsignal(nsig));
    if (nsig != SIGKILL || nsig != SIGSTOP) {
        if (signal(nsig, my_handler) == SIG_ERR) {
            exit(nsig);
        }
    }
}

int main(void)
{ 
    for (int i = 1; i <= MAX_SIGNAL; ++i) {
        if (i == SIGKILL || i == SIGSTOP) {
            continue;
        }
        if (signal(i, my_handler) == SIG_ERR) {
            exit(i);
        }
    }
    
    for(;;);
    return 0;
}