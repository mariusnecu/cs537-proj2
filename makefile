SRC = vmsim.c
OBJS = tlbtest.o wsstest.o pttest.o prtest.o
CC = gcc
CFLAGS = -g -Wall -std=c99
LIBS = -lm

all : ${OBJS}
	${CC} ${CFLAGS} -o vmsim ${SRC} ${OBJS} ${LIBS}

clean :
	rm -f *.o
	rm -f vmsim

%.o : %.c
	${CC} ${CFLAGS} -c $<
