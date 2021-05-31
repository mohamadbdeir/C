int checkTime(char time[6])
{

	for(int i = 0; i<5; i++)
	{
		if(i !=2 && !isdigit(time[i]))
			return 0;
	}
	
	char hours[3];
	hours[0] = time[0];
	hours[1] = time[1];
	
	int h = atoi(hours);
	

	
	if(h <0 || h>23)
		return 0;
	
	char minutes[3];
	minutes[0] = time[3];
	minutes[1] = time[4];
	
	int m = atoi(minutes);

	if(m < 0 || m > 59)
		return 0;
		
	return 1;
}



int checkDate(int dd, int mm, int yy)
{


if(yy>=1900 && yy<=9999)
    {
        //check month
        if(mm>=1 && mm<=12)
        {
            //check days
            if((dd>=1 && dd<=31) && (mm==1 || mm==3 || mm==5 || mm==7 || mm==8 || mm==10 || mm==12))
		{

		           return 1;
		}
            else if((dd>=1 && dd<=30) && (mm==4 || mm==6 || mm==9 || mm==11))
		{

		           return 1;		}
            else if((dd>=1 && dd<=28) && (mm==2))
            {

		           return 1;
	    }
            else if(dd==29 && mm==2 && (yy%400==0 ||(yy%4==0 && yy%100!=0)) && yy < 2100)
                {

		           return 1;
                }
                
            else
               {

                return 0;
               }
        }
        else
        {

            return 0;
        }
    }
    else
    {

        return 0;
    }
}

int compareDates (char date1[11], char date2[11])
{


	char ddchr1[3];
	ddchr1[0] = date1[0];
	ddchr1[1] = date1[1];
	
	int dd1 = atoi(ddchr1);
	
	char mmchr1[3];
	mmchr1[0] = date1[3];
	mmchr1[1] = date1[4];
	
	int mm1 = atoi(mmchr1);
	
	char yychr1[5];
	yychr1[0] = date1[6];
	yychr1[1] = date1[7];
	yychr1[2] = date1[8];
	yychr1[3] = date1[9];
	
	int yy1 = atoi(yychr1);
	

	
	////////////////////////////// DATE 1
	
	char ddchr2[3];
	ddchr2[0] = date2[0];
	ddchr2[1] = date2[1];
	
	int dd2 = atoi(ddchr2);
	
	char mmchr2[3];
	mmchr2[0] = date2[3];
	mmchr2[1] = date2[4];
	
	int mm2 = atoi(mmchr2);
	
	char yychr2[5];
	yychr2[0] = date2[6];
	yychr2[1] = date2[7];
	yychr2[2] = date2[8];
	yychr2[3] = date2[9];
	
	int yy2 = atoi(yychr2);
	

	
	if(yy2 > yy1)
	{
		return 1;
	}
	else if(yy1 > yy2)
	{	 
		return -1;
	}
	
	else
	{

		if(mm2 > mm1)
		{
			return 1;
		}
		else if(mm1> mm2)
		{
			return -1;
		}
		else
		{

			if(dd2> dd1)
			{
				return 1;
			}
			else if(dd1> dd2)
			{
				return -1;
			}
			else
				return 0; //both dates are equal
		}
	}	
	
}




int compareTimes(char time1[20], char time2[20])
{
	

	char h1c[10] = "\0";
	strncpy(h1c, time1, 2);
	
	int h1 = atoi(h1c);
	//printf("h1: %d \n", h1);
	
	char m1c[10] = "\0";
	strncpy(m1c, &time1[3], 2);
	
	int m1 = atoi(m1c) ;
	//printf("m1: %d \n", m1);
	
	char h2c[10] = "\0";
	strncpy(h2c, time2, 2);
	
	int h2 = atoi(h2c);
	//printf("h2: %d \n", h2);
	
	char m2c[10] = "\0";
	strncpy(m2c, &time2[3], 2);
	
	int m2 = atoi(m2c);
	//printf("m2 : %d \n", m2);
	
	
	
	if(h2>h1)
	{
		//printf("time 2 >> \n");
		return 1;
	}
	else if(h1>h2)
	{
		//printf("time 1 hours >> \n");
		return -1;
	}
	else
	{
		if(m2 > m1)
		{
			//printf("time 2 >> \n");
			return 1;
		}
		else
		if(m1 > m2)
		{
			//printf("time 1 minutes>> \n");	
			return -1;
		}
		else 
			return 0;
	}
	
}

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























