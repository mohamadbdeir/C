#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"headers/user.h"
int main()
{
	FILE *infile;

	infile = fopen("data/admin", "r");
	
	char username[1024] = "admin";
	char password [1024] = "admin";
	
	User input;
	while(fread(&input, sizeof(User), 1, infile))
	{
		if(strcmp(input.username, username) == 0)
		{
			if(strcmp(input.password, password) == 0 )
				printf("connected \n");
		}
	}
	printf("failed \n");
	

	fclose(infile);
	
}
