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

void createFile() {
 	int count =1;
      char newname[100];
      sprintf(newname, "%d", count);

      char dir[]="/home/kiki/Documents/makanan/makan_sehat";
      strcat(dir, newname);
      strcat(dir,".txt");
      
      FILE *makanan = fopen("/home/kiki/Documents/makanan/makan_sehat", "w"); 
      fclose(makanan);
      count=count+1;
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

  if ((chdir("/")) < 0) {
    exit(EXIT_FAILURE);
  }

  close(STDIN_FILENO);
  close(STDOUT_FILENO);
  close(STDERR_FILENO);
  while(1) 
  {
    time_t timer;
    struct stat sb;
    stat("/home/kiki/Documents/makanan/makan_enak.txt", &sb);

    if(difftime(time(&timer),sb.st_atime)<=30)
    {
	createFile();
    }
    sleep(5);
  }
  exit(EXIT_SUCCESS);
}
