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
#include"./../headers/movieId.h"
#include"./../headers/reservation.h"
#include<errno.h>
#include <dirent.h>
#define PORT 8080
#define BUF  1024

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

void rescheduleReservations(char username[BUF])
{
	printf("RESCHEDULING RESERVATIONS \n");

	FILE *reservationFile;
	char reservationPath[BUF] = "\0";
	sprintf(reservationPath, "./data/%s/reservations", username);
	reservationFile = fopen(reservationPath, "a+");
	
	FILE *futureReservations;
	char futureResPath[BUF] = "\0";
	sprintf(futureResPath, "./data/%s/futureRes", username);
	futureReservations = fopen(futureResPath, "w");
	Reservation reservation;
	while(fread(&reservation, sizeof(Reservation), 1, reservationFile))
	{
	
		//get reserervation date date
		char resDate[20]; 
		strcpy(resDate, reservation.date);
		char resTime[20]; 
		strcpy(resTime, reservation.startTime);
		
		
		time_t rawtime;
   		struct tm *info;
   		char currentDate[80];
   		char currentTime[6];


   		time( &rawtime );	
	  	info = localtime( &rawtime );
	
  		strftime(currentDate,80,"%d/%m/%Y", info);
  		strftime(currentTime,6,"%H:%M", info);
  		
  		int dateComparison = compareDates(currentDate, resDate);
  		
  		//printf("\n \ncurrent time: %s ", currentTime);
  		//printf("movie time: %s \n\n", movieTime);
  		int timeComparison = compareTimes(currentTime, resTime);
  		
  		//printf("Cureent date %s \n", currentDate);
  		//printf("Movie date: %s \n", movieDate);
  		//printf("date compare %d \n", dateComparison);
  		
  		//printf("Cureent time %s \n", currentTime);
  		//printf("Movie time: %s \n", movieTime);
  		//printf("time compare %d \n", timeComparison);
  		
  		
  		
  		if(dateComparison > 0)
  		{
  			//printf("writing to future movies \n");
			fwrite (&reservation, sizeof(Reservation), 1, futureReservations);
  		}
  		
  		if(dateComparison == 0 && timeComparison > 0)
  		{
  			//printf("writing to future movies \n");
			fwrite (&reservation, sizeof(Reservation), 1, futureReservations);
  		}
	}
	
	
	fclose(futureReservations);
	fclose(reservationFile);
	

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

/*
uses username arg to keep track of the name of the user that logged for further usage
*/
int login(int fd, char globalUsername[BUF], char userType[6]) 
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
		
		if(strcmp(username, ":exit") == 0)
			return 2;
			
		printf("received username: %s \n", username);
		
		
		recv(fd, password, BUF, 0); //receives password;
		printf("received password: %s \n", password);
		if(strcmp(password, ":exit") == 0)
			return 2;
		
		strcpy(globalUsername, username);
	
		User input;
		while(fread(&input, sizeof(User), 1, infile))
		{
		

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
			//recv(fd, recvBuf, BUF, 0); //receives admin login
			printf("bad credentials \n");
		}
		fclose(infile);
	}
}


void createUser(int fd)
{
	FILE *file;
	file = fopen("data/users", "a+");
	
	char username[BUF];
	int unique = 1;
	
	do
	{
		unique = 1;
		recv(fd, username, BUF, 0);
		if(strcmp(username,":exit") == 0)
		{
			return;	
		}
	
		printf("received username: %s \n", username);
	
		User input;
		while(fread(&input, sizeof(User), 1, file))
		{

			if(strcmp(input.username, username) == 0)
			{
				unique = 0;
			}
	
		}
	
		if(unique)
		{

			send(fd, "ok", BUF, 0);
		
		}
		else
		{
			send(fd, "usernameTaken", BUF, 0);
		}
	}while(!unique);
	
	char password[BUF];
	recv(fd, password, BUF, 0);
	
	if(strcmp(password,":exit") == 0)
	{
			return;	
	}
	
	User user;
	
	strcpy(user.username, username);
	strcpy(user.password, password);
	
	fwrite (&user, sizeof(User), 1, file);
	
	fclose(file);	
	
	
	

	
	
}

