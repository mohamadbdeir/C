#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<string.h>
#include<time.h>
#include<ctype.h>
#include<sys/wait.h>
#include"./../headers/user.h"
#include"./../headers/room.h"
#include"./../headers/movie.h"
#include"./../headers/dateTime.h"
#include<errno.h>
#define PORT 8080
#define BUF  1024


int checkUniqueUsername(char username[BUF], char userType[6])
{
	char filePath[128] = "./data/";
	
	strcat(filePath, userType);

	FILE *file;
	file = fopen(filePath, "r");
	
	printf("received username to test %s \n", username);
	
	User input;
	while(fread(&input, sizeof(User), 1, file))
	{

		if(strcmp(input.username, username) == 0)
		{

			return 0;
		}
	}
	

	
	fclose(file);
	return 1;
	
}

void reschedule()
{
	printf("RESCHEDULING \n");

	FILE *moviesFile;
	moviesFile = fopen("data/movies", "r");
	
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
  		
  		//printf("\n \ncurrent time: %s ", currentTime);
  		//printf("movie time: %s \n\n", movieTime);
  		int timeComparison = compareTimes(currentTime, movieTime);
  		
  		//printf("Cureent date %s \n", currentDate);
  		//printf("Movie date: %s \n", movieDate);
  		//printf("date compare %d \n", dateComparison);
  		
  		//printf("Cureent time %s \n", currentTime);
  		//printf("Movie time: %s \n", movieTime);
  		//printf("time compare %d \n", timeComparison);
  		
  		
  		
  		if(dateComparison > 0)
  		{
  			//printf("writing to future movies \n");
			fwrite (&movie, sizeof(Movie), 1, futureMovies);
  		}
  		
  		if(dateComparison == 0 && timeComparison > 0)
  		{
  			//printf("writing to future movies \n");
			fwrite (&movie, sizeof(Movie), 1, futureMovies);
  		}
	}
	
	
	fclose(futureMovies);
	fclose(moviesFile);
}

int checkRoom(char room[2])
{


	FILE *file;
	file = fopen("./data/rooms", "r+");
	Room input;	
	while(fread(&input, sizeof(Room), 1, file))
	{

		if(strcmp(input.name, room) == 0)
		{
			return 1;	
		}
	}

	fclose(file);
	return 0;
	
	
}

int getNumberOfPlaces(char room[3])
{

	FILE *file;
	file = fopen("./data/rooms", "r+");
	Room input;	
	while(fread(&input, sizeof(Room), 1, file))
	{

		if(strcmp(input.name, room) == 0)
		{
			return	input.capacity;	
		}
	}

	fclose(file);
	return 0;		
}

void createNewAccount(char username[BUF], char password[BUF], char userType[6])
{
	char filePath[128] = "./data/";
	
	strcat(filePath, userType);

	FILE *file;
	file = fopen(filePath, "a");
	
	User user;
	strcpy(user.username, username);
	strcpy(user.password, password);
	
	fwrite (&user, sizeof(User), 1, file);
	
	
	fclose(file);
	
	
	
}

void createAdminAccount(int fd)
{
		char username[BUF];
		char password[BUF];
		int unique = 0;
		printf("create admin account \n");
		
		int accountDataOk = 0;
		while(!unique)
		{
			recv(fd, username, BUF, 0); //receives username

			if(strcmp(username, ":exit") == 0)
				return;

			unique = checkUniqueUsername(username, "admin");
			/*
			 Checking if username is unique and sending response
			*/
			if(!unique)
			{
				send(fd, "usernameTaken", BUF, 0);
			}
			else
			{
				send(fd, "usernameOk", BUF, 0);

			}
		}
		
		//receiving password
		
			recv(fd, password, BUF, 0);
			if(strcmp(password, ":exit") == 0)
				return;
			createNewAccount(username, password, "admin"); //creating the account and writing it to the file
		
		
		
}

void createUserAccount(int fd)
{
	
		char username[BUF];
		char password[BUF];
		int unique = 0;
		printf("create user account \n");
		
		int accountDataOk = 0;
		while(!unique)
		{
			recv(fd, username, BUF, 0); //receives username

			if(strcmp(username, ":exit") == 0)
				return;

			unique = checkUniqueUsername(username, "users");
			/*
			 Checking if username is unique and sending response
			*/
			if(!unique)
			{
				send(fd, "usernameTaken", BUF, 0);
			}
			else
			{
				send(fd, "usernameOk", BUF, 0);

			}
		}
		
		//receiving password
		
			recv(fd, password, BUF, 0);
			if(strcmp(password, ":exit") == 0)
				return;
			createNewAccount(username, password, "users"); //creating the account and writing it to the file
		
}

