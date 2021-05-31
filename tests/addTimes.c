#include<stdio.h>
#include<stdlib.h>
#include<string.h>

void addTimes(char resBuf[10], char initialTime[10], char duration[10])
{
  	//decompose initial time
	char hchr[3]= "\0";
	strncpy(hchr, initialTime, 2);
	
	char mchr[3] ="\0";
	strncpy(mchr, &initialTime[3], 2);
	
	int h = atoi(hchr);
	int m = atoi(mchr);
	
	
	//decompose duration:
	char dhchr[3]= "\0";
	strncpy(dhchr, duration, 2);
	
	char dmchr[3] ="\0";
	strncpy(dmchr, &duration[3], 2);
	
	int dh = atoi(dhchr);
	int dm = atoi(dmchr);
	
	
	int resh = h + dh;
	int resm = m + dm;
	
	if(resm > 59)
	{
		resm = resm % 60;
		resh++;
	}
	
	resh = resh % 24;
	
	
	if(resh<10 && resm >= 10)
	sprintf(resBuf, "0%d:%d", resh, resm);
	
	if(resh>=10 && resm<10)
	sprintf(resBuf, "%d:0%d", resh, resm);
	
	if(resh<10 && resm < 10)
	sprintf(resBuf, "0%d:0%d", resh, resm);
	
	if(resh>= 10 && resm>10)
	sprintf(resBuf, "%d:%d", resh, resm);


	

	
	
}
int main()
{
	char startTime[7];
	printf("Enter start time: ");
	scanf("%s", startTime);
	
	char endTime[10] = "\0";
	
	addTimes(endTime, startTime, "02:00");
	
	
	printf("End time: %s \n",endTime);
}
