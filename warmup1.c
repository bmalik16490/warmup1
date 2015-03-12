#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<limits.h>
#include<sys/stat.h>
#include<ctype.h>
#include <errno.h>
#include "my402list.h"
#include "warmup1.h"

static void DisplayErrorMessage()
{

fprintf(stderr," Valid commands: \n cat filename | ./warmup1 sort \n ./warmup1 sort filename \n ./warmup1 sort ReadFromstdin\n");
exit(1);

}

//Parses each line for its correctness.
static void FormatInputLine( char *buf, TransElem *tElem)
{
	char *tab, *fields[4], description[25], amt[7], *loc, *start;
	int tabCount, i, len,amount;	
	time_t current;
	//unsigned long long t;
	unsigned int uiTime;
	long val1,val2;
	char *ptr=NULL;

	i=0;
	uiTime=0;
	fields[0]=start=buf;
	tabCount = 0;
	//t=0;

	/* *************store each field************** */
	while((tab=strchr(fields[tabCount],'\t'))!=NULL)
	{	
		*tab++='\0'; //replace '\n' by '\0'
		fields[++tabCount]=tab;
	}	
	
	/* ************* file format error ************ */	
	if(tabCount != 3)
	{
		fprintf(stderr," Error: File not in proper format. Each line should have exactly 4 fields.\n");
		DisplayErrorMessage();
	}
	else
		len=strlen(fields[3]);
	/* ************ trim spaces ************ */	
	if(len != 0)
	{	
		while((fields[3][i]==' ' || fields[3][i]=='\n') && fields[3][i] != '\0')
		{
		
			len--;
			i++;	
		
		}
	}
	/* ************* file format error: No description ************ */	
	
	if(len==0)
	{
		fprintf(stderr," Error: File not in proper format. Description is mandatory.\n");
		DisplayErrorMessage();
		
	}
	if(len>25)
		len=25;	//trim description
			
	strncpy(description, &(fields[3][i]), len); 
	description[len-1]='\0'; 		
	strcpy(tElem->desc,description);
		
	/* *****************parse sign**************** */
	
	amount=atoi(fields[2]);	
	if(strlen(fields[0])==0)
	{
		fprintf(stderr," Error: No sign \n");
		DisplayErrorMessage();
	}	

	if(*fields[0] == '-' || *fields[0] == '+')
	{
		if(*fields[0] == '-') 
			tElem->sign= -1;
		else
			 tElem->sign=1;
		
	}
	else
	{
		fprintf(stderr," Error: Only - or + sign allowed.\n");
		DisplayErrorMessage();
	}	

	
	/* *************parse amount************ */
	
	/* ************1. if amount more than 10 million throw error*************** */
	if((len=strlen(fields[2]))>10)
	{
		fprintf(stderr," Error: Amount should be less than 10 million.\n");
		DisplayErrorMessage();
	}
	

	i=0;
	tabCount=0;	
			
	amount=atoi(strncpy(amt,fields[2],7));
	amount*=100;
	loc=strchr(fields[2], '.');
	loc++;	
	while(loc[i] != '\0')
	{
		tabCount++;
		i++;

	}

	/* ********************2. if more than 2 decimal places throw error****************** */
	if(tabCount>2)
	{
		fprintf(stderr," Error: Amount should have exact 2 digits after the decimal(.) \n");
		DisplayErrorMessage();
	}

	amount+=atoi(loc);
	tElem->amt=amount;



	/* *****************parse time************** */


	
	current=time(NULL); //get current time
	
	/* ****************1. timestamp length too long******************* */ 
	if(strlen(fields[1]) > 10 )
	{
		fprintf(stderr," Error: Timestamp is too long.\n");
		DisplayErrorMessage();
	}
	
	/* ****************2. no timestamp ******************* */ 	
	if(strlen(fields[1]) ==0)
	{
		fprintf(stderr," Error: No Timestamp.\n");
		DisplayErrorMessage();
	}

	/* *************3. time stamp not a number ************* */
	for(i=0;i< strlen(fields[1]); i++)
	{	if(!isdigit(fields[1][i]))
		{
			fprintf(stderr," Error: Timestamp is incorrect. It should be a number.\n");
			DisplayErrorMessage();
		}
	}

	/* ****************4. timestamp should be a valid integer ******************* */ 
	val1=strtoul(fields[1], &ptr, 10);
	val2=strtoul(ptr, NULL, 10);
	if(!(val2==0 && errno!=ERANGE)) // doesnot overflow and is a valid positive integer
	{
		fprintf(stderr," Error: Timestamp is incorrect. It should be a valid integer(0- 4294967295).\n");
		DisplayErrorMessage();

	}
	else
	{
		uiTime= val1;
	}
	
	/* *******************5. timestamp greater than current time stamp ************** */
	
	if(uiTime>current)
	{
		fprintf(stderr," Error: Timestamp is incorrect. It should be less than current timestamp.\n");
		DisplayErrorMessage();
	}
	
	tElem->transtime=uiTime;
}


