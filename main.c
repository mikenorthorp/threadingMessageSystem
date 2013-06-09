
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
    char fileName[CHAR_MAX];
    char secretValue[TICKET_SECRET_MAX];
} authStruct;

// Struct to store lines from username password file
typedef struct
{
    char usernamePassword[CHAR_MAX];
} userPassFile;

// Struct to store lines from service file
typedef struct
{
    char service[SERVICE_NAME_MAX];
    char secret[SECRET_MAX];
    char userList[USER_LIST_MAX];
} serviceFile;

// A struct containing all lists
typedef struct
{
    List_t userPassFile;
} allLists;


// Functions for creating each thread
void cleanUp()
{
    // Deallocate stuff

}
// Client Thread
void createClientThread()
{

}

// Main client thread logic
void clientThread() {

}

// Authentication Thread
void createAuthenticationThread()
{

}

// Main authThread logic
void authThread() {

}

// Ticket granting thread
void createTicketGrantingThread()
{

}

// Service Thread (can create multiple)
void createServiceThread()
{

}

// Return a list with all the users in it
void readInUsersFromFile(char *filename)
{
    char line[80];
    char parsed[80];
    List_t userPassList;
    userPassFile *userPassNode;

    if (List_init( &userPassList ))
    {

        // Open filename given
        // TODO: Error checking for filename read in
        stdin = fopen (filename, "r");

        while (fgets(line, 80, stdin) != NULL)
        {
            //Read in a line
            sscanf (line, "%s", parsed);

            // Add it to the userpass struct
            userPassNode = (userPassFile *) malloc( sizeof( userPassFile ) );
            if (userPassNode != NULL)
            {
                strncpy( userPassNode->usernamePassword, parsed, CHAR_MAX - 1 );
                userPassNode->usernamePassword[CHAR_MAX] = '\0'; /* Make sure that it is null-terminated. */

                // Add userpass struct to a list
                if (List_add_tail( &userPassList, (void *)userPassNode ) == 0)
                {
                    printf ("Error in inserting the process into the list.\n");
                }
                else
                {
                    printf("Added %sto the list\n", parsed);
                }
            }
            else
            {
                printf("Unable to allocate memory for struct");
            }
        }
        fclose(stdin);  /* close the file prior to exiting the routine */
    }
    else
    {
        printf("Could not initialize list");
    }

    //End file reading

    //Return a linked list with all values
}

void readInServices()
{

}


int
main( int argc, char **argv )
{
    readInUsersFromFile("authFile.txt");
    // Call client thread to start waiting for user input
    createClientThread();


    // End program
    return 1;
}

