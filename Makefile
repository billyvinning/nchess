
EXE:=termichess

all:
	gcc -std=c99 main.c -o ${EXE} -DNCURSES_WIDECHAR=1  -lncursesw -lm

clean:
	rm -f ${EXE}

