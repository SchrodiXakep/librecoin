/* Copyright (C) 2015 Schrodi */
/********************************************************
 * sqlfunc -- functions to connect to MySQL server,     *
 ********************************************************/

/* Copyright (C) 1991-2014 Free Software Foundation, Inc. */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>

/* Copyright (c) 2009, 2013, Oracle and/or its affiliates. All rights reserved. */
#include <my_global.h>
#include <my_sys.h>
#include <mysql.h>
#include <my_config.h>

/* Local Includes */
#include "helpfile.h"

/* Function Prototypes */
void my_error_exit(char* err);
void open_database(void);
void close_database(void);
void check_database_structure(void);

// /* Global Variables */
static char *socket_name = NULL;    /* socket name (use built-in value) */
static unsigned int my_flags = 0;      /* connection flags (none) */
static MYSQL *conn;                     /* pointer to connection handler */

/* close connection and exit */
void my_error_exit(char* err){
    fprintf (stderr, "%s, %s - %u (%s): %s\n", timestamp(), err,\
             mysql_errno (conn), mysql_sqlstate (conn), mysql_error (conn));
    close_database();
    exit(2);
}

/* check for the correct database structure */
void check_database_structure(void){
    printf("%s, Checking Database ;)\n", timestamp());
}

/* connect to server */
void open_database(void){
    /* initialize client library */
    if (mysql_library_init (0, NULL, NULL)){
        my_error_exit("mysql_library_init() failed\n");
    }
    /* initialize connection handler */
    conn = mysql_init (NULL);

    if (conn == NULL){
        fprintf(stderr, "%s, mysql_init() failed (probably out of memory)", timestamp());
        exit(2);
    }

    /* connect to server */
    if (mysql_real_connect (conn, host, user, pass, database, port, socket_name, my_flags) == NULL){
        my_error_exit("mysql_real_connect() failed");
    }
    printf("%s, Connected to %s on %s@%s:%d\n", timestamp(),database,user,host,port);
}//open_database

/* disconnect from server */
void close_database(void){
    mysql_close (conn);
    mysql_library_end();
    printf("%s, Connection to %s on %s@%s:%d closed\n\t\t\t  MySQL Client Library closed\n", timestamp(), database,user,host,port);
}//close_database
