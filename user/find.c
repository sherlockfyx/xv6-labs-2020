#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char*
fmtname(char *path)
{
  static char buf[DIRSIZ+1];
  char *p;

  // Find first character after last slash.
  for(p = path+strlen(path); p >= path && *p != '/'; p--);
  p++;

  // Return blank-padded name.
  if(strlen(p) >= DIRSIZ) return p;
  memmove(buf, p, strlen(p));
  buf[strlen(p)] = '\0';
  //memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));
  return buf;
}

void
find(char *dirname, char *filename)
{
  char buf[512], *p;
  int fd;
  struct dirent de; //目录项
  struct stat st; //文件状态

  if((fd = open(dirname, 0)) < 0){//打开文件或目录
    fprintf(2, "find: cannot open %s\n", dirname);
    return;
  }

  if(fstat(fd, &st) < 0){//读取文件信息
    fprintf(2, "find: cannot stat %s\n", dirname);
    close(fd);
    return;
  }

  switch(st.type){
  case T_FILE:
    if(strcmp(fmtname(dirname), filename) == 0)
      printf("%s\n", dirname);
    break;

  case T_DIR:
    if(strlen(dirname) + 1 + DIRSIZ + 1 > sizeof buf){
      printf("find: path too long\n");
      break;
    }
    strcpy(buf, dirname);
    p = buf+strlen(buf);
    *p++ = '/';

    while(read(fd, &de, sizeof(de)) == sizeof(de)){
      if(de.inum == 0 || strcmp(de.name,"..") == 0 || strcmp(de.name,".") == 0)
        continue;
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;// buf = dirname/filename

      //recursively call find
      find(buf, filename);
    }
    break;
  }
  close(fd);
}

int
main(int argc, char *argv[]){
    if(argc != 3){
        fprintf(2, "Usage: find <dirname> <filename>\n");
        exit(0);
    }
    find(argv[1], argv[2]);

    exit(0);
}
