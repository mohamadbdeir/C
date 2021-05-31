#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<string.h>
#include<sys/wait.h>
#include<ctype.h>
#include<time.h>
#include"headers/user.h"
#include"headers/movie.h"
#include"headers/dateTime.h"
#define PORT 8080
#define BUF  1024



void checkFiles()
{

	FILE *usersFile;
	usersFile = fopen("data/users", "a");
	fclose(usersFile);
	
	FILE *roomFile;
	roomFile = fopen("data/rooms", "a");
	fclose(roomFile);

	
	
	
	FILE *moviesFile;
	moviesFile = fopen("data/movies", "a+");
	
	//Future movies file contains all movies which dates aren't haven't came yet
	FILE *futureMovies;
	futureMovies = fopen("data/futureMovies", "w");
	Movie movie;
	while(fread(&movie, sizeof(Movie), 1, moviesFile))
	{
	
	
		//get today's date
		char movieDate[20]; 
		strcpy(movieDate, movie.date);
		char movieTime[20]; 
		strcpy(movieTime, movie.startTime);
		
		
		time_t rawtime;
   		struct tm *info;
   		char currentDate[80];
   		char currentTime[6];


   		time( &rawtime );	
	  	info = localtime( &rawtime );
	
  		strftime(currentDate,80,"%d/%m/%Y", info);
  		strftime(currentTime,6,"%H:%M", info);
  		
  		int dateComparison = compareDates(currentDate, movieDate);
  		int timeComparison = compareTimes(currentTime, movieTime);
  		
  		
  		if(dateComparison > 0 || timeComparison > 0 )
  		{

			fwrite (&movie, sizeof(Movie), 1, futureMovies);
  		}
	}
	
	
	fclose(futureMovies);
	fclose(moviesFile);
	
	FILE *admin = fopen("data/admin", "a");
	int c = fgetc(admin);
	if (c == EOF) {
		User user;
		printf("initializing default admin account... \n");
		char username[50] = "admin";
		char password[50] = "admin";
	
		strcpy(user.username, username);
		strcpy(user.password, password);
	
		fwrite (&user, sizeof(User), 1, admin);
		printf("admin account initialized \n");
		fclose(admin);
	} 
}

int login(int fd, char userType[6])
{
	char recvBuf[BUF];
	printf("%s login \n", userType);
	int success = 0;
	while(!success)
	{
		FILE *infile;
		char filePath[BUF] = "data/";
		strcat(filePath, userType);
		infile = fopen(filePath, "r");
		
		char username[BUF];
		char password[BUF];
		
		recv(fd, username, BUF, 0); //receives username;
		printf("received username: %s \n", username);
		recv(fd, password, BUF, 0); //receives password;

		
	
		User input;
		while(fread(&input, sizeof(User), 1, infile))
		{
		
			printf("received password: %s \n", password);
			printf("password is file: %s \n", input.password);

			if(strcmp(input.username, username) == 0)
			{
				if(strcmp(input.password, password) == 0)
				{
					success = 1;
					send(fd, "connection successfull", BUF, 0);
					printf("success \n");
					return success;
				}
			}
		}
	
		if(!success)
		{
			send(fd, "bad credentials", BUF, 0);
			recv(fd, recvBuf, BUF, 0); //receives admin login
			printf("bad credentials \n");
		}
		fclose(infile);
	}
}

int main()
{
	system("clear");
	checkFiles();
	int server_fd = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in server_address;
	server_address.sin_family = AF_INET;
	server_address.sin_port =  htons(PORT);
	server_address.sin_addr.s_addr = INADDR_ANY;

	int addrlen = sizeof(server_address);

	int a = bind(server_fd, (struct sockaddr *)&server_address, addrlen);
	
	printf("Listening: \n");
	int b = listen(server_fd, 15);
	
	//create a process who's the role is to watch for movies and change the information when  they're date is past
	
	
	while(1)
	{
		int new_fd = accept(server_fd, (struct sockaddr *)&server_address,
		(socklen_t *)&addrlen);
	
		
		if(fork() == 0)
		{
			printf("new connection \n");
			char recvBuf[BUF];
			recv(new_fd, recvBuf, BUF, 0); //receives user choice user or admin login
			printf("received option: %s \n", recvBuf);
			
			if(strcmp(recvBuf, "adminLogin") == 0)
			{
				login(new_fd, "admin");
				char serverConnection = new_fd+'0';
				execlp("./serverSide/adminServer", "adminServer", &serverConnection, NULL, NULL);
				printf("after exec, should not display \n");
			}
			

			

			if(strcmp(recvBuf, "user") == 0)
			{
				printf("user is connecting \n");
				char serverConnection = new_fd+'0';
				execlp("./serverSide/userServer", "userServer", &serverConnection, NULL, NULL);
				printf("after exec, should not display \n");
			}
			
		}
		
	}
}
