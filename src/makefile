CC=avr-gcc
EXECUTABLE=test1
TARGET=atmega328

OBJECTS=threading/scheduler.o\
	main.o

all: ${OBJECTS}
	${CC} -mmcu=${TARGET} -o ${EXECUTABLE} ${OBJECTS}

clean:
	rm -f ${OBJECTS} ${EXECUTABLE}

threading/scheduler.o: threading/scheduler.S
	${CC} -mmcu=${TARGET} -o threading/scheduler.o -c threading/scheduler.S

main.o: main.c
	${CC} -mmcu=${TARGET} -o main.o -c main.c
