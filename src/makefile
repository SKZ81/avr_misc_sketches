CC=avr-gcc
EXECUTABLE=test1
TARGET=atmega328

CFLAGS+=-g -mmcu=${TARGET}

OBJECTS=threading/scheduler.o\
	thread2.o\
	main.o

all: ${OBJECTS}
	${CC} -mmcu=${TARGET} -o ${EXECUTABLE} ${OBJECTS}

clean:
	rm -f ${OBJECTS} ${EXECUTABLE}

threading/scheduler.o: threading/scheduler.S
	${CC} ${CFLAGS} -o threading/scheduler.o -c threading/scheduler.S

thread2.o: thread2.S
	${CC} ${CFLAGS} -o thread2.o -c thread2.S

main.o: main.c
	${CC} ${CFLAGS} -o main.o -c main.c