// Reads input from file. Prints error if line more than 1024 chars including \0

static void ReadInput(FILE *f, My402List* myList)
{
	char buf[1025];
	TransElem  *tElem;
	My402ListElem *current;

	tElem=(TransElem*)malloc(sizeof(TransElem));
	current =(My402ListElem*)malloc(sizeof(My402ListElem));
	
	/* *************** check if line of 1024 chars************ */
	while(fgets(buf,1026,f)!=NULL)
	{	
		if(strlen(buf)>1024)
		{
			fprintf(stderr,"Error: Reading input line of more than 1024 characters.\n");
			DisplayErrorMessage();
		}

	//now initialize transaction element, make all checks, add to list

	if(!(tElem=(TransElem*)malloc(sizeof(TransElem))))
	{
		fprintf(stderr,"Error while initializing the bank's transaction element. Exiting.\n");
		exit(1);
	}

	/* ************Format input line********** */
	FormatInputLine(buf, tElem);
		
	/* **************check if timestamp alreadt present*************** */	
	current=My402ListFirst(myList);//get first element
		
	do
	{
		if(((TransElem*)(current->obj))->transtime==tElem->transtime) 
		{
			fprintf(stderr,"Error: Timestamp already exists.\n");
			DisplayErrorMessage();
		}
	}while((current=My402ListNext(myList, current))!=NULL); 

	/* ***********************append node****************** */
	My402ListAppend(myList,(void*)tElem);
		
	}		
	
}

//sort list using bubble sort
static void SortList(My402List *myList)
{
	int i,j,k;
	time_t ttime;	
	char *strTime;
	char finalTime[16];
	My402ListElem *elem1, *t, *elem2, *elem3, *first;

	first=My402ListFirst(myList);
	t=(My402ListElem *)malloc(sizeof(My402ListElem));
	
	/* ***************** Sort using bubble sort ************** */
	
	for( elem1=first,i=0; i<My402ListLength(myList);i++,elem1=elem1->next)
	{	for(elem2=elem1->next, j=i+1;j<My402ListLength(myList);j++, elem2=elem2->next)
		{
					
			if( ((TransElem*)(elem1->obj))->transtime > ((TransElem*)(elem2->obj))->transtime )
			{
				t->obj=elem1->obj;
				elem1->obj=elem2->obj;
				elem2->obj=t->obj;				
					
			}
			
		}
	
	
	}
	elem3=My402ListFirst(myList);

	/* ************** converts time to required format **************** */
	for(k=0;k<My402ListLength(myList);k++, elem3=elem3->next)
	{	
		ttime=((TransElem *)(elem3->obj))->transtime;
		strTime=ctime(&ttime);
		j=0;
		for(i=0;i<strlen(strTime);i++)
		{	
			if(i>9 && i<19 ) 
				continue;
		      finalTime[j]=strTime[i];
		      j++;
		}	
		finalTime[15]='\0';		
		strncpy(((TransElem*)(elem3->obj))->date,finalTime,16);
		((TransElem*)(elem3->obj))->date[15]='\0';
			
	
	/* ********************* computes balance *************** */

		if(k==0)
		{
			((TransElem*)(elem3->obj))->bal=(((TransElem*)(elem3->obj))->amt)*(((TransElem *)(elem3->obj))->sign);
		}
		else
		{
			((TransElem*)(elem3->obj))->bal = ( (((TransElem *)(elem3->obj))->amt)*(((TransElem *)(elem3->obj))->sign) ) + ((TransElem *)(elem3->prev->obj))->bal;
			
		}	
	}
	
}	

//converts amount ot proper format and displays it
static void DisplayAmount(int bamount, int aamount, unsigned sign)
{

	int j, i=0;
	char buff1[13], buff2[13], buff4[3], buff3[13], finalbuf[15];
	memset(buff2,'x',13);
	memset(buff3,'\0',13);
	buff2[12]='\0';
	buff2[1]=buff2[5]=',';
	buff2[9]='.';
	memset(finalbuf,' ',14);
	finalbuf[14]='\0';
	
	snprintf(buff1,13,"%d",bamount);
	j=9;
	i=strlen(buff1)-1;
	while(i>-1)
	{	
		j--;
		if(buff2[j]==',')
		{	
			continue;
		}
		buff2[j]=buff1[i];
		i--;
	
	}	
	if(aamount>-1 && aamount<10)
	{	
		buff2[10]='0';
		snprintf(buff4,2,"%d",aamount);
		buff2[11]=buff4[0];

	}
	else
	{
		snprintf(buff4,3,"%d",aamount);
		buff2[10]=buff4[0];
		buff2[11]=buff4[1];
	}

	i=0,j=0;
	for(i=0;i<13;i++)
	{
		if(buff2[i]!='x')
		{
			if(i!=0 && buff2[i]==',')
			{	if(buff2[i-1]=='x')
					continue;
				buff3[j]=buff2[i];
				j++;
			}
			else
			{
				buff3[j]=buff2[i];
				j++;
			}
		}
	}		


	if(sign==-1)
	{
		finalbuf[0]='(';
		finalbuf[13]=')';
	}
	else
	{
		finalbuf[0]=' ';
		finalbuf[13]=' ';

	}

	i=13-strlen(buff3);
	j=0;
	while(buff3[j]!='\0')
	{
		finalbuf[i]=buff3[j];
		j++;
		i++;

	}	
	fprintf(stdout,"| %s ",finalbuf);
}

