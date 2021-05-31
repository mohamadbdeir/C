#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"../headers/user.h"
int main()
{
	FILE *infile;

	infile = fopen("../data/users", "r");
	
	User input;
	while(fread(&input, sizeof(User), 1, infile))
	{
		printf("username: %s \n password:  %s \n", input.username, input.password);
	}
	

	fclose(infile);
	
}
