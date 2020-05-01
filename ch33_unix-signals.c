#include <signal.h>
#include <stdio.h>

void handle(int arg) { printf("stop waking me up...\n"); }

/* $ ./build/ch33_unix-signals.o &
 * [3] 36705
 * $ kill -HUP 36705
 * stop wakin’ me up...
 */

int main(int argc, char *argv[]) {
  signal(SIGHUP, handle);
  while (1)
    ;
  return 0;
}
