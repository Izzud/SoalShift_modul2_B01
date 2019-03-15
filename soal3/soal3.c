#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/wait.h>

int main () {
	int fd[2];
	pid_t child_id;
	pid_t child_id2;
	pipe(fd);
	int status;
	if(pipe(fd) < 0) exit (1);

	child_id = fork();

	if(child_id == 0){
	char *argv[5] = {"unzip","/home/kiki/Documents/campur2.zip","-d", "/home/kiki/Documents", NULL};
	execv("/usr/bin/unzip", argv);
	}
	else
	{
	while((wait(&status)) > 0);
	child_id2 = fork();
	if(child_id2 == 0 ){
	close (fd[0]); //read
		dup2(fd[1],STDOUT_FILENO);
		char *argv[3] = {"ls", "/home/kiki/Documents/campur2/", NULL};
		execv("/bin/ls", argv);
		}
		else{
			while((wait(&status))>0);
			close(fd[1]);
			dup2(fd[0],STDIN_FILENO);
			int fileopen = open("/home/kiki/Documents/daftar.txt", O_WRONLY);
			close(fd[0]);
			dup2(fileopen, STDOUT_FILENO);
			char *argv[3] = {"grep",".txt$", NULL};
			execv("/bin/grep", argv);
		}

}	
}

