README
======

README and code base best viewed on [Github](https://github.com/mikenorthorp/threadingMessageSystem)


This is an assignment for my Operating Systems class CSCI 3120

This program uses threads and a mailbox system to authenticate users
read in from a file, allow them to run services read in from another file
and return the result of a service call to the main client thread.


This program only has two threads working fully with each other. That is the
client thread and the authentication thread. The program allows creates a
client thread which allows the user to input a username and password. It then
sends this to the authentication thread through the mailbox system which then
checks agaisnt a list of read in usernames and passwords to see if it should
authenticate. If it authenticates, the thread sends a message back to the client
thread with the authentication string.

Requirements
------------

This program requires `gcc` and `make` to compile.

This program is made to run on the bluenose.cs.dal.ca servers but should work
on most linux distros.

Installation
------------

1. Copy files into working directory.
2. run `make` command in terminal
3. run `./main authFile.txt serviceFile.txt`
*You can give the program different file arguments than above, but check below
in the configuration section for the format of each file*

Making the Program Do Things
----------------------------

1. You will be prompted for a username(mike) and password(bobby) or enter any user:pass found in the authFile.txt.
2. This will send a message to the authentication thread which will verify the user and password was correct
3. You will receive an authentication string back to the client thread that includes the secret ticket value (apple)
4. If you enter incorrect credentials, you will receive a string in the form 0:username:password:0
5. The program will then exit

Configuration
-------------

Edit the authFile.txt or make your own, then put usernames and passwords on each
line in the form `username:password`

Edit the serviceFile.txt or make your own, then put service names(add, sub, mult),
secret values, and a list of users that can use that service in the form `service:secret:user1,user2,user3...userN`


Method Descriptions
------------------

#### list.c and list.h - Linked List Structure ####

Linked list structure with methods for adding, removing, and getting
nodes from a list.

#### pmessages.c, pmessages.h and pmessages_private.h - Mailbox system ####

This is a mailbox system to send messages between threads safely.

#### main.c and main.h - Main File ####

This handles the different threads, creates them, and handles the authentication
and message sending for the program.

#### File Read Functions ####

List_t *readInServiceFile(char *filename);

	- Reads in a service file of the format `service:secret:user1,user2,user3...userN` on each line
	It splits each part into a struct, then stores this in a list for each line, then returns the list.

List_t *readInUsersFromFile(char *filename);

	- Reads in a authentication file of the format `username:password` on each line
	It stores each line in a struct, then stores this in a list for each line, then returns the list.


#### Thread Functions ####

void *authThread(void *auth);

	- The auth thread waits for a user name and pass to get sent to it from the client thread
	which it will then authenticate. It will return a valid authentication string or invalid
    if the user is authenticated or not. It authenticates based on the list of usernames:passwords from
    the authentication file read in at the start. It returns NULL. This thread receives and sends messages to the
    client thread.

void *clientThread();

	- The client thread reads in a username and password and then tries to authenticate with the
    authentication thread. If it receives a proper message back it should allow the user to
    run a service name with two arguments (which is not implemented). It returns NULL. This thread sends and receives
    messages from the authentication thread.

void *ticketGrantingThread();

	- This thread is not implemented but would wait for a message from the client thread with an authentication string, service
	and a username. It would then verify that the authentication string is correct and return a valid ticket string to the user.

void *serviceThread();

	- This thread is not implemented but would include support for add, sub and mult services, which would wait
	for tickets from the client thread, check if the ticket is valid, perform the requested operation, then send a message
	back to the client thread.

Citations
=========

The mailbox system was developed from the professor Mike McAllister.






