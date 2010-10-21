SRC = vmsim.c
OBJS = tlbtest.o wsstest.o pttest.o prtest.o
CC = gcc
CFLAGS = -g

all : ${OBJS}
	${CC} -o vmsim ${SRC} ${OBJS}

clean :
	rm -f *.o
	rm -f vmsim

%.o : %.c
	${CC} ${CFLAGS} -c $<
