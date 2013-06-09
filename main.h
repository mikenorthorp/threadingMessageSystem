
#ifndef _main_h_
#define _main_h_

#define CHAR_MAX (150)
#define BUF_MAX (80)
#define TICKET_SECRET_MAX (10)
#define SECRET_MAX (21)
#define USER_LIST_MAX (81)
#define SERVICE_NAME_MAX (5)

/****************************************
 *  Define structs for different threads
 ****************************************/

// Auth struct includes a filename and secret value
typedef struct
{
    char fileName[CHAR_MAX];
    char secretValue[TICKET_SECRET_MAX];
} authStruct;

// The userPassFile struct stores the username and password as a string
typedef struct
{
    char usernamePassword[CHAR_MAX];
} userPassFile;

/* The serviceFile struct stores the secretValue of ticket granting thread, service name, secret and userlist
   from the serviceFile read in */
typedef struct
{
    char secretValue[TICKET_SECRET_MAX];
    char service[SERVICE_NAME_MAX];
    char secret[SECRET_MAX];
    char userList[CHAR_MAX];
} serviceFile;



/***********************************
 * File Read In Functions
 ***********************************/

List_t *readInServiceFile(char *filename);
List_t *readInUsersFromFile(char *filename);


/***********************************
 * Thread Functions
 ***********************************/

void *serviceThread();
void *ticketGrantingThread();
void *clientThread();
void *authThread(void *auth);

#endif /* _main_h_ */

