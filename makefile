
#Thisnext linedefines arguments that are passed to all compilation steps

CCFLAGS = -g -Wall -pthread

test_messages: test_messages.c pmessages.h pmessages.o list.o
	gcc $(CCFLAGS) -o test_messages test_messages.c pmessages.o list.o

list.o: list.c list.h
	gcc $(CCFLAGS) -c list.c
pmessages.o: pmessages.c pmessages.h pmessages_private.h
	gcc $(CCFLAGS) -c pmessages.c

clean:
	-rm list.o pmessages.o

spotless: clean
	-rm test_messages

