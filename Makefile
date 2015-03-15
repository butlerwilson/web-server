USER_RESOURCE = ./src/server.c
BIN_PATH = ./bin/
LIB_PATH = ./lib/
<<<<<<< HEAD
INCLUDE_PATH= ./include
=======
INCLUDE_PATH= /home/young/Learner/applications/BS/server/include
>>>>>>> origin/master
DESTINATION = server
LIB_CODE = server.o

MYSQL=$(shell mysql_config --cflgas --libs)

#$($(BIN_PATH)DESTINATION) : $(LIB_PATH$(LIB_CODE))
#	gcc $(LIB_PATH$(LIB_CODE)) -o $($(BIN_PATH)DESTINATION)
#$(LIB_PATH$(LIB_CODE)) : $(USER_RESOURCE)
#	gcc $(USER_RESOURCE) -c -o $(LIB_PATH$(LIB_CODE))

$(BIN_PATH)$(DESTINATION) :
	gcc $(USER_RESOURCE) -I$(INCLUDE_PATH) $(shell mysql_config --cflags --libs) -D_REENTRANT -o $(BIN_PATH)$(DESTINATION) -lpthread

debugger:
	gcc $(USER_RESOURCE) -I$(INCLUDE_PATH) $(shell mysql_config --cflags     --libs) -D_REENTRANT -o $(BIN_PATH)$(DESTINATION) -lpthread -g
	gdb $(BIN_PATH)$(DESTINATION)
clean:
	rm $(BIN_PATH)$(DESTINATION)

#./$(BIN_PATH)$(DESTINATION)