void createRoom(int fd)
{
	char recvBuf[BUF];
	printf("create room \n");
	
	int nameUnique = 1;
	char name[3];
	
	do
	{	
		printf("waiting for room name input \n"); 
		recv(fd, name, BUF, 0); 
		if(strcmp(name, ":exit") == 0)
		{	
			return;
		}
		printf("received room name: %s \n", name);
		
		//check if the name already exists
		

		FILE *file;
		file = fopen("./data/rooms", "r+");

		Room input;
		while(fread(&input, sizeof(Room), 1, file))
		{


			if(strcmp(input.name, name) == 0)
			{

				send(fd, "nameTaken", BUF, 0);
				nameUnique = 0;
				break;
			}
			else
			{
				nameUnique = 1;
			}
		}
	

		fclose(file);
		if(nameUnique)
		{
			send(fd, "nameUnique", BUF, 0);
			printf("name unique \n");

		}
		
	}while(!nameUnique);
	
	char recvCapacity[BUF];
	recv(fd, recvCapacity, BUF, 0); //receiving room capacity as a string
	
	if(strcmp(recvCapacity, ":exit") == 0)
	{	
		return;
	}
	
	int capacity = atoi(recvCapacity);
	
	Room room;
	FILE *outfile;
	outfile = fopen("./data/rooms", "a+");
	strcpy(room.name, name);
	room.capacity = capacity;
	fwrite (&room, sizeof(room), 1, outfile);
	fclose(outfile);
	
}


void seeRooms(int fd)
{

	

		FILE *file;
		file = fopen("./data/rooms", "r+");
		Room input;
		while(fread(&input, sizeof(Room), 1, file))
		{

			char sendBuf[BUF] = "Room: ";
			strcpy(sendBuf, input.name);
			printf("room name: %s \n", sendBuf);
			send(fd, sendBuf, BUF, 0); //sending the name
			
			strcpy(sendBuf, "");
			sprintf(sendBuf, "%d", input.capacity);
			send(fd, sendBuf, BUF, 0); //sending number of places
			
			
		}
		
		fclose(file);

		send(fd, "end", BUF, 0);
		

}

int roomAvailable(char room[3], char date[11], char startTime[6], char endTime[6])
{

	FILE *file;
	file = fopen("./data/movies", "r+");
	Movie input;	
	while(fread(&input, sizeof(Movie), 1, file))
	{
	
		char readDate[11];
		strcpy(readDate, input.date);
		
		char readStime[6];
		strcpy(readStime, input.startTime);
		
		char readEtime[6];
		strcpy(readEtime, input.endTime);
		
		char readRoom[3];
		strcpy(readRoom, input.room);
		
		if(strcmp(readDate, date) == 0 && strcmp(readRoom, room) == 0) 
		{
			//comparing input startTime and the one in the file
			int a = compareTimes(startTime, readEtime); // -1 if startTime < readEtime
		//	printf(" a= %d \n", a);
			
			//comparing startTime and readStartime
			int b = compareTimes(startTime, readStime); // -1 if startTime<readStime
			//printf(" b= %d \n", b);
			
			//compare endTime and readStime
			int c = compareTimes(endTime, readStime); // -1 if endTime < readStime
			//printf(" c= %d \n", c);
			 //compare endTime and readEnd
			 int d = compareTimes(endTime, readEtime); // -1 if endTime < readEtime
			// printf(" d= %d \n", d);
			 
			 if((a == 1 || a == 0) && (b == -1 || b ==0))
			 {
			 	printf("room not available 1 \n");
			 	return 0;
			 }
			 
			 if((c == -1 || c ==0) && (d == 1 || d == 0))
			 {
				printf("room not available 2 \n");
			 	return 0;
			 }
		}
		

	}
	
	fclose(file);
	printf("returning 1 \n");
	return 1;
}

