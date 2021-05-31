#include<time.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<unistd.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<signal.h>
#include<ctype.h>
#include "headers/dateTime.h"
#define PORT 8080
#define BUF 1024


int errno ;


int login(int fd)
{
	send(fd, "adminLogin", BUF, 0);
	printf("enter your username: ");
	char username[BUF];
	
	fgets(username, BUF, stdin);
	username[strcspn(username, "\n")] = 0;
	
	send(fd, username, BUF, 0);
	
	
	printf("enter your password: ");
	char password[BUF];
	
	fgets(password, BUF, stdin);
	password[strcspn(password, "\n")] = 0;
	
	send(fd, password, BUF, 0);
	
	printf("\n");
	
	
	printf("WAITING.. \n");
	char recvBuf[BUF];
	recv(fd, recvBuf, BUF, 0);
	if(strcmp(recvBuf, "connection successfull") == 0)
		return 1;
	
	return 0;
	
}

void createAdminAccount(int socket_fd)
{
		char recvBuf[BUF];
		system("clear");
		printf("--Create admin account--- \n");
		send(socket_fd, "createAdminAccount", BUF, 0);
		
		int unique = 0;
		
		//sending username and checking if it is unique
		while(!unique)
		{
			char username[BUF];
			printf("Enter new username: ");
			
			fgets(username, BUF, stdin);
			username[strcspn(username, "\n")] = 0;
			
			send(socket_fd, username, BUF, 0); //send username
			if(strcmp(username, ":exit") == 0)
				return;
			system("clear");
			
			recv(socket_fd, recvBuf, BUF, 0);
			if(strcmp(recvBuf, "usernameTaken") == 0)
			{
				printf("This username is already taken \n");
			}
			else
			{
				unique = 1;
			}

		}
		
		//sending  password and confirmation
		int confirmed = 0;
		char password[BUF];
		char confirmation[BUF];
		while(!confirmed)
		{
			printf("Enter the new password: ");
			fgets(password, BUF, stdin);
			password[strcspn(password, "\n")] = 0;
			if(strcmp(password, ":exit") == 0)
			{
				send(socket_fd, password, BUF, 0);
				return;
			}
			
			printf("Confirm password: ");
			fgets(confirmation, BUF, stdin);
			confirmation[strcspn(confirmation, "\n")] = 0;
			
			if(strcmp(confirmation, ":exit") == 0)
			{
				send(socket_fd, confirmation, BUF, 0);
				return;
			}
			
			if(strcmp(password, confirmation) == 0)
			{
				confirmed = 1;
			}
			
			else
			{
				system("clear");
				printf("Password and confirmation are not matching \n");
			}
		}
		
		//Password and confirmation match, sending data
		send(socket_fd, password, BUF, 0);
}

void createUserAccount(int socket_fd)
{
	char recvBuf[BUF];
		system("clear");
		printf("--Create user account--- \n");
		send(socket_fd, "createUserAccount", BUF, 0);
		
		int unique = 0;
		
		//sending username and checking if it is unique
		while(!unique)
		{
			char username[BUF];
			printf("Enter new username: ");
			fgets(username, BUF, stdin);
			username[strcspn(username, "\n")] = 0;
			
			
			send(socket_fd, username, BUF, 0); //send username
			if(strcmp(username, ":exit") == 0)
				return;
			system("clear");
			
			recv(socket_fd, recvBuf, BUF, 0);
			if(strcmp(recvBuf, "usernameTaken") == 0)
			{
				printf("This username is already taken \n");
			}
			else
			{
				unique = 1;
			}

		}
		
		//sending  password and confirmation
		int confirmed = 0;
		char password[BUF];
		char confirmation[BUF];
		while(!confirmed)
		{
			printf("Enter the new password: ");
			fgets(password, BUF, stdin);
			password[strcspn(password, "\n")] = 0;
			
			if(strcmp(password, ":exit") == 0)
			{
				send(socket_fd, password, BUF, 0);
				return;
			}
			
			printf("Confirm password: ");
			fgets(confirmation, BUF, stdin);
			confirmation[strcspn(confirmation, "\n")] = 0;
			if(strcmp(confirmation, ":exit") == 0)
			{
				send(socket_fd, confirmation, BUF, 0);
				return;
			}
			
			if(strcmp(password, confirmation) == 0)
			{
				confirmed = 1;
			}
			
			else
			{
				system("clear");
				printf("Password and confirmation are not matching \n");
			}
		}
		
		//Password and confirmation match, sending data
		send(socket_fd, password, BUF, 0);	

}

