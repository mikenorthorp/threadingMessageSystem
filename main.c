
#include "pmessages.h"
#include "list.h"
#include <string.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define CHAR_MAX (50)
#define BUF_MAX (80)
#define TICKET_SECRET_MAX (10)
#define SECRET_MAX (21)
#define USER_LIST_MAX (81)
#define SERVICE_NAME_MAX (5)


//Global threads
// Create auth thread
pthread_t authThreadRef;

//Create client thread
pthread_t clientThreadRef;

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
List_t *readInUsersFromFile(char *filename)
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
                    printf("Unable to allocate memory for struct\n");
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
        printf("Could not initialize list\n");
    }

    //End file reading
    printf("Return from file read\n");
    //Return a linked list with all values
    return userPassList;
}

// Functions for creating each thread
void cleanUp()
{
    // Deallocate stuff

}

// Main client thread logic
void *clientThread()
{

    pthread_t receive;
    char *messageReceived;
    int size;

    printf("Client thread ready to send\n");

    // Send message to auth thread
    if (send_message_to_thread( authThreadRef, "mike:bobby", strlen("mike:bobby") + 1) != MSG_OK)
    {
        printf( "first failed\n" );
    }

    // Wait for username and pass auth string

    printf("Client thread waiting...");
    // Receive response
    if (receive_message( &receive, &messageReceived, &size) == MSG_OK)
    {
        printf ("Client received message 1--%s--size %d\n", messageReceived, size );
    }
    else
    {
        printf ("first receive failed\n");
    }
}

// Main authThread logic
void *authThread(void *auth)
{
    List_t *userPassList;
    userPassFile *lastNode = NULL;
    userPassFile *userPassNode = NULL;
    userPassList = readInUsersFromFile(((authStruct *)auth)->fileName);

    // Wait for username pass to authenticate from another thread
    pthread_t receive;
    char *messageReceived;
    int isAuthed;
    int size;

    isAuthed = 0;


    printf("Auth thread ready to receive...");
    if (receive_message( &receive, &messageReceived, &size) == MSG_OK)
    {
        printf ("Auth received message 1--%s--size %d\n", messageReceived, size );
        // Check if matches list of username and passwords
        // If the head is not null
        if ((userPassList != NULL) && (userPassList->head != NULL))
        {
            // Get the head node of the list
            List_head_info(userPassList, (void *)&userPassNode);
            // Loop through the list until null to check for authorized user:pass
            while (userPassNode != NULL)
            {
                if (strcmp(userPassNode->usernamePassword, messageReceived) == 0)
                {
                    isAuthed = 1;
                }
                // Get next node
                List_next_node(userPassList, (void *)&lastNode, (void *)&userPassNode);
            }

            // Send message back to client thread that auth passed with authentication string
            if (isAuthed == 1)
            {
                // Send message to auth thread
                if (send_message_to_thread( clientThreadRef, "Passed", strlen("Passed" + 1) != MSG_OK))
                {
                    printf( "Auth 1 first failed\n" );
                }
            }
            // Send message back to client thread that it failed auth
            else
            {
                // Send message to auth thread
                if (send_message_to_thread( clientThreadRef, "Failed", strlen("Failed" + 1) != MSG_OK))
                {
                    printf( "Auth 0 first failed\n" );
                }
            }
        }
    }
    else
    {
        printf ("first receive failed\n");
    }

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


int main( int argc, char *argv[] )
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

    //Get mailbox ready
    messages_init();

    // Create auth struct
    authStruct *auth;

    // Add to auth struct
    auth = (authStruct *) malloc( sizeof( authStruct ) );

    // Add values to auth struct
    strncpy( auth->fileName, argv[1], CHAR_MAX - 1 );
    auth->fileName[CHAR_MAX - 1] = '\0'; /* Make sure that it is null-terminated. */

    strncpy( auth->secretValue, superSecretTicketGrantingThread, TICKET_SECRET_MAX - 1 );
    auth->secretValue[TICKET_SECRET_MAX] = '\0'; /* Make sure that it is null-terminated. */

    // Create client thread
    if (pthread_create(&clientThreadRef, NULL, clientThread, NULL))
    {
        fprintf(stderr, "Error creating thread\n");
    }

    // Create auth thread
    if (pthread_create(&authThreadRef, NULL, authThread, auth))
    {
        fprintf(stderr, "Error creating thread\n");
    }


    // Wait for client thread to end
    if (pthread_join(clientThreadRef, NULL))
    {
        fprintf(stderr, "Error joining thread\n");
        return 2;
    }

    // Wait for auth thread to end
    if (pthread_join(authThreadRef, NULL))
    {
        fprintf(stderr, "Error joining thread\n");
        return 2;
    }

    // End program
    return 1;
}

