/* Copyright (C) 2015 Schrodi */

/* Copyright (C) 1991-2014 Free Software Foundation, Inc. */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* libconfig - A library for processing structured configuration files
   Copyright (C) 2005-2010  Mark A Lindner */
#include <libconfig.h>

/* Local Includes */
#include "main.h"
#include "helpfile.h"
#include "sqlfunc.h"

/* Begin Main LibreCoin Function */
int main (int argc,char** argv){
	//create log file for stderr.
	error_log();

	//Give me a little space...
	printf("\n");

	//Check Argumens. Work in progress. (helpfile)
	check_arguments(argc, argv);
	if((flags & EXIT_FLAG) != 0){ return(EXIT_SUCCESS); } // Exit on specific arguments.

	//set variables from config file. (helpfile)
	set_variables();
 	printf("%s - Variables Set\n", timestamp());

 /********************
  * Connect to MySQL *
  ********************/

	//(sqlfunc)
	open_database();
	printf("%s - Connected to %s on %s:%d\n", timestamp(),database,host,port);

	//DO STUFF!

	//(sqlfunc)
	close_database();
	printf("%s - Connection to %s on %s:%d closed\n", timestamp(), database,host,port);

	//destroy config structure. (helpfile)
	close_config();
	printf("%s - Variables Destroyed\n", timestamp());
	EOL;
	return(EXIT_SUCCESS);
}//main
