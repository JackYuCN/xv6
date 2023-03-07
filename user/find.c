#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

/**
 * @brief fmtname -- return the format name of a path, i.e the characters after the last '/'
 * 
 * @param path -- path string
 * @return char* -- format string of the path
 */
char*
fmtname(char *path)
{
  char *p;

  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;

  return p;
}

/**
 * @brief find -- a simple version of Unix find program
 * 
 * @param rootpath -- root path to search for
 * @param filename -- filename to be found
 */
void 
find(char *rootpath, char *filename)
{
  char buf[512];  
  char *p;
  struct dirent de;
  struct stat st;
  int fd;

  if((fd = open(rootpath, 0)) < 0){
    fprintf(2, "find: cannot open %s\n", rootpath);
    return;
  }

  if(fstat(fd, &st) < 0){
    fprintf(2, "find: cannot stat %s\n", rootpath);
    close(fd);
    return;
  }

  switch(st.type) {
    case T_FILE:
      if (strcmp(fmtname(rootpath), filename) == 0) {
        printf("%s\n", rootpath);
      }
      break;

    case T_DIR:
      if(strlen(rootpath) + 1 + DIRSIZ + 1 > sizeof(buf)){
        printf("find: path too long\n");
        break;
      }
      memset(buf, 0, sizeof(buf));
      strcpy(buf, rootpath);
      p = buf + strlen(buf);
      *p++ = '/';
      while(read(fd, &de, sizeof(de)) == sizeof(de)){
        if(de.inum == 0 || strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0)
          continue;
        memmove(p, de.name, DIRSIZ);
        find(buf, filename);
      }
      break;
  }
  close(fd);
}

int
main(int argc, char *argv[])
{
  if (argc < 3) {
    fprintf(2, "Usage: find root-path file-name ...\n");
    exit(1);
  }

  for (int i = 2; i < argc; i++) {
    find(argv[1], argv[i]);
  }

  exit(0);
}