void reservation(int fd, char username[BUF])
{

 	//How it works:
 	/*
 	
 		create a folder for the user, create a file that will containt structs associating each movie an id
 		send structs to the user, user picks ID, check to what movie the id matches
 		find the corresponding movie in the scheduled. 
 			
 		update the remaning number of places
 		write the updated movie to a temp file
 		make a copy of the original movie file
 		override it and adding to it the updated movie
 		
 		create a reservation struct that holds the reservation made by a user and save it to a file
 		
 	*/

	char recvBuf[BUF];

	reschedule();
	printf("reservation done by: %s \n", username);
	char command[BUF];
	sprintf(command, "mkdir ./data/%s", username); //command to create a folder
	char idsPath[BUF];
	sprintf(idsPath, "./data/%s", username);
	DIR* dir = opendir(idsPath);
	if (dir) {
 	  printf("directory exists \n");
  	  closedir(dir);
	} 
	else if(ENOENT == errno) 
	{
  		printf("created dirtectory \n");
  		system(command);
	} 		
	

	
	//create a file that will store movie associations with ID s
	FILE *movieIdFile;
	char filePath[BUF];
	sprintf(filePath, "./data/%s/ids", username);
	movieIdFile = fopen(filePath, "w+");
	

	FILE *futureMovies;
	futureMovies = fopen("data/futureMovies", "r");
	
	int choosenMovieOk = 0;	
	do
	{
		Movie input;
		int id = 1;
		while(fread(&input, sizeof(Movie), 1, futureMovies))
		{
			if(input.remainingPlaces > 0)
			{
				MovieId movieId;
				movieId.id = id;
				strcpy(movieId.name, input.name);
				
				fwrite (&movieId, sizeof(MovieId), 1, movieIdFile);
				
				char toSend[2048];
				sprintf(toSend, "%d. %s \n", id, input.name);
				printf("sending: %s \n", toSend);
				send(fd, toSend, BUF, 0);
		
				id++;
			}
		}
		printf("end \n \n");
		if(id == 1) //no movie available to make reservations.
		{
			send(fd, "nothigToDisplay", BUF, 0);
			return;
		}
		send(fd, "end", BUF, 0);

		recv(fd, recvBuf, BUF, 0); //receives movie id chosen by user
		if(strcmp(recvBuf, ":exit") == 0)
		{
			return;
		}
	
		
		int aa = atoi(recvBuf);
		printf("choosen id: %d \n", aa);
		if(aa > id)
		{
			send(fd, "movieNotAvailable", BUF, 0);
			printf("error \n");
			choosenMovieOk = 0;
			rewind(futureMovies);
			fclose(movieIdFile)	;
			movieIdFile = fopen(filePath, "w+");
		}
		else
		{
			printf("OK ! \n");
			send(fd, "movieOk", BUF, 0);
			choosenMovieOk = 1;
		}
		
	}while(!choosenMovieOk);

	

	char choosenMovie[BUF] = "\0";
	MovieId inId;
	int choosenId = atoi(recvBuf);

	rewind(movieIdFile);	
	
	//get the name of choosen movie
	while(fread(&inId, sizeof(MovieId), 1, movieIdFile))
	{
	
		if(inId.id == choosenId)
		{

			strcpy(choosenMovie, inId.name);
		}
		
	}
	
	int count = 0; //count how many times the choosen movie is shown
	rewind(movieIdFile);
	while(fread(&inId, sizeof(MovieId), 1, movieIdFile))
	{
		if(strcmp(inId.name, choosenMovie) == 0)
		{
			count++;
		}
	}
	
	printf("Choosen movie is shown %d  times \n", count);

	
	Movie detailsArr[count];
	rewind(futureMovies);
	Movie details;
	int index = 0;
	
	while(fread(&details, sizeof(Movie), 1, futureMovies))
	{

		if(strcmp(details.name, choosenMovie ) == 0)
		{

			detailsArr[index] = details;
			index ++;

		}	

	}
	
	printf("count: %d \n", count);
	for(int i = 0; i<count; i++)
	{
		char sendBuf[BUF] = "\0";
		printf("sending details %d \n", i);
		char j[10];
		sprintf(j, "%d. \n", i+1);
		strcat(sendBuf, j);
		
		strcat(sendBuf, "Name: ");
		strcat(sendBuf, detailsArr[i].name);
		strcat(sendBuf, "\n");
		


		strcat(sendBuf, "Date: ");
		strcat(sendBuf, detailsArr[i].date);
		strcat(sendBuf, "\n");
		
		strcat(sendBuf, "Starting time: ");
		strcat(sendBuf, detailsArr[i].startTime);
		strcat(sendBuf, "\n");
		
		strcat(sendBuf, "Ending time: ");
		strcat(sendBuf, detailsArr[i].endTime);
		strcat(sendBuf, "\n");

	
		strcat(sendBuf, "Room: ");
		strcat(sendBuf, detailsArr[i].room);
		strcat(sendBuf, "\n");
		
		strcat(sendBuf, "Remaining places: ");
		char k[10];
		sprintf(k, "%d \n", detailsArr[i].remainingPlaces);
		strcat(sendBuf, k);
		strcat(sendBuf, "\n");
		
		
		send(fd, sendBuf, BUF, 0);
	
	}
	
	send(fd, "end", BUF, 0);
	
	
	int detailsOk = 1;
	do
	{
		recv(fd, recvBuf, BUF, 0);
		printf("Choosen details: %s \n ", recvBuf); //receives index of choosen details
		
		if(strcmp(recvBuf, ":exit") == 0)
		{
			return ;
		}
	
		index = atoi(recvBuf) - 1;
		printf("index: %d \n", index);
	
		if(index >= count || index < 0)
		{
			send(fd, "choiceNotAvailable", BUF, 0);
			detailsOk = 0;
			printf("not available \n");
		}
		else
		{
			send(fd, "detailsOk", BUF, 0);
			detailsOk = 1;
			printf("ok \n");
		}
	}while(!detailsOk);

	
	fclose(movieIdFile);
	fclose(futureMovies);
	
	Movie m = detailsArr[index];
	printf("choosen details.date: %s \n",m.date );
	printf("choosen details.start: %s \n",m.startTime );	

	
	
	
	printf("so far so good \n");
	m.remainingPlaces --;
	
	char path[BUF] ="\0";
	sprintf(path, "./data/%s/tempRsrv", username);
	FILE *userTempReservation = fopen(path, "w+");
	fwrite (&m, sizeof(Movie), 1, userTempReservation);
	
	//create a copy of the movies file	
	system("cp ./data/movies ./data/moviescpy");
	
	//open original movies file and override it with updated data
	FILE *originalMovies = fopen("./data/movies", "w");
	FILE *tempCopy = fopen("./data/moviescpy", "r");

	
	Movie readMovie;
	while(fread(&readMovie, sizeof(Movie), 1, tempCopy))
	{
		//m is the movie choosen by user
		if(strcmp(readMovie.name, m.name) == 0 &&
		   strcmp(readMovie.date, m.date) == 0 &&
		   strcmp(readMovie.startTime, m.startTime) == 0 &&		   
		   strcmp(readMovie.room, m.room) == 0 )
		   {
			fwrite (&m, sizeof(Movie), 1, originalMovies);
		   }
		 else
		 {
		 	fwrite (&readMovie, sizeof(Movie), 1, originalMovies);
		 }
		    
		
	}
	fclose(originalMovies);
	fclose(tempCopy);
	fclose(userTempReservation);
	reschedule();
	
	system("rm ./data/moviescpy");  //remove temporary file
	
	//create reservation data for the user
	char reservationPath[BUF];
	sprintf(reservationPath, "./data/%s/reservations", username);
	
	FILE *reservationFile = fopen(reservationPath, "a+");
	Reservation reservation;
	strcpy(reservation.username, username);
	strcpy(reservation.movie, m.name);  //m is the movie choosen by the user
	strcpy(reservation.date , m.date);
	strcpy(reservation.startTime, m.startTime);
	strcpy(reservation.endTime , m.endTime);
	strcpy(reservation.room , m.room);
	
	fwrite(&reservation, sizeof(Reservation), 1, reservationFile);
	fclose(reservationFile);
	
	
}

