#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"../headers/room.h"
int main()
{
	FILE *infile;

	infile = fopen("../data/rooms", "r");
	
	Room input;
	while(fread(&input, sizeof(Room), 1, infile))
	{
		printf("name: %s \n capacity:  %d \n", input.name, input.capacity);
	}
	

	fclose(infile);
	
}
