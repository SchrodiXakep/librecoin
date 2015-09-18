/* Copyright (C) 2015 Schrodi */

#ifndef __MAIN__
#define __MAIN__

//Version: (Release).(Year with preceeding 0 because the future).(Month).(Day)(Alpha/Beta/ReleaseCandidate)
#define LC_VERSION "0.02015.9.14a" //update later when significant progress has been made.

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
#define SIZE 256 //limit for password length.
extern const char* host;
extern const char* user;
extern const char* database;
extern const char pass[SIZE];
extern int port;

/* Function Prototypes */
void set_password(char* pass);

// End __MAIN__ ifndef
#define EOL printf("\nEND OF LINE.\n")
#endif
