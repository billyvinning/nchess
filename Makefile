
EXE:=termichess

all:
	gcc -std=c99 main.c -o ${EXE} -lncurses -lm

clean:
	rm -f ${EXE}

