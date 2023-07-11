#include "kernel/types.h"
#include "user/user.h"
#define MAX 35

int main() {
  int parent[2], child[2];
  pipe(parent); // ���������̵Ĺܵ�
  pipe(child); // �����ӽ��̵Ĺܵ�
  if (fork()) {
    close(parent[0]); 
    close(child[1]); 
    int num;
    for (num = 2; num <= MAX; ++num) {
      write(parent[1], &num, sizeof(num)); // ��ܵ�д����Ȼ��
    }
    close(parent[1]); // д�����
    int prime;
    while (read(child[0], &prime, sizeof(prime)) > 0) {
      fprintf(1, "prime %d\n", prime);
    }
    close(child[0]); // ��ȡ����
    wait(0);
  }//������
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
        write(child[1], &num, sizeof(num)); // ��ܵ�д������
      }
    }
    close(parent[0]); // ��ȡ����
    close(child[1]); // д�����
    exit(0);
  }//�ӽ���
  exit(0);
}
