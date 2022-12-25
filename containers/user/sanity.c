#include "kernel/types.h"
#include "kernel/stat.h"
#include "user.h"

int main(int argc, char *argv[]) {
// Uncomment the code once you have set_pid_ns and ps implemented.
  int namespace;
  if (fork() == 0) {
    namespace = set_pid_ns(1);
    printf(1, "namespace=%d\n", namespace);
    ps();
  } else {
    wait();
    printf(1, "\nnamespace=0\n");
    ps();
  }
  exit();
}
