#include"../headers/user.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
int main()
{
	User user;
	FILE *outfile;
	
	outfile = fopen("../data/admin", "a+");
	
	if (outfile == NULL)
   	 {
        fprintf(stderr, "\nError opend file\n");
        exit (1);
    	}
	
	char username[50] = "admin";
	char password[50] = "admin";
	
	
	strcpy(user.username, username);
	strcpy(user.password, password);
	
	fwrite (&user, sizeof(User), 1, outfile);
	
	printf("admin account initialized \n");
	
	fclose(outfile);
	
}
