#include "kernel/param.h"
#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
  char c;
  char *arguments[MAXARG]; // 存exec命令行参数
  int index = 0; // arguments的索引
  char argument[512]; // 存标准输出的字符
  int p = 0; // argument's index;
  int tmp_index = 0; // 标准输出中每个字符串的起始位置

  for (int i = 1; i < argc; i++) {
    arguments[index] = argv[i];
    index++;
  }

  while (read(0, &c, sizeof(c)) > 0) {
    if (c == ' ') {
      argument[p] = 0;
      p++;
      arguments[index] = &argument[tmp_index];
      index++;
      tmp_index = p;
    } else if (c == '\n') {
      argument[p] = 0;
      arguments[index] = &argument[tmp_index];
      tmp_index = 0;
      int pid = fork();
      if (pid == 0) {
        exec(argv[1], arguments);
        fprintf(2, "xargs: exec error\n");
      } else if (pid < 0) {
        fprintf(2, "xargs: fork failed\n");
      }
      wait(0);
      index = argc - 1;
      p = 0;
    } else {
      argument[p] = c;
      p++;
    }
  }
  exit(0);
}