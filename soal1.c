#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <dirent.h>
#define suffix "_grey.png"

// 1 for true     
int isNotRenamed(char* filename, int len){
    const char *last_nine;
    last_nine = &filename[len-9];

    if(!strcmp(last_nine,suffix))
        return 0;
    return 1;
}

int main() {
  pid_t pid, sid;

  pid = fork();

  if (pid < 0) {
    exit(EXIT_FAILURE);
  }

  if (pid > 0) {
    exit(EXIT_SUCCESS);
  }

  umask(0);

  sid = setsid();

  if (sid < 0) {
    exit(EXIT_FAILURE);
  }

  if ((chdir("/home/izzud/modul2")) < 0) {
    exit(EXIT_FAILURE);
  }

  close(STDIN_FILENO);
  close(STDOUT_FILENO);
  close(STDERR_FILENO);

  while(1) {
    // main program here
    DIR *d;
    int len;
    char *filename, *ext;
    struct dirent *dir;

    char newname[200];

    d = opendir(".");

    if (d){
        //loop to check all files
        while ((dir = readdir(d)) != NULL){
            memset(newname,0,200);

            //printf("%s\n", dir->d_name);

            filename = dir->d_name;

            len = strlen(filename);

            //get file's extension
            ext = strrchr(filename, '.');

            //rename and move .png file
            if(ext && strlen(ext)==4 && !strcmp(ext,".png")){
              char fname[200] = {0};
              strncpy(fname,filename, len-4);
              snprintf(newname, 200, "gambar/%s_grey.png", fname);
              rename(filename, newname);
            }
        }
      closedir(d);
    }
    sleep(2);
    //return 0;
  }
  
  exit(EXIT_SUCCESS);
}