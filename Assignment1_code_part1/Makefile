# OSAS framework makefile,
# parameters:
# BSLCOM=<comport minus 1>, example make program BSLCOM=3
# MOTEID=<moteaddress>, example MOTEID=0x56

MOTEID=0x57

# makfile configuration
NAME            = SchedTest
#OBJECTS         = SchedTest.o Led.o Clock.o Scheduler.MultiTask.o Semaphore.o
#OBJECTS         = SchedTest.o Led.o Clock.o Scheduler.MultiTask.o 
#OBJECTS         = SchedTest.o Led.o Clock.o SchedulerPre.o 
OBJECTS         = SchedTest.o Led.o Clock.o Context.o SchedulerPre.o 
#OBJECTS         = SchedTest.o Led.o Clock.o SchedulerPre.o Uart.o 
#OBJECTS         = SchedTest.o Led.o Clock.o SchedulerNPBasic.o 
#OBJECTS         = SchedTest.o Led.o Clock.o SchedulerNP.o 
CPU             = msp430x1611

CFLAGS          = -mmcu=${CPU} -Os -Wall -Winline -g -DNODE_ADDR=${MOTEID} 
# -fno-inline-functions
MAPFLAGS	= -Wl,-Map=${NAME}.map,--cref

# bootloader options
# 0->COM1 / ttyS0
# 1->COM2 / ttyS1
# BSLCOM		= 2

#prefix
PREFIX		= msp430-
#switch the compiler (for the internal make rules)
CC              = ${PREFIX}gcc
SIZE		= ${PREFIX}size


.PHONY: all FORCE clean download download-jtag download-bsl dist

#all should be the first target. it's built when make is run without args
all: ${NAME}.elf ${NAME}.a43 ${NAME}.lst
	${SIZE} ${NAME}.elf

#confgigure the next line if you want to use the serial download
#download: download-jtag
download: download-bsl

#additional rules for files
${NAME}.elf: ${OBJECTS}
	${CC} -mmcu=${CPU} -o $@ ${OBJECTS} ${MAPFLAGS}

${NAME}.a43: ${NAME}.elf
	msp430-objcopy -O ihex $^ $@

${NAME}.lst: ${NAME}.elf
	msp430-objdump -dSt $^ >$@


download-jtag: all
	msp430-jtag -e ${NAME}.elf

download-bsl: all
	msp430-bsl-telos --telos -c ${BSLCOM} -r -e -p ${NAME}.elf

clean:
	rm -f ${NAME}.elf ${NAME}.a43 ${NAME}.lst ${OBJECTS} ${NAME}.map

#backup archive
dist:
	tar -zcvf dist-'date +%y%m%d_%H%M'.tar.gz *.c *.h make*

Debug: all

Release: all

cleanDebug: clean

cleanRelease: clean

#dummy target as dependecy if something has to be build everytime
FORCE:

#project dependencies

Clock.o: Clock.h
Led.o: Led.h
Context.o: Context.h
SchedTest.o: Led.h
SchedTest.o: Clock.h
SchedTest.o: Scheduler.h
SchedulerNPBasic.o: Scheduler.h
