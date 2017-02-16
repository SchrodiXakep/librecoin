/* Copyright (C) 2015 Schrodi */

#ifndef __HELPFILE__
#define __HELPFILE__

//Version: (Release).(Year with preceeding 0 because the future).(Month).(Day)(Alpha/Beta/ReleaseCandidate)
#define LC_VERSION "0.02016.1.11a" //updates randomly.

/* Global Flags */
extern int flags;

/* Flag Definitions */
extern const int EXIT_FLAG;
extern const int PORT_SET;
extern const int HOST_SET;
extern const int USER_SET;
extern const int PASS_SET;
extern const int DATABASE_SET;

/* Global Variables */
#define SIZE 1024 //limit for password length.

extern const char* host;
extern const char* user;
extern const char* database;
extern const char pass[SIZE];
extern int port;

/* Function Prototypes */
void set_variables(void);
void close_config(void);
void set_password(char* pass);
void check_arguments(int argc, char** argv);
void print_help(int argc, char** argv);
void print_version(void);
void error_log(void);
char *timestamp(void);

#endif