void addRoom(int fd)
{
	char recvBuf[BUF];
	system("clear");
	printf("--Create a room--- \n");
	send(fd, "createRoom", BUF, 0);
	
	
	char name[BUF];
	
	int nameUnique = 1;
	
	int nameOk = 0; 
	do
	{	
		printf("enter the room's name: ");
		fgets(name, BUF, stdin);
		name[strcspn(name, "\n")] = 0;
		if(strcmp(":exit", name) == 0)
		{
			send(fd, ":exit", BUF, 0);
			return;
		}
		
		if(strlen(name) == 2)
		{
			if(isalpha(name[0]) && isdigit(name[1]))
			{

				nameOk = 1;
			}
		}
		
		if(!nameOk)
		{
			system("clear");
			printf("The room name must be a letter followed by a number \n");
		}
		else
		{
			send(fd, name, BUF, 0); //sending room name after checking leter and number			
			recv(fd, recvBuf, BUF, 0); //receives room create status
			

			
			if(strcmp(recvBuf, "nameTaken") == 0)
			{
				nameUnique = 0;
				system("clear");
				printf("This name already exists \n");
				nameOk = 0;
			}
			else
			{
				nameUnique = 1;

			}
		}
		
	}while(!nameOk || !nameUnique);
	
	int capacityValid = 1;
	char capacity[BUF];
	
	do
	{
		printf("enter capacity ");
		
		fgets(capacity, BUF, stdin);
		capacity[strcspn(capacity, "\n")] = 0;
		
		if(strcmp(":exit", capacity) == 0)
		{
			send(fd, ":exit", BUF, 0);
			return;
		}
		
		capacityValid = atoi(capacity);
		
		if(capacityValid <= 0 || capacityValid > 100)
		{
			system("clear");
			printf("Capacity should be a positif number smaller than 100 \n");
		}
	
	}while(capacityValid <= 0 || capacityValid > 100);
	
	send(fd, capacity, BUF, 0);
	printf("room created successfully \n");

}



void seeRooms(int fd, int calledByUser) //called by user allows to know if the method is called on user request or by another method
{
	if(calledByUser)
	{
		send(fd, "seeRooms", BUF, 0);
		system("clear");
	}
	

	printf("---Available rooms--- \n");
	
	char recvBuf[BUF];
	int end = 0;
	while(!end)
	{
		recv(fd, recvBuf, BUF, 0); //receives name or end 
		
		if(strcmp(recvBuf, "end") ==0) //end of data
		{
			end = 1;
			break;
		}
		printf("Room: %s \t", recvBuf); // prints name of the room
		
		recv(fd, recvBuf, BUF, 0); //receives number of places
		printf("Total places: %s \n", recvBuf); // prints total number of places
		
	}
	
	if(calledByUser)
	{
	char s[2];
	printf("Send anything to continue \n");
	fgets(s, 2, stdin);
	}
	
	
	
}