void seeReservation(int fd, char username[BUF])
{
	printf("see reservations \n");
	char command[BUF];
	sprintf(command, "mkdir ./data/%s", username); //command to create a folder
	char idsPath[BUF];
	sprintf(idsPath, "./data/%s", username);
	printf("username: %s", username);
	DIR* dir = opendir(idsPath);
	if (dir) {
 	  printf("directory exists \n");
  	  closedir(dir);
	} 
	else if(ENOENT == errno) 
	{
  		printf("created dirtectory \n");
  		system(command);
	} 		
	
	rescheduleReservations(username);
	
	
	FILE *reservationFile;
	char path[BUF];
	sprintf(path, "./data/%s/futureRes", username);
	
	reservationFile = fopen(path, "r+");
	
	char sendBuf[BUF] = "\0";
	Reservation reservation;
	
	//cehck if file contains data or exists;
 	
 	int count = 0;
	while(fread(&reservation, sizeof(Reservation), 1, reservationFile))
	{
		printf("reading \n");
		strcat(sendBuf, "Movie: ");
		strcat(sendBuf, reservation.movie);
		strcat(sendBuf, " \n");
		
		printf("reservation.movie: %s \n", reservation.movie);
		
		strcat(sendBuf, "Date: ");
		strcat(sendBuf, reservation.date);
		strcat(sendBuf, " \n");
		
		strcat(sendBuf, "Start time: ");
		strcat(sendBuf, reservation.startTime);
		strcat(sendBuf, " \n");
		
		strcat(sendBuf, "End time: ");
		strcat(sendBuf, reservation.endTime);
		strcat(sendBuf, " \n");
		
		strcat(sendBuf, "Room: ");
		strcat(sendBuf, reservation.room);
		strcat(sendBuf, " \n");
		
		count ++;
		printf("SEE RESERVATIONSSSSS !!!! %s", sendBuf);
		send(fd, sendBuf, BUF, 0);
		sendBuf[0] = '\0';
	}
	printf("count: %d \n", count);
	if(count == 0)
	{
		printf("no data \n");
		send(fd, "noData", BUF, 0);
	}
	else
	{
		send(fd, "end", BUF, 0);
	}
	//TODO: 
	/*
		 modify the remaining number of places
		 write change to temp file
		 update movies file
		 reschedule
		5. DONE:create a reservation struct that contains username, movie and details
		6. DONE: handle exit message
		7. DONE: check number of places before showing a movie for reservation
		8. DONE: allow user to see his own reservations
		9. create a reschedule to avoid showing past reservations
	
	*/
	fclose(reservationFile);
	
}

