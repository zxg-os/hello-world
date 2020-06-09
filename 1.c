#include<sys/types.h>
#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include<errno.h>
#include<signal.h>
int wf;
void waiting()
{
	while(wf!=0);
}
void stop()
{
	wf=0;
}
int main(int argc(),char *argv[])
{
	pid_t pid1,pid2;
	pid1 = fork();
	if(pid1 < 0)
	{
		exit(-1);
	}
	else if(pid1 == 0)
	{
		wf = 1;
		signal(16,stop);
		//signal(SIGINT, SIG_IGN);
		//signal(SIGQUIT, SIG_IGN);
		waiting();
		lockf(1,1,0);
		printf("Child Process 1(pid=%d,ppid=%d) is killde by Parent!\n",getpid(),getppid());
		lockf(1,0,0);
		exit(0);
	}
	else
	{
		pid2 = fork();
		if(pid2 < 0)
		{
			exit(-1);
		}
		else if (pid2 == 0)
		{
			wf = 1;
			//signal(SIGINT, SIG_IGN);
			//signal(SIGQUIT, SIG_IGN);
			signal(17,stop);
			waiting();
			lockf(1,1,0);
			printf("Child Process 2(pid=%d,ppid=%d) is killed by Parent!\n",getpid(),getppid());
			lockf(1,0,0);
			exit(0);
		}
		else
		{
			wf = 1;
			signal(SIGINT,stop);
			waiting();
			kill(pid1,16);
			kill(pid2,17);
			printf("Parent(pid=%d) Process is killed!\n",getpid());
			exit(0);
		}
	}
	return 0;
}
