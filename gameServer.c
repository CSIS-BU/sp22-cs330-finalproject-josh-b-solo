#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <errno.h>

#define PORT 27568
#define BUFFER_SIZE 2048
#define QUEUE_LENGTH 10
typedef int bool;
#define true 1;
#define false 0;

int main()
{
	struct sockaddr_in sin;
	int addr_len=sizeof(sin);
	int s, new_s;
	pid_t pid;

	bzero((char *)&sin, sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = INADDR_ANY;
	sin.sin_port = htons(PORT);

	if((s = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
		perror("simplex-talk: socket");
		exit(1);
	}

	if((bind(s, (struct sockaddr *)&sin, sizeof(sin))) < 0){
		perror("simplex-talk: bind");
		exit(1);
	}

	listen(s, QUEUE_LENGTH);


	while(1)
	{
		if((new_s = accept(s, (struct sockaddr *)&sin, &addr_len)) < 0){
			perror("simplex-talk: accept");
			close(new_s);
		}
		pid = fork();
		if(pid > 0){
			close(new_s);
		}
		else if(pid == 0){
			close(s);




			//game code
			char buf [BUFFER_SIZE];
			size_t len = 0;
			char askToPlay[] = "Do you want to play a game (Y/N)?\n";
			char goodBye[] = "Goodbye\n";
			char invalidInputTemplate[] = "Please only insert %i values\n";
			while(1)
			{
				char play = 'x';
				while(play != 'y' && play != 'n' && play != 'Y' && play != 'N')
				{
					send(new_s, askToPlay, strlen(askToPlay), 0);
					while((len = recv(new_s, buf, sizeof(buf), 0) != 1))
					{
						if(strcasecmp(buf, "Joshua\n")==0)
						{
							strcpy(buf, "Starting Global Thermonuclear War\n");
							send(new_s, buf, strlen(buf), 0);
						}
						else if(len != 1)
						{
							size_t temp_len = sprintf(buf, invalidInputTemplate, 1);
							send(new_s, buf, temp_len, 0);
						}
					}
					play = buf[0];
				}
				if(play == 'n' || play == 'N')
				{
					send(new_s, goodBye, strlen(goodBye), 0);
					close(new_s);
					exit(1);
				}
				else
				{
					int tries = 10;
					char answer[6] = "";
					char guess[6] = "";
					char clue[6] = "";
					srand(time(NULL));
					for(int i = 0; i<5; i++)
					{
						int temp = rand() % 10;
						answer[i] = temp + '0';
					}
					bool win = false;
					while(tries > 0 && !win)
					{
						if(tries == 10)
						{
							size_t temp_len = sprintf(buf, "Good luck, you have %i tries to solve the puzzle. C = correct, 0 = right number wrong place, X = incorrect number\n", tries);
							send(new_s, buf, temp_len, 0);
						}
						else
						{
							size_t temp_len = sprintf(buf, "%s\nYou have %i tries remaining\n", clue, tries);
							send(new_s, buf, temp_len, 0);
						}
						while((len = recv(new_s, buf, sizeof(buf), 0)) != 5)
						{
							if(len!=5)
							{
								size_t temp_len = sprintf(buf, invalidInputTemplate, 5);
								send(new_s, buf, temp_len, 0);
							}
						}
						for (int i = 0; i<5; i++)
						{
							guess[i] = buf[i];
						}
						for (int i = 0; i<5; i++)
						{
							int j;
							bool found = false;
							int f;
							for(j=0; j<5; j++)
							{
								if(guess[i] == answer[j])
								{
									found = true;
									f=j;
								}
								if (found && j == i) break;
							}
							if (found)
							{
								if(i==f) clue[i] = 'C';
								else clue[i] = 'O';
							}
							else clue[i] = 'X';
						}
						
						tries--;
						if(strcmp(clue, "CCCCC") == 0) win=true;
					}
					if(win)
					{
						strcpy(buf, "Congratulations, you win\nEnter anything to continue\n");
					}
					else
					{
						sprintf(buf, "You lose, the answer was %s\nEnter anything to continue\n", answer);
					}
					send(new_s, buf, strlen(buf), 0);
				}
			}
		}

	}
	return 0;
}
