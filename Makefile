
EXE:=nchess

all:
	gcc -std=c99 src/main.c -o ${EXE} -DNCURSES_WIDECHAR=1  -lncursesw -lm

clean:
	rm -f ${EXE}

