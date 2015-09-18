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
#include "main.h"       //Global Variables.

/* Function Prototypes */
void connect_to_database(void);

// /* Global Variables */
static char *socket_name = NULL;    /* socket name (use built-in value) */
static unsigned int my_flags = 0;      /* connection flags (none) */
static MYSQL *conn;                     /* pointer to connection handler */

/* connect to server */
void open_database(void){
    /* initialize connection handler */
    conn = mysql_init (NULL);

    if (conn == NULL){
        fprintf (stderr, "mysql_init() failed (probably out of memory)\n");
        exit (1);
    }

    /* connect to server */
    if (mysql_real_connect (conn, host, user, pass, database, port, socket_name, my_flags) == NULL){
        fprintf (stderr, "mysql_real_connect() failed\n");
        mysql_close (conn);
        exit (1);
    }
}

/* disconnect from server */
void close_database(void){
    mysql_close (conn);
}
