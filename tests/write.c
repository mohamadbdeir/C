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
	
	char username[50];
	printf("enter username: \n");
	scanf("%s", username);
	
	char password[50];
	printf("enter password: \n");
	scanf("%s", password);
	
	strcpy(user.username, username);
	strcpy(user.password, password);
	
	fwrite (&user, sizeof(User), 1, outfile);
	
	
	
	fclose(outfile);
	
}
