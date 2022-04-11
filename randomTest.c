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

#define SEND_BUFFER_SIZE 2048
typedef int bool;
#define true 1;
#define false 0;
int main()
{
char buf[SEND_BUFFER_SIZE];
size_t len = 0;

while(1)
{
char play = 'x';
char askToPlay[] = "Do you want to play a game (Y/N)?\n";
//char goodBye[] = "Goodbye\n";
char invalidInputTemplate[] = "Please only insert %i values\n";
while(play != 'y' && play!= 'n' && play!='Y' && play!='N')
{
        //fwrite(play, 1, strlen(play), stdout); fflush(stdout);
        //printf("\n");
        fwrite(askToPlay, 1, strlen(askToPlay), stdout); fflush(stdout); //Server Send Client Recieve (1,0)
        while((len=read(0, buf, SEND_BUFFER_SIZE))!=2) //Server Recieve Client Send (0,1)
	{
		if(len != 2) 
		{
			size_t temp_len = sprintf(buf, invalidInputTemplate, 1); 
			fwrite(buf, 1, temp_len, stdout); fflush(stdout); //(1,0)
		}
	}
        play = buf[0];
}
 
if (play == 'n' || play=='N') 
{
	//fwrite(goodBye, 1, strlen(goodBye), stdout); //(1/0)
	fflush(stdout);
	exit(1);
}
else
{
	char gameBuffer[SEND_BUFFER_SIZE];
	char tempBuffer[SEND_BUFFER_SIZE];
	int tries = 10;
	char answer[5];
	char guess[5];
	char clue[5];
	srand(time(NULL));
	for(int i = 0; i<5; i++)
	{	
		int temp = rand() % 10;
		answer[i]=temp + '0';
	}
	fwrite(answer, 1, strlen(answer), stdout); fflush(stdout);
	printf("\n");
	//size_t temp_len = sprintf(gameBuffer, "\nYou have %i tries left\n", tries);
	//fwrite(gameBuffer, 1, temp_len, stdout); fflush(stdout);
	fwrite("Lets play\n", 1, strlen("Lets play\n"), stdout); fflush(stdout); //(1,0) 
	bool win = false;
	while(tries > 0 && !win)
	{	
		char tempBuffer[SEND_BUFFER_SIZE];
		printf("tries left - %i\n", tries);
		while((len=read(0, tempBuffer, SEND_BUFFER_SIZE)) != 6)
			{
				if (len!=6)
				{
				size_t temp_len = sprintf(gameBuffer, invalidInputTemplate, 5);
				fwrite(gameBuffer, 1, temp_len, stdout); fflush(stdout); //(0,1)
				}
			}	
		for(int i = 0; i<5; i++)
		{
			//printf("%c", buf[i]);
			guess[i] = tempBuffer[i];
		}
		for(int i = 0; i<5; i++)
		{		
			int j;
			bool found = false;
			int f;
			for(j=0; j<5; j++)
			{
				if(guess[i] == answer[j])
				{
					found = true;
					f = j;
				}
				if(found && j==i) break;
			}
			if (found)
			{
				//printf("i=%i\n", i);
				//printf("f=%i\n", f);
				if(i==f) clue[i] = 'C';
				else clue[i] = 'O';
			}
			else clue[i] = 'X';
		}
		fwrite(clue, 1, strlen(clue), stdout);
		printf("\n");
		tries--;
		if(strcmp(clue, "CCCCC") == 0) win=true;

	}
	if(win) 
	{
		printf("Congradulations, you win\n");
	}
	else
	{
		printf("You lose, the answer was %s\n", answer);
	}

}




}
}
