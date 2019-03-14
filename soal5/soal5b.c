#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <signal.h>

int main(){
    char out[7]={0};

    //pipe, find pid
    FILE *cmd = popen("pidof /home/izzud/Documents/SoalShift_modul2_B01/soal5/soal5a", "r");

    //grab its output
    fgets(out, 7, cmd);

    //convert it to unsigned int 
    pid_t pid = strtoul(out, NULL, 10);

    //close pipe
    pclose(cmd);

    //printf("PID: %d\n", pid);

    //send SIGKILL signal to process
    if(!kill(pid, SIGKILL))
        printf("Process with pid of %d is successfully killed\n", pid);

    return(0);
}
