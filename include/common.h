#ifndef __common_h__
#define __common_h__

#include <sys/time.h>
#include <sys/stat.h>
#include <assert.h>
#include <stddef.h>

double GetTime() {
  struct timeval t;
  // Either use nullptr or include #include <stddef.h> to have NULL definition
  int rc = gettimeofday(&t, NULL);
  assert(rc == 0);
  return (double) t.tv_sec + (double) t.tv_usec/1e6;
}

void Spin(int howlong) {
  double t = GetTime();
  while ((GetTime() - t) < (double) howlong)
    ;
}

#endif // __common_h__