void addMovie(int fd)
{
	char recvBuf[BUF];
	printf("add a movie \n");

	char roomName[BUF];
	char movieName[BUF];
	char date[BUF];
	char startTime[BUF];
	char endTime[BUF];
	char duration[BUF];
	int places = 0;
	
	int roomOk = 0;
	int exists = 1; //validating of the choosen room exits
	
	do
	{
	

		do
		{
			seeRooms(fd);

			recv(fd, roomName, BUF, 0); //receives room name;
			if(strcmp(roomName, ":exit") == 0)
			{
				printf("exit \n");
				return;
			}
	
			exists = checkRoom(roomName);
			
			if(!exists)
			{
				send(fd, "notExisting", BUF, 0);
			}
			else
			{
				send(fd, "roomOk", BUF, 0);
			}
		
		}while(!exists);
	
	
		places = getNumberOfPlaces(roomName);
	
		recv(fd, movieName, BUF, 0);
		if(strcmp(movieName, ":exit") == 0)
			{
				printf("exit \n");
				return;
			}	
	
		printf("movie name: %s \n", movieName);
	
		recv(fd, date, BUF, 0); //receive date
	
		if(strcmp(date, ":exit") == 0)
		{
			printf("exit \n");
			return ;
		}
	
	
		recv(fd, startTime, BUF, 0); //receive start time;
		if(strcmp(startTime , ":exit") == 0)
		{
			printf("exit \n");
			return;
		}
	
		recv(fd, duration, BUF, 0); //receive duration;
		if(strcmp(duration , ":exit") == 0)
		{
			printf("exit \n");
			return;
		}

		printf("duration: %s \n", duration);
		addTimes(endTime, startTime, duration);
		
		printf("End time: %s \n\n", endTime);
		
		/*printf("Detais : room: %s \n movie: %s \n places: %d \n date: %s \n start: %s \n end: %s \n", 
		roomName, movieName, places, date, startTime, endTime);*/
	
		roomOk = roomAvailable(roomName, date, startTime, endTime);
		
		if(!roomOk)
		{		
				send(fd, "roomNotAvailable", BUF, 0);
				printf("room not available sent \n");
		}
			
		else
		{
			send(fd, "roomAvailble", BUF, 0);
			printf("room available sent \n");
		}
		
	}while(!roomOk);
	
	
	
	FILE *outfile;
	outfile = fopen("./data/movies", "a+");
	
	
	Movie movie;
	strcpy(movie.name, movieName);
	strcpy(movie.room, roomName);
	strcpy(movie.date, date);
	strcpy(movie.startTime, startTime);
	strcpy(movie.endTime, endTime);
	movie.places = places;
	movie.remainingPlaces = places;
	
	fwrite (&movie, sizeof(Movie), 1, outfile);
	fclose(outfile);
	
	reschedule();
	
	
}


void seeMovies(int fd)
{


	reschedule();
	printf("see movies: \n");
	FILE *infile;

	infile = fopen("./data/futureMovies", "r");
	
	char sendBuf[BUF];
	
	Movie input;

	while(fread(&input, sizeof(Movie), 1, infile))
	{

		
			strcpy(sendBuf, "");
			strcat(sendBuf, "Movie: ");
			strcat(sendBuf, input.name);
			strcat(sendBuf, "\n");
			
			strcat(sendBuf, "Date: ");
			strcat(sendBuf, input.date);
			strcat(sendBuf, "\n");
			
			strcat(sendBuf, "Starting time: ");
			strcat(sendBuf, input.startTime);
			strcat(sendBuf, "\n");
		
			strcat(sendBuf, "Ending time: ");
			strcat(sendBuf, input.endTime);
			strcat(sendBuf, "\n");

			strcat(sendBuf, "Room:  ");
			strcat(sendBuf, input.room);
			strcat(sendBuf, "\n");
			
			strcat(sendBuf, "Total places:  ");
			char places[256];
			sprintf(places, "%d", input.places);
			strcat(sendBuf, places);
			strcat(sendBuf, "\n");
			
			strcat(sendBuf, "Remaining places:  ");
			char remaining[256];
			sprintf(remaining, "%d", input.remainingPlaces);
			strcat(sendBuf, remaining);
			
			strcat(sendBuf, "\n");
		
			send(fd, sendBuf, BUF, 0);

		
	}
	

	
	fclose(infile);
	send(fd, "end", BUF, 0);


}




int main(int argc, char *argv[])
{
	int fd = atoi(argv[1]);
	char recvBuf[BUF];
	char sendBuf[BUF];
	

	
	int exit = 0;
	
	while(!exit)
	{
	recv(fd, recvBuf, BUF, 0); //receives user option
	
	
	if(strcmp(recvBuf, "createAdminAccount") == 0)
	{
		createAdminAccount(fd);
		
	}
	
	else if(strcmp(recvBuf, "createUserAccount") == 0)
	{
		createUserAccount(fd);	
		
	}
	
	else if(strcmp(recvBuf, "createRoom") == 0)
	{
		createRoom(fd);
		
	}
	
	else if(strcmp(recvBuf, "seeRooms") == 0)
	{

		seeRooms(fd);
		
	}
	
	else if(strcmp(recvBuf, "addMovie") == 0)
	{
		addMovie(fd);
	}
	
	else if(strcmp(recvBuf, "seeMovies") == 0)
	{
		seeMovies(fd);
	}
	
	else if(strcmp(recvBuf, "exit") == 0)
	{
		exit = 1;
		printf("exit \n");
	}
	
    }
	
		
}
