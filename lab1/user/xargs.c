#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char* argv[]) {
  char buf[512], buf2[32][32];
  char* pass[32];
  for (int i = 0; i < 32; ++i) {
    pass[i] = buf2[i];
  }// 初始化pass数组，用于存储参数
  for (int i = 1; i < argc; ++i) {
    strcpy(buf2[i - 1], argv[i]);
  } // 将命令行参数拷贝到buf2数组中
  int m;
  char* p = buf;
  int pos = argc - 1;
  char* c = pass[pos];
  pass[pos + 1] = 0;// 设置pass数组的末尾为NULL，作为exec函数的参数列表结束标志
  m = read(0, buf, 512); // 从标准输入读取数据，并解析参数
  do {
    p = buf;
    if (m < 0) {
      exit(1);
    }
    // 遍历读取到的数据
    while (p < buf + m) {
      if (*p == ' ' || *p == '\n') {
        *c = '\0';
        if (fork()) {
          wait(0);
        }  // 创建子进程，执行参数对应的命令
        else {
          exec(pass[0], pass);
        }
        ++p;
        c = pass[pos];
      }
      else {
        *c++ = *p++;
      }
    }
  } 
  while ((m = read(0, buf, 512)) > 0);
  if (m < 0) {
    fprintf(2, "xargs: read error\n");
    exit(1);
  }
  exit(0);
}
