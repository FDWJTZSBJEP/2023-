#include "kernel/types.h"
#include "user/user.h"
#define MAX 35

int main() {
  int parent[2], child[2];
  pipe(parent); // 创建父进程的管道
  pipe(child); // 创建子进程的管道
  if (fork()) {
    close(parent[0]); 
    close(child[1]); 
    int num;
    for (num = 2; num <= MAX; ++num) {
      write(parent[1], &num, sizeof(num)); // 向管道写入自然数
    }
    close(parent[1]); // 写入结束
    int prime;
    while (read(child[0], &prime, sizeof(prime)) > 0) {
      fprintf(1, "prime %d\n", prime);
    }
    close(child[0]); // 读取结束
    wait(0);
  }//父进程
  else {
    close(parent[1]); 
    close(child[0]); 
    int num;
    while (read(parent[0], &num, sizeof(num)) > 0) {
      int is_prime = 1;
      for (int i = 2; i * i <= num; ++i) {
        if (num % i == 0) {
          is_prime = 0;
          break;
        }
      }
      if (is_prime) {
        write(child[1], &num, sizeof(num)); // 向管道写入质数
      }
    }
    close(parent[0]); // 读取结束
    close(child[1]); // 写入结束
    exit(0);
  }//子进程
  exit(0);
}
