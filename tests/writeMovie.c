#include"../headers/movie.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
int main()
{
	Movie movie;
	FILE *outfile;
	
	
	char name[1024];
	char date[1024];
	char startTime[1024];
	char endTime[1024];
	char room[1024];
	int places;
	
	outfile = fopen("../data/movies", "a+");
	
	if (outfile == NULL)
   	 {
        fprintf(stderr, "\nError opend file\n");
        exit (1);
    	}
	

	printf("enter name: \n");
	scanf("%s", name);
	
	strcpy(movie.name, name);
	
	printf("enter date: \n");
	scanf("%s", date);
	strcpy(movie.date, date);
	
	printf("start time: \n");
	scanf("%s", startTime);
	strcpy(movie.startTime, startTime);
	
	printf("end time:\n");
	scanf("%s", endTime);
	strcpy(movie.endTime, endTime);
	
	printf("Room: \n");
	scanf("%s", room);
	strcpy(movie.room, room);
	
	printf("places: \n");
	scanf("%d", &places);
	movie.places = places;
	
	fwrite (&movie, sizeof(Movie), 1, outfile);
	
	
	fclose(outfile);
	
}
