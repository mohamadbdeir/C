#include<netinet/in.h>
#include<sys/socket.h>
#include<unistd.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<signal.h>
#include<sys/wait.h>
#define PORT 8080
#define BUF 1024

char globalUsername[BUF];

void seeMovies(int fd)
{
	send(fd, "seeMovies", BUF, 0);

	system("clear");
	printf("---Schedule--- \n");
	
	char recvBuf[BUF];
	
	int end = 0;
	while(!end)
	{
		recv(fd, recvBuf, BUF, 0);

		if(strcmp(recvBuf, "end") != 0)
		{
			printf("%s \n", recvBuf);
		}
		
		else
		{
			end = 1;
		}
	}
	
	char s[2];
	printf(" \n Send anything to continue \n");
	fgets(s, 2, stdin);
}

void reservation(int fd)
{
	send(fd, "reservation", BUF, 0);
	printf("Choose a movie: \n");
	
	char recvBuf[BUF];
	
	/*int end = 0;
	while(!end)
	{

		recv(fd, recvBuf, BUF, 0);
		
		if(strcmp(recvBuf, "end") == 0)
		{
			end = 1;
		}
		else
		{
			printf("%s \n", recvBuf);
		}
	}*/
	
	
	

		char userOption[BUF];
		int option = 0;
	do
	{

		option = -1;
		int end = 0;
		while(!end)
		{	
			recv(fd, recvBuf, BUF, 0);	//receive movie with id
			if(strcmp(recvBuf, "end") == 0)
			{
				end = 1;
			}
			else if(strcmp(recvBuf, "nothigToDisplay") == 0)
			{
				printf("There is nothing showing now, try again later... \n");
				char s[2];
				printf(" \n Send anything to continue \n");
				fgets(s, 2, stdin);
				return ;
			}
			else
			{
				printf("%s", recvBuf);
			}
		}
	

		char userOption[BUF];
		char *remaining;
		
		printf("Your input: \n");
		fgets(userOption, BUF, stdin);
		userOption[strcspn(userOption, "\n")] = 0;
		
		//handling exit case
		if(strcmp(userOption, ":exit") == 0)	
		{
			send(fd, userOption, BUF, 0);
			return;
		}
		option = strtol(userOption, &remaining, 10);
		
		
		if(strlen(remaining) !=0 && strlen(remaining) !=1)  //choice not valid
			option = -1;
			
		if(option < 0)
		{
			printf("Please try again \n");
		}
		
		//confirm that choose movie exists
		else //input is a number
		{

			send(fd, userOption, BUF, 0); //send id
			
			recv(fd, recvBuf, BUF, 0);
			
			if(strcmp(recvBuf, "movieNotAvailable") == 0)
			{
				system("clear");
				printf("Please try again \n");
				option = -1;
			}
			
			else
			{
				system("clear");
				option = 1;
			}
		}
		

	}while(option < 0);
	

	//send(fd, userOption, BUF, 0);
	
	//recv(fd, recvBuf, BUF, 0); //receives how many times the selected movie is shown
	
	//int numberOfTimes = atoi(recvBuf);


	printf("Choose the program that suits you: \n");
	int end = 0;
	int count = 0;
	option = 0;
	while(end == 0)
	{
		count ++;

		recv(fd, recvBuf, BUF, 0);

		if(strcmp(recvBuf, "end") == 0)
		{
			end = 1;

		}
		else
		{
			printf("%s", recvBuf);
		}
	}
	

	
	//user should pick a number after seeing movie details

	do
	{
		char *remaining;
		
		fgets(userOption, BUF, stdin);
		userOption[strcspn(userOption, "\n")] = 0;	
		
		if(strcmp(userOption, ":exit") == 0)
		{
			send(fd, userOption, BUF, 0);
			return ;
		}
		
		option = strtol(userOption, &remaining, 10);
		
		
		if(strlen(remaining) !=0 && strlen(remaining) !=1)  //choice not valid
			option = -1;
			
			
		if(option < 0)
		{
			printf("Please try again: \n");
		}
		else
		{

			send(fd, userOption, BUF, 0); //send choosen details
			
			//receive confirmation
			recv(fd, recvBuf, BUF, 0);
			
			if(strcmp(recvBuf, "choiceNotAvailable") == 0)
			{
				printf("Please try again: \n");
				option = -1;
			}
			else
			{
				option = 1;
			}
		}
		

			
	}while(option < 0);
	

	
	
	//receive confirmation about the sent details

	
	
}

int login(int fd)
{

	printf("enter your username: ");
	char username[BUF];
	//	scanf("%s", username);
	fgets(username, BUF, stdin);
	username[strcspn(username, "\n")] = 0;	
	
	send(fd, username, BUF, 0);
	
	if(strcmp(":exit", username) == 0)
	{
		system("clear");
		return 2;
	}
	
	strcpy(globalUsername, username);
	
	printf("enter your password: ");
	char password[BUF];
	//	scanf("%s", password);
	fgets(password, BUF, stdin);
	password[strcspn(password, "\n")] = 0;	
	
	send(fd, password, BUF, 0);
	
	if(strcmp(":exit", password) == 0)
	{
		system("clear");
		return 2;
	}
	
	printf("\n");
	
	char recvBuf[BUF];
	printf("waiting for response: ... \n");
	recv(fd, recvBuf, BUF, 0);
	if(strcmp(recvBuf, "connection successfull") == 0)
		return 1;
	
	return 0;
	
}


