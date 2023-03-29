#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
  char c = 'a';
  int pc[2], cp[2];
  pipe(pc);
  pipe(cp);
  int pid = fork();
  if (pid == 0) {
    close(pc[1]);
    if (read(pc[0], &c, 1) > 0) {
      printf("%d: received ping\n", getpid());
    }
    close(pc[0]);
    close(cp[0]);
    if (write(cp[1], &c, 1) > 0) {
      close(cp[1]);
      exit(0);
    }
    exit(1);
  } else if (pid < 0) {
    printf("pingpong: fork failed\n");
    exit(1);
  }
  close(pc[0]);
  write(pc[1], &c, 1);
  close(pc[1]);
  close(cp[1]);
  wait(0);
  if (read(cp[0], &c, 1) > 0) {
    printf("%d: received pong\n", getpid());
  }
  close(cp[0]);
  exit(0);
}