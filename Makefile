BINARIES= \
	serial \
	pthreads \
	libdispatch \
	pthreadspt2 \
	serialpt2

JOBS?=		10
WORK_PER_JOB?=	10

CC=		clang
CFLAGS=		-D JOBS=${JOBS} -D WORK_PER_JOB=${WORK_PER_JOB} \
		-Weverything -Wno-unused-parameter -pthread

# We don't need to link all of the following libraries for every program that
# we're going to compile, but there's no harm in attempting to do so
# (the linker will ignore any code it isn't looking for):
LDFLAGS=	-ldispatch -pthread


all: ${BINARIES}

clean:
	rm -f ${BINARIES}

run: 
	./serial
	./pthreads
	./libdispatch

run2: 
	./runHunnid.sh

run3:
	./libdispatch

ministat-run:
	head -n 3 initial-serial-times.dat | ministat > mini3.txt
	head -n 5 initial-serial-times.dat | ministat  > mini5.txt
	head -n 10 initial-serial-times.dat | ministat > mini10.txt
	head -n 100 initial-serial-times.dat | ministat > mini100.txt
	head -n 1000 initial-serial-times.dat | ministat > mini1000.txt