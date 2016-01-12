/* Copyright (C) 2015 Schrodi */
/********************************************************
 * helpfile -- functions to display help commands,      *
 *      version information, and other similar          *
 *      command line messages and helpful functions.    *
 ********************************************************/

/* Copyright (C) 1991-2014 Free Software Foundation, Inc. */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <termios.h>

/* libconfig - A library for processing structured configuration files
   Copyright (C) 2005-2010  Mark A Lindner */
#include <libconfig.h>

/* Copyright (c) 2009, 2013, Oracle and/or its affiliates. All rights reserved. */
#include <my_config.h>

/* Local Includes */
#include "helpfile.h"

/* Flag Definitions */
int flags;
const int EXIT_FLAG = (1<<0);
const int PORT_SET = (1<<1);
const int HOST_SET = (1<<2);
const int USER_SET = (1<<3);
const int PASS_SET = (1<<4);
const int DATABASE_SET = (1<<5);

/* Global Variables */
//MySQL
//Create config structure and variables to store config settings.
config_t conf;
const char* host;
const char* user;
const char* database;
const char pass[SIZE]; //FIX TO USE SIZE DEFINE!!!!
int port;

/* Function Prototypes */
void set_password(char* pass);
void check_arguments(int argc, char** argv);
void print_help(int argc, char** argv);
void print_version(void);
void error_log(void);
char *timestamp(void);

//function to set password from command line argument.
void set_password(char* pass){
	static struct termios oldt, newt;
	    int i = 0;
	    int c;

	    /*saving the old settings of STDIN_FILENO and copy settings for resetting*/
	    tcgetattr( STDIN_FILENO, &oldt);
	    newt = oldt;

	    /*setting the approriate bit in the termios struct*/
	    newt.c_lflag &= ~(ECHO);

	    /*setting the new bits*/
	    tcsetattr( STDIN_FILENO, TCSANOW, &newt);

	    /*reading the password from the console*/
	    while ((c = getchar())!= '\n' && c != EOF && i < SIZE){
	        pass[i] = c;
			i += 1;
	    }
		pass[i] = '\0';
		//Check to make sure password is entered.
		if(pass[0]=='\0'){
			fprintf(stderr, "%s, Password Error: No password entered.\n", timestamp());
			exit(1);
		}

	    /*resetting our old STDIN_FILENO*/
	    tcsetattr( STDIN_FILENO, TCSANOW, &oldt);
}//set_password

// Function to set variables based on command line arguments and config file.
void check_arguments(int argc, char** argv){
    flags = 0; //Clear variable for flags.
    int i;
    if(argc == 1){} //No arguments do nothing.
    //This is where the function really begins.
    else{
        //Begin checking arguments
        for(i = 1; i < argc; i += 1){
            if( (strncmp("-h", argv[i], 2) == 0) || (strncmp("--help", argv[i], 6) == 0) || (strncmp("?", argv[i], 1) == 0) ){
                print_help(argc, argv);
                flags |= EXIT_FLAG; //flag set to EXIT_SUCCESS.
                continue;
            }//-h, --help
            else
            if( (strncmp("-v", argv[i], 2) == 0) || (strncmp("--version", argv[i], 9) == 0) ){
                print_version();
                flags |= EXIT_FLAG; //flag set to EXIT_SUCCESS.
                continue;
            }//--version
            else
            if( (strncmp("--port", argv[i], 6) == 0) ){
                port = atoi(argv[i+1]);
                flags |= PORT_SET;
                continue;
            }//--port
            else
            if( (strncmp("--host", argv[i], 6) == 0) ){
                host = argv[i+1];
                flags |= HOST_SET;
                continue;
            }//--host
            else
            if( (strncmp("--user", argv[i], 6) == 0) ){
                user = argv[i+1];
                flags |= USER_SET;
                continue;
            }//--user
            if( (strncmp("--database", argv[i], 10) == 0) ){
                database = argv[i+1];
                flags |= DATABASE_SET;
                continue;
            }//--database
            else
            if( (strncmp("--password", argv[i], 10) == 0) ){
                printf("Enter password: ");
                set_password((char*)pass);
                flags |= PASS_SET;
                printf("\n\n"); //to put a return after entering Password:
                continue;
            }//--password

            else{ continue; } //Unknown argument or variable. Do nothing and continue.

        }//end for loop checking arguments.
    }//end function if/else.
}//check_arguments

