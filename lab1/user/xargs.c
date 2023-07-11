#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char* argv[]) {
  char buf[512], buf2[32][32];
  char* pass[32];
  for (int i = 0; i < 32; ++i) {
    pass[i] = buf2[i];
  }// ��ʼ��pass���飬���ڴ洢����
  for (int i = 1; i < argc; ++i) {
    strcpy(buf2[i - 1], argv[i]);
  } // �������в���������buf2������
  int m;
  char* p = buf;
  int pos = argc - 1;
  char* c = pass[pos];
  pass[pos + 1] = 0;// ����pass�����ĩβΪNULL����Ϊexec�����Ĳ����б������־
  m = read(0, buf, 512); // �ӱ�׼�����ȡ���ݣ�����������
  do {
    p = buf;
    if (m < 0) {
      exit(1);
    }
    // ������ȡ��������
    while (p < buf + m) {
      if (*p == ' ' || *p == '\n') {
        *c = '\0';
        if (fork()) {
          wait(0);
        }  // �����ӽ��̣�ִ�в�����Ӧ������
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
