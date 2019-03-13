#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <grp.h>
#include <pwd.h>
#define keyword "www-data"

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

  if ((chdir("/home/izzud/Documents/SoalShift_modul2_B01/hatiku")) < 0) {
    exit(EXIT_FAILURE);
  }

  close(STDIN_FILENO);
  close(STDOUT_FILENO);
  close(STDERR_FILENO);

  while(1) {
    // main program here
    char filename[] = {"elen.ku"};
    char *owner, *group;
    struct stat sb;  

    if(stat(filename, &sb) == 0){
        mode_t perm = sb.st_mode | 0777;        //bitwise OR operation

        //change file permission
        chmod(filename, perm);

        struct passwd *pw = getpwuid(sb.st_uid);
        struct group  *gr = getgrgid(sb.st_gid);

        owner = pw->pw_name;
        group = gr->gr_name;
        
        if(!strcmp(owner, keyword) && !strcmp(group, keyword))
            remove(filename);
    }
    
    sleep(3);
    //return 0;
  }
  
  exit(EXIT_SUCCESS);
}