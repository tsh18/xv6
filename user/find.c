#include "kernel/types.h"
#include "user/user.h"
#include "kernel/fs.h"
#include "kernel/stat.h"

void find(char *path, char *filename) {
  int fd;
  struct stat st;
  struct dirent de;

  if ((fd = open(path, 0)) < 0) {
    fprintf(2, "find: cannot open %s\n", path);
    return;
  }

  if (fstat(fd, &st) < 0) {
    fprintf(2, "find: cannot stat %s\n", path);
    close(fd);
    return;
  }

  if (st.type == T_DIR) {
    while (read(fd, &de, sizeof(de)) == sizeof(de)) {
      if (de.inum == 0 || !strcmp(".", de.name) || !strcmp("..", de.name)) {
        continue;
      }
      char buf[512];
      strcpy(buf, path);
      char *p;
      p = buf + strlen(buf);
      *p++ = '/';
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;
      if (strcmp(de.name, filename) == 0) {
        printf("%s\n", buf);
      }
      find(buf, filename);
    }
  }
  close(fd);
}
int main(int argc, char *argv[]) {
  if (argc < 3) {
    fprintf(2, "usage: find path filename\n");
    exit(1);
  }
  find(argv[1], argv[2]);
  exit(0);
}