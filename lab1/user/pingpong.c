#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char* argv[])
{
  int parent[2], child[2];
  pipe(parent);//���������̹ܵ�
  pipe(child);//�����ӽ��̹ܵ�
  char msg[32];
  if (fork()) {
    write(parent[1], "ping", 4);
    read(child[0], msg, 4);
    wait(0);// �ȴ��ӽ��̽���
    fprintf(1, "%d: received %s\n", getpid(), msg);
  }// ������
  else {
    write(child[1], "pong", 4);
    read(parent[0], msg, 4);
    fprintf(1, "%d: received %s\n", getpid(), msg);
  }//�ӽ���
  exit(0);
}
