/* main.c
   Michael Northorp - CSCI 3120 - Assignment 2
   This program uses threads and a mailbox system to authenticate users
   read in from a file, allow them to run services read in from another file
   and return the result of a service call to the main client thread.

   This program only has two threads working fully with each other. That is the
   client thread and the authentication thread. The program allows creates a
   client thread which allows the user to input a username and password. It then
   sends this to the authentication thread through the mailbox system which then
   checks agaisnt a list of read in usernames and passwords to see if it should
   authenticate. If it authenticates, the thread sends a message back to the client
   thread with the authentication string. */

#include "pmessages.h"
#include "list.h"
#include "main.h"
#include <string.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/***********************************
 *  Global Variables
 ***********************************/

//Create client thread
pthread_t clientThreadRef;

// Create auth thread
pthread_t authThreadRef;

//Create service thread
pthread_t serviceThreadRef;

// Create ticket granting thread
pthread_t ticketThreadRef;

// Ticket granting thread secret
char ticketGrantingSecret[TICKET_SECRET_MAX] = "apple";



/***********************************
 *  Main
 ***********************************/
int main( int argc, char *argv[] )
{

    //Get mailbox ready
    messages_init();

    // Read in from command line for authFile name and serviceFile name
    if (argc != 3)
    {
        printf ("Incorrect number of parameters read in, exiting\n");
        return 1;
    }

    // Define a list for service file and read it in
    List_t *serviceFileList;
    serviceFileList = readInServiceFile(argv[2]);

    // Create auth struct
    authStruct *auth;

    // Allocate memory for auth struct
    auth = (authStruct *) malloc( sizeof( authStruct ) );

    // Add values to auth struct for filename and secret value of ticket thread
    strncpy( auth->fileName, argv[1], CHAR_MAX - 1 );
    auth->fileName[CHAR_MAX - 1] = '\0'; /* Make sure that it is null-terminated. */
    strncpy( auth->secretValue, ticketGrantingSecret, TICKET_SECRET_MAX - 1 );
    auth->secretValue[TICKET_SECRET_MAX] = '\0'; /* Make sure that it is null-terminated. */

    /* Create client thread, auth thread, service thread and ticket thread */
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

    // Create service thread
    if (pthread_create(&serviceThreadRef, NULL, serviceThread, NULL))
    {
        fprintf(stderr, "Error creating thread\n");
    }

    // Create ticket granting thread
    if (pthread_create(&ticketThreadRef, NULL, ticketGrantingThread, serviceFileList))
    {
        fprintf(stderr, "Error creating thread\n");
    }


    /* Wait for threads to finish */

    // Wait for service thread to end
    if (pthread_join(serviceThreadRef, NULL))
    {
        fprintf(stderr, "Error joining thread\n");
        return 2;
    }

    // Wait for ticket granting thread to end
    if (pthread_join(ticketThreadRef, NULL))
    {
        fprintf(stderr, "Error joining thread\n");
        return 2;
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


/***********************************
 *  Thread Functions
 ***********************************/

/* The client thread reads in a username and password and then tries to authenticate with the
   authentication thread. If it receives a proper message back it should allow the user to
   run a service name with two arguments */
void *clientThread()
{
    char   inputbuffer[BUF_MAX];
    char   username[CHAR_MAX];
    char   password[CHAR_MAX];
    char   messageToSend[CHAR_MAX];
    pthread_t receive;
    char *messageReceived;
    int size;

    //Read in user input
    printf("Enter a username: ");
    if (fgets( inputbuffer, BUF_MAX - 1, stdin ))
    {
        /* Put the parameters into a PCB and store it in the list of processes. */
        if (sscanf( inputbuffer, "%s", username) == 1)
        {
        }
        else
        {
            printf ("Incorrect number of parameters read.\n");
        }
    }
    // Nothing to read in
    else
    {
        printf ("Nothing read in.\n");
    }

    //Read in user input
    printf("Enter your password: ");
    if (fgets( inputbuffer, BUF_MAX - 1, stdin ))
    {
        /* Put the parameters into a PCB and store it in the list of processes. */
        if (sscanf( inputbuffer, "%s", password) == 1)
        {
            //Read in password
        }
        else
        {
            printf ("Incorrect number of parameters read.\n");
        }
    }
    // Nothing to read in
    else
    {
        printf ("Nothing read in.\n");
    }


    // Create message of user:pass to send to authentication thread
    strcpy (messageToSend, username);
    strcat (messageToSend, ":");
    strcat (messageToSend, password);

    // Send message to auth thread
    if (send_message_to_thread( authThreadRef, messageToSend, strlen(messageToSend) + 1) != MSG_OK)
    {
        printf( "first failed\n" );
    }

    // Wait for username and pass auth string

    // Receive response from authentication thread
    if (receive_message( &receive, &messageReceived, &size) == MSG_OK)
    {
        printf ("Client received message 1--%s--size %d\n", messageReceived, size );
    }
    else
    {
        printf ("first receive failed\n");
    }

    /* After this I would make logic for reading in a service name with two arguments, then pass those to the ticket
       and service threads, which would then get a response back */

    return NULL;
}

/* The auth thread waits for a user name and pass to get sent to it from the client thread
   which it will then authenticate. It will return a valid authentication string or invalid
   if the user is authenticated or not. */
void *authThread(void *auth)
{
    List_t *userPassList;
    userPassFile *lastNode = NULL;
    userPassFile *userPassNode = NULL;
    userPassList = readInUsersFromFile(((authStruct *)auth)->fileName);
    char messageToSend[CHAR_MAX];
    char username[CHAR_MAX];

    // Wait for username pass to authenticate from another thread
    pthread_t receive;
    char *messageReceived;
    int isAuthed;
    int size;

    isAuthed = 0;

    // Wait to receive a message
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
                // Create message to send back in the form 1:username:password:secretValueTicket
                strcpy (messageToSend, "1:");
                strcat (messageToSend, messageReceived);
                strcat (messageToSend, ":");
                strcat (messageToSend, ((authStruct *)auth)->secretValue);

                // Send message to auth thread
                if (send_message_to_thread( clientThreadRef, messageToSend, strlen(messageToSend) + 1) != MSG_OK)
                {
                    printf( "Auth 1 first failed\n" );
                }
            }
            // Send message back to client thread that it failed auth
            else
            {
                //Get username with regex
                sscanf( messageReceived, "%[^:]", username);

                //Create message to send on failure
                strcpy (messageToSend, "0:");
                strcat (messageToSend, username);
                strcat (messageToSend, ":0");

                // Send message to auth thread
                if (send_message_to_thread( clientThreadRef, messageToSend, strlen(messageToSend) + 1) != MSG_OK)
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

/* This thread is not implemented but would include support for add, sub and mult services, which would wait
   for tickets from the client thread, check if the ticket is valid, perform the requested operation, then send a message
   back to the client thread. */
void *serviceThread()
{

    return NULL;
}

/* This thread is not implemented but would wait for a message from the client thread with an authentication string, service
   and a username. It would then verify that the authentication string is correct and return a valid ticket string to the user. */
void *ticketGrantingThread()
{

    return NULL;
}



/***********************************
 * File Read In Functions
 ***********************************/

/* This function returns a linked list of the serviceFile struct with all of the
   information from the service file in it */
List_t *readInServiceFile(char *filename)
{
    /* Read in file from argument supplied
       Loop through each line
       Store Ticket Secret Value, Service name, secret, and a list of the comma seperated users into a struct
       Add each struct to a linked list so you have a list of services, their secret, and their users
       Return the linked list */

    // Define variables for parseing and storing values
    char line[80];
    char parsed[80];
    char service[80];
    char secret[80];
    char userList[80];
    List_t *returnList;
    serviceFile *serviceNode;
    FILE *file;

    // Malloc list
    returnList = (List_t *) malloc( sizeof( List_t ) );

    if (List_init( returnList ))
    {
        // Open filename given
        file = fopen (filename, "r");

        if (file != NULL)
        {

            while (fgets(line, 80, file) != NULL)
            {
                //Read in a line
                sscanf (line, "%s", parsed);

                //Get parts of service file
                sscanf( parsed, "%[^:]:%[^:]:%s", service, secret, userList);

                // Add it to the userpass struct
                serviceNode = (serviceFile *) malloc( sizeof( serviceFile ) );
                if (serviceNode != NULL)
                {
                    strncpy( serviceNode->secretValue, ticketGrantingSecret, TICKET_SECRET_MAX - 1 );
                    serviceNode->secretValue[TICKET_SECRET_MAX] = '\0'; /* Make sure that it is null-terminated. */

                    strncpy( serviceNode->service, service, CHAR_MAX - 1 );
                    serviceNode->service[CHAR_MAX] = '\0'; /* Make sure that it is null-terminated. */

                    strncpy( serviceNode->secret, secret, CHAR_MAX - 1 );
                    serviceNode->secret[CHAR_MAX] = '\0'; /* Make sure that it is null-terminated. */

                    strncpy( serviceNode->userList, userList, CHAR_MAX - 1 );
                    serviceNode->userList[CHAR_MAX] = '\0'; /* Make sure that it is null-terminated. */

                    // Add userpass struct to a list
                    if (List_add_tail( returnList, (void *)serviceNode ) == 0)
                    {
                        printf ("Error in inserting the process into the list.\n");
                    }
                }
                else
                {
                    printf("Unable to allocate memory for struct\n");
                }

            }
            fclose(file);  /* close the file prior to exiting the routine */
        }
        else
        {
            printf("Could not open file, exiting... %s\n", filename);
            exit(0);
        }
    }
    else
    {
        printf("Could not initialize list\n");
    }


    // End file reading

    //Return a linked list with all values for ticket granting thread
    return returnList;
}

/* This function returns a linked list of all the users and passwords from the
   auth file read in by the auth thread */
List_t *readInUsersFromFile(char *filename)
{
    // Define variables for storing and parsing values
    char line[80];
    char parsed[80];
    List_t *userPassList;
    userPassFile *userPassNode;
    FILE *file;

    // Malloc list
    userPassList = (List_t *) malloc( sizeof( List_t ) );

    // Init the list to store user and pass in
    if (List_init( userPassList ))
    {
        // Open filename given
        file = fopen (filename, "r");

        // Check if file is null
        if (file != NULL)
        {
            // Loop through each line of the file
            while (fgets(line, 80, file) != NULL)
            {
                //Read in a line
                sscanf (line, "%s", parsed);

                // Allocate memory for userPassNode
                userPassNode = (userPassFile *) malloc( sizeof( userPassFile ) );
                if (userPassNode != NULL)
                {
                    // Add username and password to the struct and then into the linked list
                    strncpy( userPassNode->usernamePassword, parsed, CHAR_MAX - 1 );
                    userPassNode->usernamePassword[CHAR_MAX] = '\0'; /* Make sure that it is null-terminated. */

                    // Add userpass struct to a list
                    if (List_add_tail( userPassList, (void *)userPassNode ) == 0)
                    {
                        printf ("Error in inserting the process into the list.\n");
                    }
                }
                else
                {
                    printf("Unable to allocate memory for struct\n");
                }
            }
            fclose(file);  /* close the file prior to exiting the routine */
        }
        else
        {
            printf("Could not open file, exiting... %s\n", filename);
            exit(0);
        }
    }
    else
    {
        printf("Could not initialize list\n");
    }

    //Return a linked list with all values
    return userPassList;
}

