#include"../headers/user.h"
#include"../headers/room.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
int main()
{
	Room room;
	FILE *outfile;
	
	outfile = fopen("../data/rooms", "a+");
	
	if (outfile == NULL)
   	 {
        fprintf(stderr, "\nError opend file\n");
        exit (1);
    	}
	
	char name[50];
	printf("enter name: \n");
	scanf("%s", name);
	
	int capacity;
	printf("enter capacity: \n");
	scanf("%d", &capacity);
	
	strcpy(room.name, name);
	room.capacity = capacity;
	
	printf("Writing: name : %s \n capacity : %d \n", room.name, room.capacity);
	
	fwrite (&room, sizeof(room), 1, outfile);
	
	
	
	fclose(outfile);
	
}
