#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

char*
fmtname(char *path)
{ 
  static char buf[DIRSIZ+1];
  char *p; 
  //printf(" %s Dheeraj inside fmtname\n",path);
  
  // Find first character after last slash.
  for(p=path+strlen(path); p >= path && *p != '/'; p--)
    ;
  p++;
  
  // Return blank-padded name.
  if(strlen(p) >= DIRSIZ)
    return p;
  memmove(buf, p, strlen(p));
  memset(buf+strlen(p), ' ', DIRSIZ-strlen(p));
  return buf;
}


int match_strings( char *a, char *b, int n)
{
  int i = 0; 
  while(i<n &&  *a == *b)
  {
    i++; 
    a++; 
    b++;
  }
  if( i>=n)
    return 1;
  else 
    return 0;

}

void
ls(char *path, char *pattern)
{
  char buf[512], *p;
  int fd;
  struct dirent de;
  struct stat st;

  if((fd = open(path, 0)) < 0){
    fprintf(2, "ls: cannot open %s\n", path);
    return;
  }

  if(fstat(fd, &st) < 0){
    fprintf(2, "ls: cannot stat %s\n", path);
    close(fd);
    return;
  }

switch(st.type){
  case T_FILE:
    printf("%s %d %d %l\n", fmtname(path), st.type, st.ino, st.size);
    break;

  case T_DIR:
    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof buf){
      printf("ls: path too long\n");
      break;
    }
    strcpy(buf, path);
    p = buf+strlen(buf);
    *p++ = '/';
    while(read(fd, &de, sizeof(de)) == sizeof(de)){
      if(de.inum == 0)
        continue;
      memmove(p, de.name, DIRSIZ);
      p[DIRSIZ] = 0;
      //printf(" %s  Dheeraj inside while loop\n ",p);
      if(stat(buf, &st) < 0){
        printf("ls: cannot stat %s\n", buf);
        continue;
      }

      // get the fmt name from the buffer 

      char *file_name = fmtname( buf);
      if( file_name[0] == '.')
        continue; 
      else if( st.type ==1)
      {
        ls( buf, pattern);
      }
      else if ( st.type ==2 && match_strings( file_name, pattern, strlen( pattern)) == 1)
      {
        printf(" found file %s\n", buf);

      }
      printf("%s %d %d %d\n", fmtname(buf), st.type, st.ino, st.size);
    }
    break;
  }
  close(fd);
}

int
main(int argc, char *argv[])
{
  int i;

  if(argc < 3){
    ls(argv[1], argv[2]);
    exit();
  }
}

