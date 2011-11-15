/************************
*Program: Myshell
*Written by: John camacho
*
************************/
#define NUMBER_OF_COMMANDS 8
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <ctype.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <termios.h>

extern char **environ;
char* arguments=NULL;
char in[200];
int in_;
char out[200];
int out_;
char app[200];
int app_;
//holds full str
char masterstr[200];
int dont_wait=0;

/*****************
*
*
*******************/
void check_io()
{
	//checks in
	if(in_!=0){
			if(access(in,R_OK)==-1) //checks to see if file is readable
			{
				printf("not read okay");
			}
			if(freopen(in,"r",stdin)==NULL){
				fprintf(stdout,"Failure to open");
			}
			
		}
		//checks out 
		if(out_!=0){
			if(access(in,W_OK)==-1) //checks to see if file is writeable *** may be appendable
			{
				printf("not write okay");
			}
			if(freopen(out,"w+",stdout)==NULL){ //checks to see if there was failure to open.
				fprintf(stdout,"Failure to open");
			}
			
		}
		//appends out 
		if(app_!=0){
			if(freopen(out,"a+",stdout)==NULL){		//checks for outfile.
				fprintf(stdout,"Failure to open");
			}
		
		}	
}

/************************
* Other command function handler
*
************************/
void other(char** command)
{
	
	pid_t pid;
	int status;
	
	//First Instance of the Switch case used for creating a forked process.
	//Giving it PID as a switch case, it can determine if the child process or the parent process
	//simply by checking for zero.
	                	
	switch(pid=fork()){
	case -1:
		//syserr("fork");
	case 0:
		//syserr("exec");
		//checks IO redirect
			check_io();
		//execute to "unknown" command
		if(execvp(command[0],command)==-1)
			{
				fprintf(stdout,"Command not found\n");
				_exit(-1);
			}
	default:
		if(!dont_wait)
			{
		//fprintf(stdout,"%d",pid);
		waitpid(pid,&status,WUNTRACED);}
	}
}
/************************
*	Arguement to deal with echo command
*
************************/

void echoarg(char** echoarg,int echocount)
{
	int i;
	int j;
	//checks IO redirect
	check_io();
	for(j=1;j<echocount;j++)
	{
		for(i=0;i<strlen(echoarg[j]);i++)
		{
			if(echoarg[j][i]!='"')
			{			
				fprintf(stdout,"%c",echoarg[j][i]);
			}
			
		}
		fprintf(stdout," ");
	}
	fprintf(stdout,"\n"); //clears line to throw
	
}
/************************
*	Environment function
*	Purpose: Displays all environment data that is pointed to by 
* external environment pointer.
************************/
void envi(){
	//checks IO redirect
	check_io();
	//prints out file
	fprintf(stdout,"%s",getenv("PWD"));
	int f=0;
	while(*environ!=NULL)
	{
		fprintf(stdout,"%s\n",*environ);
		environ++;
		f++;
	}
	for(;f>0;f--)
	{
		environ--;
	}
}
/************************
* Pause function implementation
* 
************************/
void sleeptime()
{
	getpass("Press Enter to continue..."); //uses getpass to wait for a string (without passing \n) for enter.
	
}
/************************
*
************************/
void clr(char** empty)
{
  //fprintf(stdout,"HELLO!\n");
	
	//empty character array
	pid_t pid;
	int status;
	strcpy(empty[0],"clear");
	//printf("%s",empty[0]);
	switch(pid=fork()){
	case -1:
		//syserr("fork");
	case 0:
		//syserr("exec");
		
	  //fprintf(stdout,"arrives at exec: %s\n", empty[0]);
		//checks IO redirect
			check_io();
		if(execvp(empty[0],empty)==-1)	//executes file and tests for failure
			{
				fprintf(stdout,"Command not found\n");
				_exit(-1);
			}
	default:
		if(!dont_wait)
			{
		//fprintf(stdout,"%d",pid);
		waitpid(pid,&status,WUNTRACED);}
	}
}

/************************
*change directory command
************************/
void cd(char** directory,int dircount){
	char sh[200];
	if(chdir(directory[1])!=0)
	{
			//fprintf(stdout,"%d",errno);
			fprintf(stdout,"%s\n",getenv("PWD"));	
			if(errno==2){
				fprintf(stdout,"%s\n",strerror(errno));
			}
	}
	else
	{
		
	}
	if(getcwd(sh,200)!=NULL){
			setenv("PWD",sh,1);
	}		
}

