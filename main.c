/* Copyright (C) 2015 Schrodi */

/* Copyright (C) 1991-2014 Free Software Foundation, Inc. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* libconfig - A library for processing structured configuration files
   Copyright (C) 2005-2010  Mark A Lindner */
#include <libconfig.h>

/* Local Includes */
#include "main.h"
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
const char* host;
const char* user;
const char* database;
char pass[256];
int port;

/* Function Prototypes */
void set_password(void);

/* Begin Main LibreCoin Function */
int main (int argc,char** argv){
	//variables for setting MySQL Password.
	const char* tmp_pass;



	//Give me a little space...
	printf("\n");

	//Check Argumens. Work in progress. (helpfile)
	check_arguments(argc, argv);

	if((flags & EXIT_FLAG) != 0){ return(EXIT_SUCCESS); } //Help called, exit.

	//Create config structure and variables to store config settings.
	config_t conf;
	//config_setting_t *setting; //-Wall says unused variable?

	// const char* tmpstr;
	// int tmpint;

	//initialize config structure.
	config_init(&conf);

	//Read configuration file.
	if(!config_read_file(&conf, "config")){
		fprintf(stderr, "%s:%d - %s\n", config_error_file(&conf), config_error_line(&conf),
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
		if(config_lookup_int(&conf, "port", &port)){}
		else{
			fprintf(stderr, "Number Error\n or port already set.");
			return(EXIT_FAILURE);
		}
	}

	if((flags & HOST_SET) != 0){} //Host set by argument. Do nothing.
	else{ //Set host from config file.
		if(config_lookup_string(&conf, "host", &host)){}
		else{
			fprintf(stderr, "String Error\n or host already set.");
			return(EXIT_FAILURE);
		}
	}

	if((flags & USER_SET) != 0){} //User set by argument. Do nothing.
	else{ //Set user from config file.
		if(config_lookup_string(&conf, "user", &user)){}
		else{
			fprintf(stderr, "String Error\n or user already set.");
			return(EXIT_FAILURE);
		}
	}

	if((flags & PASS_SET) != 0){} //Password set by function call. Do nothing.
	else{ //Set password from config file.
		if(config_lookup_string(&conf, "pass", &tmp_pass)){ memcpy(pass,tmp_pass,strlen(tmp_pass)); }
		else{
			fprintf(stderr, "String Error\n or user already set.");
			return(EXIT_FAILURE);
		}
	}

	/*!!! CHECKING VARIABLES !!!*/
	fprintf(stderr, "port: %d\n", port);
	fprintf(stderr, "host: %s\n", host);
	fprintf(stderr, "user: %s\n", user);
	fprintf(stderr, "pass: %s\n", pass);



	//Destroy config structure.
	config_destroy(&conf);
	EOL;
	return(EXIT_SUCCESS); //END OF LINE.
}//main

//function to set password from command line.
void set_password(void){
    fprintf(stdout, "Password: ");
    fgets(pass, 255, stdin);
	pass[(strlen(pass)-1)] = '\0';
}//set_password
