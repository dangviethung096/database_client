#makefile
LIB_PATH=${PWD}/lib
CC=gcc
OBJECT_FILE=client.o sql_interpreter.o process_message.o
FLAG=-Wall -g
BIN_PATH={PWD}/../bin
MACRO_AT_BUILD=
#-DCLIENT_TRACE_ENABLE
HEADER_FILE=${LIB_PATH}/client_api.h  ${LIB_PATH}/client_def.h  ${LIB_PATH}/client_struct.h
all: clean $(OBJECT_FILE)
	${CC} ${FLAG} ${OBJECT_FILE} -o ${PWD}/../bin/client.out
client.o: ${HEADER_FILE} client.c
	${CC} ${FLAG} -c client.c -o $@ ${MACRO_AT_BUILD}
process_message.o: ${HEADER_FILE} process_message.c
	${CC} ${FLAG} -c process_message.c -o $@ ${MACRO_AT_BUILD}
sql_interpreter.o: ${HEADER_FILE} sql_interpreter.c
	${CC} ${FLAG} -c sql_interpreter.c -o $@ ${MACRO_AT_BUILD}

.PHONY: clean

clean:
	rm -rf *.o ${BIN_PATH}/*