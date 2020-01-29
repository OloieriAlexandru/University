CC=gcc
CCPP=g++
SRCMAIN=main
SRCPIPESCMD=pipesCmd
SRCFOLDER=src
BUILDFOLDER=build

all: clean release

release: clean
	${CCPP} ./${SRCFOLDER}/${SRCMAIN}.cpp -o ./${BUILDFOLDER}/${SRCMAIN}.bin
	${CC} ./${SRCFOLDER}/${SRCPIPESCMD}.c -o ./${BUILDFOLDER}/${SRCPIPESCMD}.bin

run: release
	./${BUILDFOLDER}/${SRCMAIN}.bin

clean:
	rm -f ${BUILDFOLDER}/${SRCMAIN}.bin
	rm -f ${BUILDFOLDER}/${SRCPIPESCMD}.bin