int main(int argc, char *argv[])
{
	int fd = atoi(argv[1]);

 	//receiveing user option
 	int exit = 0;
 	do
 	{
 		char recvBuf[BUF];
	 	recv(fd, recvBuf, BUF, 0); //receives option
 	
 	
 		if(strcmp(recvBuf, "login") == 0)
 		{	
 			printf("SHOULD LOG IN \n");
 			char username[BUF];
 			int a = login(fd, username, "users");


			if(a == 1)
			{
				//while user is not logged out
				int logout = 0;
				do	
				{	
					 recv(fd, recvBuf, BUF, 0); //receives option after login
				
					if(strcmp(recvBuf, "seeMovies") == 0)
					{
						seeMovies(fd);
					}
					else if(strcmp(recvBuf, "reservation") == 0)
 					{
 						reservation(fd, username);
 					}
 					else if(strcmp(recvBuf, "seeReservation") == 0)
 					{
 						seeReservation(fd, username);
 					}
					else if(strcmp(recvBuf, "logout") == 0)
					{
						logout = 1;
					}
				}while(!logout);
 			}
 		}
 	
 		if(strcmp(recvBuf, "create") == 0)
 		{
 			createUser(fd);
 		}

 		else if(strcmp(recvBuf, "exit") == 0)
 		{
			printf("exiting ... \n");
			exit = 1;
 			
 		}
	}while(!exit);
	
}