//function to set variables from config file.
void set_variables(void){
    //variable for setting MySQL Password.
    const char* tmp_pass;

	//initialize config structure.
	config_init(&conf);

	//Read configuration file.
	if(!config_read_file(&conf, "config")){
		fprintf(stderr, "%s, %s:%d - %s\n", timestamp(), config_error_file(&conf), config_error_line(&conf),
				config_error_text(&conf));
		//Destroy config structure and exit.
		config_destroy(&conf);
		exit(1);
	}

/*****************************
 * Setup Variables for MySQL *
 *****************************/

	if((flags & PORT_SET) != 0){} //Port set by argument. Do nothing.
	else{ //Set port from config file.
		if(config_lookup_int(&conf, "port", &port )){}
		else{
			fprintf(stderr, "%s, Config Error: Port not understood.\n", timestamp());
			exit(1);
		}
	}

	if((flags & HOST_SET) != 0){} //Host set by argument. Do nothing.
	else{ //Set host from config file.
		if(config_lookup_string(&conf, "host", &host)){}
		else{
			fprintf(stderr, "%s, Config Error: Host not understood.\n", timestamp());
			exit(1);
		}
	}

	if((flags & USER_SET) != 0){} //User set by argument. Do nothing.
	else{ //Set user from config file.
		if(config_lookup_string(&conf, "user", &user)){}
		else{
			fprintf(stderr, "%s, Config Error: User not understood.\n", timestamp());
			exit(1);
		}
	}

	if((flags & DATABASE_SET) != 0){} //Database set by argument. Do nothing.
	else{ //Set database from config file.
		if(config_lookup_string(&conf, "database", &database)){}
		else{
			fprintf(stderr, "%s, Config Error: Database not understood.\n", timestamp());
			exit(1);
		}
	}

	if((flags & PASS_SET) != 0){} //Password set by function call. Do nothing.
	else{ //Set password from config file.
		if(config_lookup_string(&conf, "password", &tmp_pass)){
			memcpy((char*)pass,tmp_pass,strlen(tmp_pass)); //copy password from tmp storage then free tmp.
			free((void*)tmp_pass); //cast to void* to suppress warning.
		}
		else{ //Password not set by config or argument.
			printf("Password: ");
			set_password((char*)pass); //Get password from user.
			printf("\n\n");
		}
	}

/**************************
 * End of Variables Setup *
 **************************/

	printf("%s, Variables Created.\n", timestamp());
}//set_variables

//function to destroy config structure.
void close_config(void){
    config_destroy(&conf);
	printf("%s, Variables Destroyed\n", timestamp());
}//close_config

//function to print help to stderr.
void print_help(int argc, char** argv){
    if(argc >= 1){} //Do nothing. for compiler warning, argc unsued right now.
    printf("\n\tLibreCoin\n\tProper Usage:\n");
    printf("\t\t%s [OPTIONS]\n\n", argv[0]);

    printf("\n\t\t [OPTIONS]\n\n");

    printf("\t\t -h, ?, --help\t\t\tDisplay this help file and exit.\n");
    printf("\t\t -v, --version\t\t\tDisplay the program version number and exit.\n\n");

    printf("\t\t --port <port number>\t\tSet Port for MySQL Server.\t(Default: 3306)\n");
    printf("\t\t --host <host address>\t\tSet Host for MySQL Server.\t(Default: localhost)\n");
    printf("\t\t --user <user name>\t\tSet User for MySQL Server.\n");
    printf("\t\t --database <database name>\tSet Database for MySQL Server.\t(Default: librecoin)\n");

    printf("\n\t\t --password\t\t\tYou will be prompted to enter your MySQL Password.\n");
    printf("\n\n");
}//print_help

//function to print version to stderr.
void print_version(void){
  printf("\tVersion: %s for %s (%s)\n\n", LC_VERSION, SYSTEM_TYPE, MACHINE_TYPE);
}//print_version

//function to create stderr file.
void error_log(void){
    int fd = open("../error.log", O_RDWR | O_APPEND | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH); //create an error log.
    if (fd < 0) {
         printf("%s, Cannot open error.log\n", timestamp());
         exit(1);
    }
    if (dup2(fd, STDERR_FILENO) < 0) {
         printf("%s, Cannot redirect stderr.\n", timestamp());
         exit(1);
    }
}//error_log

//function to get the current timestamp.
char *timestamp(void){
    time_t rawtime;
    struct tm * timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);
    char *timestamp = asctime(timeinfo);
    timestamp[strlen(timestamp)-1] = '\0'; //remove '\n' from asctime.

    return timestamp;
}//timestamp
