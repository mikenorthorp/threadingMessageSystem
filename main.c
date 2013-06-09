
#include "pmessages.h"
#include "list.h"
#include <string.h>
#include <pthread.h>
#include <stdio.h>

#define CHAR_MAX (30)
#define TICKET_SECRET_MAX (10)
#define SECRET_MAX (21)
#define USER_LIST_MAX (81)
#define SERVICE_NAME_MAX (5)


//Define structs for different stuff
char superSecretTicketGrantingThread[TICKET_SECRET_MAX] = "apple";

// Auth struct
typedef struct
{
    int isAuth;
    char username[CHAR_MAX];
    char password[CHAR_MAX];
    char secretValue[TICKET_SECRET_MAX];
} authStruct;

// Struct to store lines from username password file
typedef struct
{
    char username[CHAR_MAX];
    char password[CHAR_MAX];
} userPassFile;

// Struct to store lines from service file
typedef struct
{
    char service[SERVICE_NAME_MAX];
    char secret[SECRET_MAX];
    char userList[USER_LIST_MAX];
} serviceFile;


// Functions for creating each thread
void cleanUp() {
  // Deallocate stuff

}
// Client Thread
void createClientThread() {

}

// Authentication Thread
void createAuthenticationThread() {

}

// Ticket granting thread
void createTicketGrantingThread() {

}

// Service Thread (can create multiple)
void createServiceThread(){

}

void readInUsers() {

}

void readInServices() {

}


int
main( int argc, char **argv )
{
  // Call client thread to start waiting for user input
  createClientThread();


  // End program
  return 1;
}