void addMovie(int fd)
{
	printf("---Add movie---\n");
	send(fd, "addMovie", BUF, 0);
	
	int roomExists = 1;
	char recvBuf[BUF];
	
	int roomAvailable = 0;
	do
	{
		do
		{
			seeRooms(fd, 0);
			printf("Pick a room:  ");
			char room[BUF];
			fgets(room, BUF, stdin);
			room[strcspn(room, "\n")] = 0;	
			send(fd, room, BUF, 0);
			
			if(strcmp(room, ":exit") == 0)
			{
				return;
			}
		
			recv(fd, recvBuf, BUF, 0);
			if(strcmp(recvBuf, "notExisting") == 0)
			{
				roomExists = 0;
			}
			
			else
			{
				roomExists = 1;
			}
		
			if(!roomExists)
			{
				system("clear")	;
				printf("The room you picked does not exist \n");
			}
		
		}while(!roomExists);
	
	
	
		char name[BUF];
		printf("Enter movie name: ");
		fgets(name, BUF, stdin);
		name[strcspn(name, "\n")] = 0;	
		send(fd, name, BUF, 0);
		if(strcmp(name, ":exit") == 0)
		{
		return;
		}
	
	
		char date[BUF];
		int dd,mm,yy;
	
	
		//GET TODAY'S DATE
		 time_t rawtime;
   		struct tm *info;
   		char currentDate[80];
   		char currentTime[6];

   		time( &rawtime );	
	  	 info = localtime( &rawtime );
	
  		 strftime(currentDate,80,"%d/%m/%Y", info);
  		 strftime(currentTime,6,"%H:%M", info);
  	
		//read date input
		int dateGreaterThanToday = 0;
		int dateValid = 0;
		do
		{
     
		printf("Enter date (DD/MM/YYYY format): ");

		//reading date as a string and decomposing it in order to check if it is :exit	
		char readDate[BUF];
		fgets(readDate, BUF, stdin);
		readDate[strcspn(readDate, "\n")] = 0;
	
		if(strcmp(readDate, ":exit") == 0)
		{
			send(fd, ":exit", BUF, 0);
			return;
		}
		
		char ddchr[3];
		ddchr[0] = readDate[0];
		ddchr[1] = readDate[1];
		dd = atoi(ddchr);
	
		char mmchr[3];
		mmchr[0] = readDate[3];
		mmchr[1] = readDate[4];
		mm  = atoi(mmchr);
	
	
		char yychr[20];
		/*yychr[0] = readDate[6];
		yychr[1] = readDate[7];
		yychr[2] = readDate[8];
		yychr[3] = readDate[9];*/
		
		strncpy(yychr, &readDate[6], 5);
		//printf("yychr :%s \n", yychr);
	
		yy = atoi(yychr);
		
		//printf("yy %d \n", yy);

		

		dateValid = checkDate(dd, mm, yy);
	
		
		if(dd<10 || mm<10)
		{

	
			if(dd<10 && mm <10)
			{

				sprintf(date, "0%d/0%d/%d", dd, mm, yy);
			}
			if(dd< 10 && mm>= 10)
			{

				sprintf(date, "0%d/%d/%d", dd, mm, yy);
			}
			if(dd >=10 && mm< 10)
			{

				sprintf(date, "%d/0%d/%d", dd, mm, yy);			
			}
		}
		else
		{
			sprintf(date, "%d/%d/%d", dd, mm, yy);
		}
	
		dateGreaterThanToday = compareDates(date, currentDate);
	
		if(dateGreaterThanToday == 1)
		{
			system("clear");
			printf("Can't add a movie on a past date \n");
		}
	
		if(!dateValid)
		{
			//system("clear");
			printf("date not valid \n");
		}
	
	
		}while(!dateValid || dateGreaterThanToday == 1) ;
	
		//date OK , sentd it
		send(fd, date, BUF, 0);

		char startTime[BUF];
	
		int timeFormatValid = 0;
		int timeNotPast = 0; //starting time can't be in the past
	
		do
		{
			printf("Enter starting time: ");
			fgets(startTime, BUF, stdin);
			startTime[strcspn(startTime, "\n")] = 0;	
	
			if(strcmp(startTime, ":exit") == 0)
			{
				send(fd, startTime, BUF, 0);
				return;
			}

			timeFormatValid = checkTime(startTime);
		
			if(strlen(startTime) > 5)
				timeFormatValid = 0;
		
			if(!timeFormatValid)
			{
				system("clear");
				printf("Time not valid \n");
			}
		
			timeNotPast = compareTimes(startTime, currentTime);
		
			if(timeNotPast == 1 && dateGreaterThanToday == 0)
			{
				system("clear");
				printf("starting time can't be past \n");
			}

		
		}while(!timeFormatValid || (timeNotPast == 1 && dateGreaterThanToday == 0)); 
	
	//startting time OK send it
		send(fd, startTime, BUF, 0);
	
		/*char endTime[BUF];
	
		 timeFormatValid = 0;
		 timeNotPast = 0; //starting time can't be before starting time
	
		do
		{
			printf("Enter ending time: ");
			fgets(endTime, BUF, stdin);
			endTime[strcspn(endTime, "\n")] = 0;	
			
			if(strcmp(endTime, ":exit") == 0)
			{	
				send(fd, endTime, BUF, 0);
				return;
			}

			timeFormatValid = checkTime(endTime);
		
			if(strlen(endTime) > 5)
			timeFormatValid = 0;
		
			if(!timeFormatValid)
			{
				system("clear");
				printf("Time not valid \n");
			}
		
			timeNotPast = compareTimes(endTime, startTime);
		
			if(timeNotPast == 1)
			{
				system("clear");
				printf("ending time  can't be before starting time \n");
			}

		
		}while(!timeFormatValid || timeNotPast == 1 ); 
	 
		 //end time ok send it;
		 send(fd, endTime, BUF, 0);*/
		 
		char duration[10] = "\0";
		 int durationValid = 1;
		 do
		 {

		 	durationValid = 1;
		 	printf("Enter duraion: ");
		 	fgets(duration, 10, stdin);
			duration[strcspn(duration, "\n")] = 0;	
			
			
			if(strcmp(duration, ":exit") == 0)
			{
				send(fd, duration, BUF, 0);
				return;
			}
			for(int i = 0; i<strlen(duration); i++)
			{
				if(!isdigit(duration[i]) && (char)duration[i] != 58)
				{
					system("clear");
					printf("duration not valid \n");
					durationValid = 0;
					break;
					
				}
			}	

			if((char)duration[2] != 58)
			{	

				durationValid = 0;
				continue;
			}
			
			char hchr[5] = "\0";
			strncpy(hchr, duration, 2);
			
			char mchr[5] = "\0";
			strncpy(mchr, &duration[3], 2);
			
			int h = atoi(hchr);
			int m = atoi(mchr);
			
			//printf("h: %d \n", h);
			//printf("m: %d \n", m);
			
			if(h >= 5 && m>0)
			{
				system("clear");
				printf("Movie can't be longer than 5 hours \n");
				durationValid = 0;
			}
			
			else if(m > 59)
			{
				system("clear");
				printf("Duration not valid \n");
				durationValid = 0;
			}
			
			
			
		 }while(!durationValid);

		 
		 //duration ok: send it
		 send(fd, duration, BUF, 0);
		 
		 
		 //printf("waiting for receive \n");
		 recv(fd, recvBuf, BUF, 0);
		 if(strcmp(recvBuf, "roomNotAvailable") == 0)
		 {
		 	system("clear");
		 	printf("The selected room is not free on the selected date and time \n");
		 }
		 else
		 {
		 	roomAvailable = 1;
		 }
		
		 
	}while(!roomAvailable);
	 

}



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