void createUserAccount(int fd)
{	
	int unique = 0;
	char recvBuf[BUF];
	do{
	
		char username[BUF];
		printf("Enter new username: ");
		//		scanf("%s", username);
		fgets(username, BUF, stdin);
		username[strcspn(username, "\n")] = 0;		
	
		send(fd, username, BUF, 0);
		if(strcmp(username,":exit") == 0)
		{
			return;	
		}
		recv(fd, recvBuf,  BUF, 0); //receiving confirmation if username is unique or not
		
		if(strcmp(recvBuf, "ok")== 0)
		{
			printf("ok \n");
			unique = 1;
		}
		else
		{
			system("clear");
			printf("Username already taken \n");
		}
		
		
	}while(!unique);
	
	system("clear");
	int testConfirmation = 0;
	char password[BUF];
	char confirmation[BUF];
	
	while(!testConfirmation)
	{
		printf("Enter your password \n");
		//		scanf("%s", password);
		fgets(password, BUF, stdin);
		password[strcspn(password, "\n")] = 0;			
		if(strcmp(password,":exit") == 0)
		{
			send(fd, password, BUF, 0);
			return;	
		}
	
		printf("Confirm password \n");
		//		scanf("%s", confirmation);
		fgets(confirmation, BUF, stdin);
		confirmation[strcspn(confirmation, "\n")] = 0;			
		if(strcmp(confirmation,":exit") == 0)
		{
			send(fd, confirmation, BUF, 0);
			return;	
		}
		
		if(strcmp(password, confirmation) == 0)
		{
			testConfirmation = 1;
		}
		else
		{
		system("clear");
		printf("Password and confirmation are not matching \n");
		}
	}
	
	send(fd, password, BUF, 0);
	
	
		
}

void seeReservation(int fd)
{
	char recvBuf[BUF];
	send(fd, "seeReservation", BUF, 0);
	
	system("clear");
	printf("Upcoming reservations: \n");
	int end = 0;
	do
	{
		recv(fd, recvBuf, BUF, 0); //receives reservation or end
		if(strcmp(recvBuf, "end") == 0)
		{
			end = 1;
		}
		else if(strcmp(recvBuf, "noData") == 0)
		{
			printf("You have no upcoming reservations \n");
			char s[2];
			printf(" \n Send anything to continue \n");
			fgets(s, 2, stdin);
			return ;

		}
		else
		{
			printf("%s \n", recvBuf);
		}
		
	}while(!end);
	
	char s[2];
	printf(" \n Send anything to continue \n");
	fgets(s, 2, stdin);

}

int main()
{
	system("clear");
	signal(SIGINT, SIG_IGN);
	int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	
	struct sockaddr_in client_address;
	client_address.sin_family = AF_INET;
	client_address.sin_addr.s_addr = INADDR_ANY;
	client_address.sin_port = htons(PORT);


	int addrlen = sizeof(client_address);


	int connection = connect(socket_fd, (struct sockaddr *)&client_address, addrlen);
	char recvBuf[BUF];


	send(socket_fd, "user", BUF, 0)	;
	printf("--- Welcome--- \n");
	int option = 111;
	int exit = 0;
	do
	{
		system("clear");
		do
		{
			if(option != 0 && option!= 1 && option != 2 && option != 111 )
				printf("Option not available");
			printf("pick your option: \n");
			printf("1. Log in \n");
			printf("2. Create an account \n");
			printf("0. Exit \n");
			
			char userOption[BUF];
			char *remaining;
		
			fgets(userOption, BUF, stdin);
			userOption[strcspn(userOption, "\n")] = 0;	
			option = strtol(userOption, &remaining, 10);
		
		
			if(strlen(remaining) !=0 && strlen(remaining) !=1)  //choice not valid
				option = -1;
			
		}while(option != 0 && option != 1 && option != 2);
	
		if(option == 1) // log in as a user
		{
	
			system("clear");
			send(socket_fd, "login", BUF, 0);
			int loggedIn = 0;
			do
			{
				loggedIn = login(socket_fd);
				if(!loggedIn)
				{	
					system("clear");
					printf("bad credentials \n");
				}
			}while(loggedIn == 0); //logged in should be == 2 if user typed :exit
			
			if(loggedIn == 1)
			{
				printf("logged in successfully \n");
			

				int loggedinOption = -1;
	
			//USER IS NOW LOGGED IN
				do
				{
					system("clear");
					printf("---Welcome, pick your option----\n");
					printf("1. See schedule \n");
					printf("2. Reservation \n");
					printf("3. See my reservations \n");
					printf("0. Log out \n");
			

				
					char userOption[BUF];
					char *remaining;
			
					fgets(userOption, BUF, stdin);
					userOption[strcspn(userOption, "\n")] = 0;	
					loggedinOption = strtol(userOption, &remaining, 10);
					
					if(loggedinOption == 1)
					{
						seeMovies(socket_fd);
						
					}
					else if(loggedinOption == 2)
					{
						reservation(socket_fd);
					}
					else if(loggedinOption == 3)
					{
						seeReservation(socket_fd);
					}
					
					else if(loggedinOption == 0)
					{
						send(socket_fd, "logout", BUF, 0);
					}
					
				}while(loggedinOption !=0 );	
		
			}
		}	
		
		if(option == 2)
		{
			system("clear");
			send(socket_fd, "create", BUF, 0);
			createUserAccount(socket_fd);
		}
		
		if(option == 0)
		{
			system("clear");
			exit = 1;
			send(socket_fd, "exit", BUF, 0);
		}
		
	}while(!exit);
}
