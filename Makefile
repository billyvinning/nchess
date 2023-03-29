
EXE:=termichess

all:
	gcc -std=c99 main.c -o ${EXE}

clean:
	rm -f ${EXE}

