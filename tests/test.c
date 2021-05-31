#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#define BUF 1024
int main()
{
	 char duration[10] = "\0";
		 
		 int durationValid = 1;
		 do
		 {
		 	printf("looooooooooop \n");
		 	durationValid = 1;
		 	printf("Enter duraion:  \n");
		 	fgets(duration, 10, stdin);
			duration[strcspn(duration, "\n")] = 0;	
			
			for(int i = 0; i<strlen(duration); i++)
			{
				if(!isdigit(duration[i]) && (char)duration[i] != 58)
				{
					system("clear");
					printf("duration not valid");
					durationValid = 0;
					continue;
					
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
			
			printf("h: %d \n", h);
			printf("m: %d \n", m);
			
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
		 
		 printf("Duration ok: %s, \n", duration);
		 
}
