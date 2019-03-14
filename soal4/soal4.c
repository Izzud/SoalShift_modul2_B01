#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <time.h>
#include <dirent.h>
#define FILENAME "makan_sehat"

int countFile(){
    int count=0;
    DIR *d;
    struct dirent *dir;
    char newname[200];

    d = opendir("/home/izzud/Documents/makanan");

    if (d){
        while ((dir = readdir(d)) != NULL){
            char* fn = dir->d_name;
            int len = strlen(fn);
            printf("%s %ld\n", fn, strlen(fn));

            if(len >= 12 && !strncmp(fn, FILENAME, 11)){
                count++;
            }
        }
        closedir(d);
    }
    //printf("%d\n",count);
    return count;
}

void createFile(){
    int i = countFile()+1;
    printf("%d", i);
    char filename[250]={0};

    FILE *out;
    snprintf(filename,200,"/home/izzud/Documents/makanan/%s%d.txt",FILENAME,i);
    out=fopen(filename,"w");
    fclose(out);
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

  if ((chdir("/home/izzud/Documents/SoalShift_modul2_B01/soal4")) < 0) {
    exit(EXIT_FAILURE);
  }

  close(STDIN_FILENO);
  close(STDOUT_FILENO);
  close(STDERR_FILENO);

  while(1) {
    // main program here
    time_t timer;

    char filename[] = {"makan_enak.txt"};
    struct stat sb;  
    time(&timer);

   if(stat(filename, &sb) == 0){
        int seconds = difftime (timer, sb.st_atime);

        if(seconds<=30)
            createFile();
    }
    sleep(5);  
  }
  
  exit(EXIT_SUCCESS);
}
