/* Copyright (C) 2015 Schrodi */

/* Copyright (C) 1991-2014 Free Software Foundation, Inc. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <termios.h>

/* libconfig - A library for processing structured configuration files
   Copyright (C) 2005-2010  Mark A Lindner */
#include <libconfig.h>

/* Local Includes */
#include "main.h"
#include "helpfile.h"
#include "sqlfunc.h"

/* Flag Definitions */
int flags;
const int EXIT_FLAG = (1<<0);
const int PORT_SET = (1<<1);
const int HOST_SET = (1<<2);
const int USER_SET = (1<<3);
const int PASS_SET = (1<<4);
const int DATABASE_SET = (1<<5);

/* Global Variables */
const char* host;
const char* user;
const char* database;
const char pass[SIZE]; //FIX TO USE SIZE DEFINE!!!!
int port;

/* Function Prototypes */
void set_password(char* pass);

/* Begin Main LibreCoin Function */
int main (int argc,char** argv){
	//create log file for stderr.
	error_log();

	//variables for setting MySQL Password.
	const char* tmp_pass;

	//Give me a little space...
	printf("\n");

	//Check Argumens. Work in progress. (helpfile)
	check_arguments(argc, argv);
	if((flags & EXIT_FLAG) != 0){ return(EXIT_SUCCESS); } // Exit on specific arguments.

	//Create config structure and variables to store config settings.
	config_t conf;

	//initialize config structure.
	config_init(&conf);

	//Read configuration file.
	if(!config_read_file(&conf, "config")){
		fprintf(stderr, "%s -\t%s:%d - %s\n", timestamp(), config_error_file(&conf), config_error_line(&conf),
				config_error_text(&conf));
		//Destroy config structure and exit.
		config_destroy(&conf);
		return(EXIT_FAILURE);
	}

/*****************************
 * Setup Variables for MySQL *
 *****************************/

	if((flags & PORT_SET) != 0){} //Port set by argument. Do nothing.
	else{ //Set port from config file.
		if(config_lookup_int(&conf, "port", &port )){}
		else{
			fprintf(stderr, "%s -\tNumber Error\n or port already set.", timestamp());
			return(EXIT_FAILURE);
		}
	}

	if((flags & HOST_SET) != 0){} //Host set by argument. Do nothing.
	else{ //Set host from config file.
		if(config_lookup_string(&conf, "host", &host)){}
		else{
			fprintf(stderr, "%s -\tString Error\n or host already set.", timestamp());
			return(EXIT_FAILURE);
		}
	}

	if((flags & USER_SET) != 0){} //User set by argument. Do nothing.
	else{ //Set user from config file.
		if(config_lookup_string(&conf, "user", &user)){}
		else{
			fprintf(stderr, "%s -\tString Error\n or user already set.", timestamp());
			return(EXIT_FAILURE);
		}
	}

	if((flags & DATABASE_SET) != 0){} //Database set by argument. Do nothing.
	else{ //Set database from config file.
		if(config_lookup_string(&conf, "database", &database)){}
		else{
			fprintf(stderr, "%s -\tString Error\n or database already set.", timestamp());
			return(EXIT_FAILURE);
		}
	}

	if((flags & PASS_SET) != 0){} //Password set by function call. Do nothing.
	else{ //Set password from config file.
		if(config_lookup_string(&conf, "pass", &tmp_pass)){
			memcpy((char*)pass,tmp_pass,strlen(tmp_pass)); //copy password from tmp storage then free tmp.
			free((void*)tmp_pass); //cast to void* to suppress warning.
		}
		else{
			fprintf(stderr, "%s -\tString Error\n or user already set.", timestamp());
			return(EXIT_FAILURE);
		}
	}

/**************************
 * End of Variables Setup *
 **************************/

 	printf("\tVariables Set\n\n");

 /********************
  * Connect to MySQL *
  ********************/

	open_database();
	printf("\t\tConnected to %s on %s:%d\n",database,host,port);

	//DO STUFF!


	close_database();
	printf("\t\tConnection to %s on %s:%d closed\n", database,host,port);
	config_destroy(&conf);
	printf("\n\tVariables Destroyed\n");
	EOL;
	return(EXIT_SUCCESS);
}//main

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

	    /*resetting our old STDIN_FILENO*/
	    tcsetattr( STDIN_FILENO, TCSANOW, &oldt);
}//set_password
