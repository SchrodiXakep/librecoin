/* Copyright (C) 2015 Schrodi
We have gone to a lot of trouble to ask questions to clarify the scope of the problem and the data necessary
to support that. The decisions to which we have come are hypothetical. They are not right or wrong. Even for
a real problem there will not be right or wrong answers; we can only ever hope for a good pragmatic solution.
 */

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
	//create log file for stderr. (helpfile)
	error_log();

	//Give me a little space...
	printf("\n");

	//Check Argumens. Work in progress. (helpfile)
	check_arguments(argc, argv);
	if((flags & EXIT_FLAG) != 0){ return(EXIT_SUCCESS); } // Exit on specific arguments.

	//set variables from config file. (helpfile)
	set_variables();

 /********************
  * Connect to MySQL *
  ********************/

	//(sqlfunc)
	open_database();

	//DO STUFF!

	//(sqlfunc)
	close_database();


	//destroy config structure. (helpfile)
	close_config();
	EOL;
	return(EXIT_SUCCESS);
}//main
