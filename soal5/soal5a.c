#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <time.h>

char* getDate(){
    time_t raw;

    struct tm *timeinfo;
    /*dynamic array */
    char *re = (char *)calloc(20, sizeof(char));

    time(&raw);                                     //get "raw date"
    timeinfo = localtime(&raw);                     //get info from "raw date" in local timezone

    strftime(re, 20, "%d:%m:%Y-%R", timeinfo);      //write date in specified format from timeinfo

    return re;
}

void copyToDirectory(char* directory){
    FILE *source, *out;
    int i = 1;
    char    ch,
            newname[28],
            src[]={"/var/log/syslog"};

    while(i<=30){
      source = fopen(src, "r");
      snprintf(newname, 28, "%s/log%d.log", directory, i);
      //puts(newname);

      out = fopen(newname, "w");

      if(source != NULL && out != NULL){
        //copy syslog to log files in directory
        while ((ch = fgetc(source)) != EOF)
          fputc(ch, out);

        fclose(out);
        }

        i++;

        fclose(source);
        sleep(60);
    }
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

  if ((chdir("/home/izzud/Documents/SoalShift_modul2_B01/soal5")) < 0) {
    exit(EXIT_FAILURE);
  }

  close(STDIN_FILENO);
  close(STDOUT_FILENO);
  close(STDERR_FILENO);

  while(1) {
    // main program here
    char *date = NULL;

    date = getDate();
    //puts(date);

    //create directory with 777 permission
    if(!mkdir(date, S_IRWXU | S_IRWXO | S_IRWXG))
        copyToDirectory(date);

    free(date);
    //return 0;
  }
  
  exit(EXIT_SUCCESS);
}
