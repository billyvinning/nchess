
EXE:=nchess

all:
	gcc -std=c99 src/main.c -o ${EXE} -lncurses -lm

clean:
	rm -f ${EXE}

