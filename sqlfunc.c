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
void process_result_set (MYSQL *conn, MYSQL_RES *res_set);

// /* Global Variables */
#define DATABASE_SIZE 2     //Current number of tables in database.
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
    //MySQL resource pointer
    MYSQL_RES *res_set;

    //Show tables MySQL command.
    char show_db_tables[68] = "SHOW TABLES FROM ";
    strncat(show_db_tables, database, 50);

    printf("%s, Checking Database ;)\n", timestamp());

    if (mysql_query (conn, show_db_tables) != 0)
        my_error_exit ("mysql_query() failed");

    else{
        res_set = mysql_store_result (conn); /* generate result set */

        if (res_set == NULL){ my_error_exit ("mysql_store_result() failed."); }

        else if ((unsigned long) mysql_num_rows(res_set) == 0){
            printf("%s, No tables found. Creating them.\n", timestamp());

            /* Tables will be created resources deallocated */

            if (mysql_query(conn, "CREATE TABLE users (\
                            user_id INT UNSIGNED NOT NULL,\
                            PRIMARY KEY (user_id),\
                            user_name VARCHAR(15) NOT NULL,\
                            pseudonym VARCHAR(15) NULL,\
                            surname VARCHAR(15) NOT NULL,\
                            middle_name VARCHAR(15) NULL,\
                            given_name VARCHAR(15) NOT NULL,\
                            name_suffix VARCHAR(6) NULL,\
                            character_hash CHAR(40) NOT NULL\
                        );") != 0){ my_error_exit ("CREATE TABLE \'users\' failed."); }
            printf("%s, Table \'users\' created.\n", timestamp());
            //CREATE TABLE users

            if (mysql_query(conn, "CREATE TABLE user_server (\
                                  server_id INT UNSIGNED NOT NULL,\
                                  PRIMARY KEY (server_id),\
                                  user_id INT UNSIGNED NOT NULL,\
                                  FOREIGN KEY (user_id) REFERENCES users (user_id),\
                                  server_ip CHAR(16) NOT NULL,\
                                  server_port CHAR(6) NOT NULL,\
                                  server_onion_service CHAR(40) NULL,\
                                  server_key CHAR(40) NOT NULL\
                                  );") != 0){ my_error_exit ("CREATE TABLE \'user_server\' failed."); }
            printf("%s, Table \'user_server\' created.\n", timestamp());
            //CREATE TABLE user_server

            if (mysql_query(conn, "CREATE TABLE phone_number (\
                            phone_id INT UNSIGNED NOT NULL,\
                            PRIMARY KEY (phone_id),\
                            user_id INT UNSIGNED NOT NULL,\
                            FOREIGN KEY (user_id) REFERENCES users (user_id),\
                            area_code VARCHAR(5) NOT NULL,\
                            phone_number VARCHAR(15) NOT NULL,\
                            phone_number_from DATE NOT NULL,\
                            phone_number_to DATE NULL\
                        );") != 0){ my_error_exit ("CREATE TABLE \'phone_number\' failed."); }
            printf("%s, Table \'phone_number\' created.\n", timestamp());
            //CREATE TABLE phone_number

            if (mysql_query(conn, "CREATE TABLE email_address (\
                            email_id INT UNSIGNED NOT NULL,\
                            PRIMARY KEY (email_id),\
                            user_id INT UNSIGNED NOT NULL,\
                            FOREIGN KEY (user_id) REFERENCES users (user_id),\
                            email VARCHAR(15) NOT NULL,\
                            gpg_key_id VARCHAR(16) NULL,\
                            gpg_fingerprint VARCHAR(52) NULL,\
                            email_address_from DATE NOT NULL,\
                            email_address_to DATE NULL\
                        );") != 0){ my_error_exit ("CREATE TABLE \'email_address\' failed."); }
            printf("%s, Table \'email_address\' created.\n", timestamp());
            //CREATE TABLE email_address

            if (mysql_query(conn, "CREATE TABLE physical_address (\
                            address_id INT UNSIGNED NOT NULL,\
                            PRIMARY KEY (address_id),\
                            user_id INT UNSIGNED NOT NULL,\
                            FOREIGN KEY (user_id) REFERENCES users (user_id),\
                            line_1 VARCHAR(15) NOT NULL,\
                            line_2 VARCHAR(15) NULL,\
                            town_city VARCHAR(15) NOT NULL,\
                            zip_postcode VARCHAR(10) NOT NULL,\
                            state_province_county VARCHAR(20) NOT NULL,\
                            country VARCHAR(20) NOT NULL,\
                            other VARCHAR(10) NULL,\
                            address_from DATE NOT NULL,\
                            address_to DATE NULL\
                            );") != 0){ my_error_exit ("CREATE TABLE \'physical_address\' failed."); }
            printf("%s, Table \'physical_address\' created.\n", timestamp());
            //CREATE TABLE physical_address

            if (mysql_query(conn, "CREATE TABLE user_generated_credits (\
                            credit_id INT UNSIGNED NOT NULL,\
                            PRIMARY KEY (credit_id),\
                            user_id INT UNSIGNED NOT NULL,\
                            FOREIGN KEY (user_id) REFERENCES users (user_id),\
                            credits INT NOT NULL,\
                            created_on DATE NOT NULL,\
                            credit_sha1 CHAR(40) NOT NULL\
                        );") != 0){ my_error_exit ("CREATE TABLE \'user_generated_credits\' failed."); }
            printf("%s, Table \'user_generated_credits\' created.\n", timestamp());
            //CREATE TABLE user_generated_credits

            if (mysql_query(conn, "CREATE TABLE receved_credits (\
                            credits INT NOT NULL\
                        );") != 0){ my_error_exit ("CREATE TABLE \'receved_credits\' failed."); }
            printf("%s, Table \'receved_credits\' created.\n", timestamp());
            //CREATE TABLE receved_credits

            if (mysql_query(conn, "CREATE TABLE sent_credits (\
                            credits INT NOT NULL\
                        );") != 0){ my_error_exit ("CREATE TABLE \'sent_credits\' failed."); }
            printf("%s, Table \'sent_credits\' created.\n", timestamp());
            //CREATE TABLE sent_credits

            if (mysql_query(conn, "CREATE TABLE last_credits (\
                            credits INT NOT NULL\
                        );") != 0){ my_error_exit ("CREATE TABLE \'last_credits\' failed."); }
            printf("%s, Table \'last_credits\' created.\n", timestamp());
            //CREATE TABLE last_credits

            printf("%s, All tables created sucessfully.\n", timestamp());
            mysql_free_result (res_set);
        }//No tables in database.

        else if ((unsigned long) mysql_num_rows(res_set) > 0 && (unsigned long) mysql_num_rows(res_set) < DATABASE_SIZE){
            printf("%s, Tables missing. Fixing...\n", timestamp());
        }//Tables missing.

        else if ((unsigned long) mysql_num_rows(res_set) == DATABASE_SIZE){
            printf("%s, Tables found.\n", timestamp());
        }//Tables found.
    }
}//check_database_structure

/* connect to server */
void open_database(void){
    /* initialize client library */
    if (mysql_library_init (0, NULL, NULL)){
        my_error_exit("mysql_library_init() failed.\n");
    }
    printf("%s, MySQL Client Library Initialized.\n", timestamp());
    /* initialize connection handler */
    conn = mysql_init (NULL);

    if (conn == NULL){
        fprintf(stderr, "%s, mysql_init() failed (probably out of memory).\n", timestamp());
        exit(2);
    }

    /* connect to server */
    if (mysql_real_connect (conn, host, user, pass, database, port, socket_name, my_flags) == NULL){
        my_error_exit("mysql_real_connect() failed.\n");
    }
    printf("%s, Connected to %s on %s@%s:%d\n", timestamp(),database,user,host,port);
}//open_database

/* disconnect from server */
void close_database(void){
    mysql_close (conn);
    printf("%s, Connection to %s on %s@%s:%d closed.\n", timestamp(), database,user,host,port);
    mysql_library_end();
    printf("%s, MySQL Client Library closed.\n", timestamp());
}//close_database
