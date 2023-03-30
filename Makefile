
EXE:=termichess

all:
	gcc -std=c99 main.c -o ${EXE} -lncurses

clean:
	rm -f ${EXE}

