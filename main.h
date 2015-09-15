// Define once.
#ifndef __MAIN__
#define __MAIN__

//Version: (Release).(Year with preceeding 0 because the future).(Month).(Day)(Alpha/Beta/ReleaseCandidate)
#define LC_VERSION "0.02015.9.14a"

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
extern const char* host;
extern const char* user;
extern const char* database;
extern char pass[256];
extern int port;

/* Function Prototypes */
void set_password(void);

// End __MAIN__ ifndef
#define EOL printf("\nEND OF LINE.\n")
#endif