//Displays table
static void Display(My402List *myList)
{
	int k, bamount=0, aamount=0;
	My402ListElem *elem;
	int absoluteSum, z=100;
	char *q="?,???,???.??";
	int len;

	fprintf(stdout, "%s\n", "+-----------------+--------------------------+----------------+----------------+");
	fprintf(stdout, "%s\n", "|       Date      | Description              |         Amount |        Balance |");
	fprintf(stdout, "%s", "+-----------------+--------------------------+----------------+----------------+");
	
	elem=My402ListFirst(myList);
	for(k=0;k<My402ListLength(myList);k++)
	{
		len=25;
		fprintf(stdout, "\n| %s | ", ((TransElem *)(elem->obj))->date);
		fprintf(stdout, "%s", ((TransElem *)(elem->obj))->desc);
		len-= strlen(((TransElem *)(elem->obj))->desc);
		while(len>0)
		{	fprintf(stdout, " ");
			len--;
		}


		/* ****************format amount**************** */
		
		
		bamount=(((TransElem *)(elem->obj))->amt)/z;
		
		aamount=(((TransElem *)(elem->obj))->amt)%z;
		
						
		DisplayAmount(bamount,aamount, ((TransElem *)(elem->obj))->sign);
	
		/* ****************format balance******************** */
		
			
			absoluteSum= abs( ((TransElem *)(elem->obj))->bal );	
			if((absoluteSum/z)>10000000)
			{	if(((TransElem *)(elem->obj))->bal <0)
					fprintf(stdout, "|(%s)",q);
				else
					fprintf(stdout, "|(%s) ",q);	
				
			}
			else
			{
				bamount=absoluteSum/z;
				aamount=absoluteSum%z;
				if(((TransElem *)(elem->obj))->bal <0)
					DisplayAmount(bamount,aamount, -1);
				else
					DisplayAmount(bamount,aamount,1);
				
			}		
			fprintf(stdout, "|");
	
		elem=elem->next;
	}	
	fprintf(stdout, "\n%s\n", "+-----------------+--------------------------+----------------+----------------+");
}

//main
int main(int argc, char** argv )
{
	My402List myList;
	FILE * f=NULL;
	struct stat sstat; //to check if the path is a directory or a file
	
	if(argc >=2)
	{
		if(strcmp(argv[1], "sort") ==0)
		{
			if(argc == 2)
			{	
				f=stdin;
			}
			else if(argc==3)
			{
				if(!(f=fopen(argv[2],"r")))
				{
					if(stat(argv[2], &sstat)==-1)
					{
						perror("Error: ");
						DisplayErrorMessage();
					}
				}
				if(stat(argv[2], &sstat)==0)
				{
					if(sstat.st_mode & S_IFDIR)
					{
						fprintf(stderr, "Error: Given Path is of a Directory.\n");
						DisplayErrorMessage();
					}
					else if(sstat.st_mode & S_IFREG) //its a file
					{	
						if(!(f=fopen(argv[2],"r")))
						{
							perror("Error: ");						
							DisplayErrorMessage();
						}
						if(sstat.st_size ==0)
						{
							fprintf(stderr,"Error: Input File is empty.\n");
							DisplayErrorMessage();
						}
					}
				}
			
		
			}	
		if(f==NULL)
		{
			fprintf(stderr, "Error: No file as input.\n");
			DisplayErrorMessage();	

		}
		//initialize list
 		if(!My402ListInit(&myList))
		{	
			fprintf(stderr, "Error: Cannot initialize list.\n");
			exit(1);					
		}	

		//read input
		ReadInput(f, &myList);
		//sort input	
		SortList(&myList);
		//Display input
		Display(&myList);	
		fclose(f);	
	}
	else
	{
		fprintf(stderr,"Error: second argument must be sort.\n");
		DisplayErrorMessage();
	}		
	
	if(argv[1][0]== '-')
	{
		fprintf(stderr,"Error: Cannot have - in the argument.\n");
		DisplayErrorMessage();
	}

	}
	else if(argc >3)
	{
		fprintf(stderr, "Error: Too many arguments.\n");
		DisplayErrorMessage();
	}	
	else if(argc <2)
	{

		fprintf(stderr, "Error: Too less arguments.\n");
		DisplayErrorMessage();

	}		
	
	return 0;
}
		
	