/************************
*Directory function implementation
************************/
void dir(char** directory,int dircount)
{	
	
	int status;


	//add ls -al to our arguments
	//add only one and overwrite dir
	directory[dircount+1]=(char*) malloc(sizeof(directory[dircount])); //malloc a new array location to 
	//fprintf(stdout,"%d",dircount);																	//shift all array data to the right 1
	strcpy(directory[0],"-al");
	while(dircount>=0)
	{
		
		directory[dircount+1]=directory[dircount];
		dircount--;
	}
	directory[0]="ls";

	
	//fprintf(stdout,"%s",directory[0]);
	pid_t pid;
	switch(pid=fork()){
	case -1:
		//syserr("fork");
	case 0:
		//fprintf(stdout,"Executing: %s",directory[0]);
		//checks IO redirect
			check_io();
		if(execvp(directory[0],directory)==-1) //executes file and tests for failure
			{
				fprintf(stdout,"Command not found\n");
				_exit(-1);
			}
		//syserr("exec");

	default:
		if(!dont_wait)
			{
		//fprintf(stdout,"%d",pid);
		waitpid(pid,&status,WUNTRACED);}
	}
	

}

/************************
*Help function
*Utilizes the more function to 
************************/

void help()
{
	fprintf(stdout,"\n");
	pid_t pid;
	int status;
	char *fstr= (char*) malloc(sizeof(char)*40);
	strcpy(fstr,getenv("SHELL"));
	strcat(fstr,"/help.txt");
	//printf("%s",fstr);
	char *help[]={"more",fstr,(char *) 0};
	switch(pid=fork()){				
	case -1:
		//syserr("fork");
	case 0:
		//syserr("exec");
		//checks IO redirect
			check_io();
		if(execvp(help[0],help)==-1)
			{
				fprintf(stdout,"Command not found\n");
				_exit(-1);
			}
		//syserr("exec");
	default:
		if(!dont_wait)
			{
		//fprintf(stdout,"%d",pid);
		waitpid(pid,&status,WUNTRACED);}
	}
	
}

/************************
*
************************/
int main(int argc, char** argv)
{
	char sh[200];
	strcpy(sh,"SHELL=");
	strcat(sh,getenv("PWD"));
	putenv(sh);

	if(argc==2)
	{
		//printf("file readin");
		if(freopen(argv[1],"r",stdin)==NULL){
			fprintf(stdout,"Failure to open");
		}
	}
	
	//char* comm;
	char str[200];
	char* commands[NUMBER_OF_COMMANDS]={"clr","dir","environ","quit","echo","cd","pause","help"};

	while(1)
	{
		//initializing redirection values
		in_=0;
		out_=0;
		app_=0;
		
		dont_wait=0; //sets next arguement to auto wait.
		fprintf(stdout,"%s==>",getenv("PWD"));
		fflush(stdout);
		//start reading function
		//get string and note that it has a newline character at the end of the string.
		
		if(fgets(str,200,stdin)==NULL)
		{
			fclose(stdin);
		}
		//end reading function
		strcpy(masterstr,str);
		
		//malloc buffer size
		int argcount=0;
		
		char* buffer =NULL;
		buffer=strtok(str," \n\t");
		char** argarray=(char**) malloc(sizeof(str));
		
		
		while(buffer!=NULL)
		{
			if(strcmp(buffer,"&")==0)
			{								
				fprintf(stdout,"%s",buffer);																														
				dont_wait=1;	
					
			}
			else if(strcmp(buffer,"<")==0)
			{
				fprintf(stdout,"%s",buffer);
				fprintf(stdin,"Input changed");	
				buffer=strtok(NULL," \n\t");
				if(buffer!=NULL)
				{
					strcpy(in,buffer);					
					in_=1;
				}
				
				
			}
			else if(strcmp(buffer,">")==0)
			{
				fprintf(stdout,"%s",buffer);
				fprintf(stdin,"Output changed");
				buffer=strtok(NULL," \n\t");
				if(buffer!=NULL)
				{
					strcpy(out,buffer);
					out_=1;
				}
			}
			else if(strcmp(buffer,">>")==0)
			{
				fprintf(stdout,"%s",buffer);
				fprintf(stdin,"append Out changed");
				buffer=strtok(NULL," \n\t");
				if(buffer!=NULL)
				{
					strcpy(app,buffer);
					app_=1;
				}
			}
			else
			{	
				argarray[argcount]=(char*) malloc(sizeof(buffer)); //allocate space for each argument 
				strcpy(argarray[argcount++],buffer);
				//take next value from command string
						
			}
			buffer=strtok(NULL," \n\t");
		}

		//start command parse
		int l;

		if(argarray[0]!=NULL)
		{
			for(l=0;l<NUMBER_OF_COMMANDS;l++)
			{
				//remove to test string comparison
				//fprintf(stdout,"%s:%s\n",arguments,commands[l]);
				if(strcmp(argarray[0],commands[l])==0)
				{
					break;
				}
		
			}
																																				
				//argarray[argcount++]=(char*) malloc (sizeof(str));											
				//argarray[argcount++]='\0';																									
				//remove to test the value of l																									
				//fprintf(stdout,"HELLO? %d",l);																									
																																									
				switch(l)																																	
				{																																					
					case(0):clr(argarray);break;																					
					case(1):dir(argarray,argcount);break;																		
					case(2):envi();break;
					case(3):return 0;break;
					case(4):echoarg(argarray,argcount);break;
					case(5):cd(argarray,argcount);break;
					case(6):sleeptime();break;
					case(7):help();break;
					default:other(argarray);break;
				}
			}
		
	}
}
