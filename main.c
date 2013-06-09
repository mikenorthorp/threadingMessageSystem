
#include "pmessages.h"
#include "list.h"
#include <string.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define CHAR_MAX (50)
#define BUF_MAX (80)
#define TICKET_SECRET_MAX (10)
#define SECRET_MAX (21)
#define USER_LIST_MAX (81)
#define SERVICE_NAME_MAX (5)


//Define structs for different stuff


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




// Return a list with all the users in it
List_t* readInUsersFromFile(char *filename)
{
    char line[80];
    char parsed[80];
    List_t *userPassList;
    userPassFile *userPassNode;

    // Malloc list
    userPassList = (List_t *) malloc( sizeof( List_t ) );

    if (List_init( userPassList ))
    {

        // Open filename given
        // TODO: Error checking for filename read in
        stdin = fopen (filename, "r");

        if (stdin != NULL)
        {

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
                    if (List_add_tail( userPassList, (void *)userPassNode ) == 0)
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
            printf("Could not open file %s\n", filename);
        }
    }
    else
    {
        printf("Could not initialize list");
    }

    //End file reading

    //Return a linked list with all values
    return userPassList;
}

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
void clientThread()
{

}

// Main authThread logic
void *authThread(void *auth)
{
    List_t *userPassList;
    userPassFile *userPassNode;
    printf("File name is %s", ((authStruct *)auth)->fileName);
    userPassList = readInUsersFromFile(((authStruct *)auth)->fileName);

    // Get head of list to check
    List_head_info(userPassList, (void *)&userPassNode);

    printf("Username and password at head is: %s", userPassNode->usernamePassword);
    return NULL;
}


// Ticket granting thread
void createTicketGrantingThread()
{

}

// Service Thread (can create multiple)
void createServiceThread()
{

}

void readInServices()
{

}


int
main( int argc, char *argv[] )
{

    // Declare variables
    char superSecretTicketGrantingThread[TICKET_SECRET_MAX] = "apple";

    // Read in from command line for authFile name and serviceFile name
    if (argc == 3)
    {
        printf("AuthFile name is %s and ServiceFile name is %s\n\n", argv[1] , argv[2]);
    }
    // No paramters read in
    else
    {
        printf ("Incorrect number of parameters read in, exiting\n");
        return 1;
    }


    // Creat auth thread
    pthread_t authThreadRef;
    authStruct *auth;

    // Add to auth struct
    auth = (authStruct *) malloc( sizeof( authStruct ) );

    // Add values to auth
    strncpy( auth->fileName, argv[1], CHAR_MAX - 1 );
    auth->fileName[CHAR_MAX - 1] = '\0'; /* Make sure that it is null-terminated. */

    strncpy( auth->secretValue, superSecretTicketGrantingThread, TICKET_SECRET_MAX - 1 );
    auth->secretValue[TICKET_SECRET_MAX] = '\0'; /* Make sure that it is null-terminated. */

    if (pthread_create(&authThreadRef, NULL, authThread, auth))
    {
        fprintf(stderr, "Error creating thread\n");
    }

    if (pthread_join(authThreadRef, NULL))
    {
        fprintf(stderr, "Error joining thread\n");
        return 2;
    }

    // End program
    return 1;
}

