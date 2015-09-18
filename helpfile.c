/* Copyright (C) 2015 Schrodi */
/********************************************************
 * helpfile -- functions to display help commands,      *
 *      version information, and other similar          *
 *      command line messages.                          *
 ********************************************************/

/* Copyright (C) 1991-2014 Free Software Foundation, Inc. */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>


/* libconfig - A library for processing structured configuration files
   Copyright (C) 2005-2010  Mark A Lindner */
#include <libconfig.h>

/* Copyright (c) 2009, 2013, Oracle and/or its affiliates. All rights reserved. */
#include <my_config.h>

/* Local Includes */
#include "main.h"       //Global Variables.

/* Function Prototypes */
void check_arguments(int argc, char** argv);
void print_help(int argc, char** argv);
void print_version(void);

// Function to set variables based on command line arguments and config file.
void check_arguments(int argc, char** argv){
    flags = 0; //Clear variable for flags.
    int i;
    if(argc == 1){} //No arguments do nothing.
    //This is where the function really begins.
    else{
        //Begin checking arguments
        for(i = 0; i < argc; i += 1){
            if( (strncmp("-h", argv[i], 2) == 0) || (strncmp("--help", argv[i], 6) == 0) || (strncmp("?", argv[i], 1) == 0) ){
                print_help(argc, argv);
                flags |= EXIT_FLAG; //flag set to EXIT_SUCCESS.
            }//-h, --help
            else
            if( (strncmp("--version", argv[i], 9) == 0) ){
                print_version();
                flags |= EXIT_FLAG; //flag set to EXIT_SUCCESS.
            }//--version
            else
            if( (strncmp("--port", argv[i], 6) == 0) ){
                port = atoi(argv[i+1]);
                flags |= PORT_SET;
            }//--port
            else
            if( (strncmp("--host", argv[i], 6) == 0) ){
                host = argv[i+1];
                flags |= HOST_SET;
            }//--host
            else
            if( (strncmp("--user", argv[i], 6) == 0) ){
                user = argv[i+1];
                flags |= USER_SET;
            }//--user
            else
            if( (strncmp("--password", argv[i], 10) == 0) ){
                printf("Password: ");
                set_password((char*)pass);
                flags |= PASS_SET;
                printf("\n\n"); //to put a return after entering Password:
            }//--password

            else{} //Do nothing.
        }//end for loop checking arguments.
    }//end function if/else.
}//check_arguments

//function to print help to stderr.
void print_help(int argc, char** argv){
    if(argc >= 1){} //Do nothing. for compiler warning, argc unsued right now.
    fprintf(stderr, "\n\tLibreCoin\n\tProper Usage:\n");
    fprintf(stderr, "\t\t%s [OPTIONS]\n\n", argv[0]);

    fprintf(stderr, "\n\t\t [OPTIONS]\n\n");

    fprintf(stderr, "\t\t -h, ?, --help\t\t\tDisplay this help file and exit.\n");
    fprintf(stderr, "\t\t --version\t\t\tDisplay the program version number and exit.\n\n");

    fprintf(stderr, "\t\t --port <port number>\t\tSet Port for MySQL Server.\n");
    fprintf(stderr, "\t\t --host <host address>\t\tSet Host for MySQL Server.\n");
    fprintf(stderr, "\t\t --user <user name>\t\tSet User for MySQL Server.\n");

    fprintf(stderr, "\n\t\t --password\t\t\tYou will be prompted to enter your MySQL Password.\n");
    fprintf(stderr, "\n\n");
}//print_help

//function to print version to stderr.
void print_version(void){
  fprintf(stderr, "\tVersion: %s for %s (%s)\n\n", LC_VERSION, SYSTEM_TYPE, MACHINE_TYPE);
}//print_version
