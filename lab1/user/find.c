#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char* name(char* path)
{
  static char buf[DIRSIZ + 1];
  char* p;
  for (p = path + strlen(path); p >= path && *p != '/'; p--);
  p++;
  // �������һ��б�ܺ�ĵ�һ���ַ�
  if (strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p)); // ��p����strlen(p)���ַ���buf
  memset(buf + strlen(p), '\0', DIRSIZ - strlen(p));// ���� '\0'��buf+strlen(p)��DIRSIZ - strlen(p)λ��
  return buf;// ���ؿո������ļ���
}// ��·������ȡ�ļ���

void find(char* path, char* fileName) {
  int fd = open(path, 0);
  char buf[512], * p;
  struct dirent de;
  if (fd < 0) {
    return;
  }
  struct stat st; 
  if (stat(path, &st) < 0) {
    close(fd);
    return;
  } // ͨ��statϵͳ���û�ȡָ��·�����ļ���Ŀ¼��״̬����������洢��struct stat�ṹ�����st�С������ȡ״̬ʧ�ܣ���ر��ļ������������ء�
  switch (st.type)
  {
  case T_FILE: //�ļ������
    if (strcmp(name(path), fileName) == 0) {
      fprintf(1, "%s\n", path); //�Ƚ��Ƿ���ͬ������ͬ�����
    }
    break;
  case T_DIR: // Ŀ¼�����
    if (strlen(path) + 1 + DIRSIZ + 1 > sizeof buf) {
      break;
    }
    strcpy(buf, path);
    p = buf + strlen(buf);
    *p++ = '/';
    while (read(fd, &de, sizeof(de)) == sizeof(de)) {
    // ͨ��readϵͳ���ö�ȡĿ¼�е�ÿ��Ŀ¼�����Ŀ¼��洢��struct dirent�ṹ�����de�С�
      if (de.inum == 0)
        continue;
      if (strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0) 
        continue;
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;
      // ������Ŀ¼·��������ǰĿ¼·����Ŀ¼�������ƴ��������������Ŀ¼��·����
      if (stat(buf, &st) < 0) {
        continue;
      }
      find(buf, fileName);// // �ݹ����find������������Ŀ¼�µ��ļ�
    }
    break;
  }
  close(fd);
}// �ݹ����ָ��Ŀ¼�µ��ļ�������ӡƥ����ļ�·��

int main(int argc, char* argv[]) {
  if (argc < 3) {
    fprintf(2, "Usage: find [dir] [filename]\n");
    exit(1);
  }// �ж������Ƿ�Ϸ�
  find(argv[1], argv[2]);
  exit(0);
}
