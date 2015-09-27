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
void error_log(void);
char *timestamp(void);

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
                printf("Password: ");
                set_password((char*)pass);
                flags |= PASS_SET;
                printf("\n\n"); //to put a return after entering Password:
                continue;
            }//--password

            else{
                continue;  //Unknown argument or variable. Do nothing.
            }
        }//end for loop checking arguments.
    }//end function if/else.
}//check_arguments

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
    int fd = open("../error.log", O_RDWR | O_APPEND | O_CREAT, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH); //create read only error log. (user can write.)
    if (fd < 0) {
         printf("%s -\tCannot open error.log\n", timestamp());
         exit(1);
    }
    if (dup2(fd, STDERR_FILENO) < 0) {
         printf("%s -\tCannot redirect stderr.\n", timestamp());
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
