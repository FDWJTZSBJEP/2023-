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
  // 查找最后一个斜杠后的第一个字符
  if (strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p)); // 从p复制strlen(p)个字符到buf
  memset(buf + strlen(p), '\0', DIRSIZ - strlen(p));// 复制 '\0'到buf+strlen(p)的DIRSIZ - strlen(p)位置
  return buf;// 返回空格填充的文件名
}// 从路径中提取文件名

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
  } // 通过stat系统调用获取指定路径的文件或目录的状态，并将结果存储在struct stat结构体变量st中。如果获取状态失败，则关闭文件描述符并返回。
  switch (st.type)
  {
  case T_FILE: //文件的情况
    if (strcmp(name(path), fileName) == 0) {
      fprintf(1, "%s\n", path); //比较是否相同，若相同则输出
    }
    break;
  case T_DIR: // 目录的情况
    if (strlen(path) + 1 + DIRSIZ + 1 > sizeof buf) {
      break;
    }
    strcpy(buf, path);
    p = buf + strlen(buf);
    *p++ = '/';
    while (read(fd, &de, sizeof(de)) == sizeof(de)) {
    // 通过read系统调用读取目录中的每个目录项，并将目录项存储在struct dirent结构体变量de中。
      if (de.inum == 0)
        continue;
      if (strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0) 
        continue;
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;
      // 构建子目录路径：将当前目录路径与目录项的名称拼接起来，构建子目录的路径。
      if (stat(buf, &st) < 0) {
        continue;
      }
      find(buf, fileName);// // 递归调用find函数，查找子目录下的文件
    }
    break;
  }
  close(fd);
}// 递归查找指定目录下的文件，并打印匹配的文件路径

int main(int argc, char* argv[]) {
  if (argc < 3) {
    fprintf(2, "Usage: find [dir] [filename]\n");
    exit(1);
  }// 判断输入是否合法
  find(argv[1], argv[2]);
  exit(0);
}