int main()
{


	int socket_fd = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in client_address;
	client_address.sin_family = AF_INET;
	client_address.sin_addr.s_addr = INADDR_ANY;
	client_address.sin_port = htons(PORT);


	int addrlen = sizeof(client_address);

	system("clear");
	int errnum;
	/*int connection =*/ connect(socket_fd, (struct sockaddr *)&client_address, addrlen);
	fprintf(stderr, "Error opening file: %s\n", strerror( errno ));
	char recvBuf[BUF];
	
	
	/*if(connection < 0)
	{
		errnum = errno;
		fprintf(stderr, "Error opening file: %s\n", strerror( errnum ));
	}

	printf("connection %d \n", connection);*/

	int loginSuccess = 0;
	
	printf("--- Welcome the admin application --- \n");
	while(!loginSuccess)
	{
		loginSuccess = login(socket_fd);
		if(!loginSuccess)
		{	
			system("clear");
			printf("Bad credentials \n");
		}
		else
		{
			printf("connection successfull \n");
			sleep(1);
		}
			
	}
	signal(SIGINT, SIG_IGN);
	int exit = 0;
	
	while(!exit)
	{
	int option = 111;
	do
	{
		system("clear");
		if(option != 0 && option !=1 && option != 2 && option != 3 && option != 4 && option != 5 && option != 6 && option!= 111)
			printf("Choice not valid \n");
		printf("Pick your option: \n");
		printf("1. Create an admin account \n");
		printf("2. Create a user account \n");
		printf("3. Add a room \n");
		printf("4. Availble rooms \n");
		printf("5. Add a movie \n");
		printf("6. See scheduled movies \n");		
		printf("0. exit \n");
		
		char userOption[BUF];
		char *remaining;
		
		fgets(userOption, BUF, stdin);
		userOption[strcspn(userOption, "\n")] = 0;	
		option = strtol(userOption, &remaining, 10);
		
		
		if(strlen(remaining) !=0 && strlen(remaining) !=1)  //choice not valid
			option = -1;

		

	}while(option != 0 && option !=1 && option != 2 && option!=3 &&  option != 4 && option != 5 && option != 6);
	
	if(option == 1)
	{
		createAdminAccount(socket_fd);
	}
	
	else if(option == 2)
	{
		createUserAccount(socket_fd);
	}
	else if(option == 3)
	{
		addRoom(socket_fd);
	}
	else if(option == 4 )
	{
		seeRooms(socket_fd, 1);
	}
	
	else if(option == 5)
	{
		addMovie(socket_fd);
	}
	
	else if(option == 6)
	{
		seeMovies(socket_fd);
	}
	else if(option == 0)
	{
		printf("exiting ... \n");
		exit = 1;
		(socket_fd, "exit", BUF, 0);
	}
	
	}
	
	
	
	
}
