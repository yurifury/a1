FLAGS := -std=c99 -Wall

all: pgmrotate pgmblur pgmblur2

pgmrotate: pgmrotate.c pgmutils.c
	clang pgmrotate.c pgmutils.c $(FLAGS) -o pgmrotate -lnetpbm

pgmblur: pgmblur.c pgmutils.c
	clang pgmblur.c pgmutils.c $(FLAGS) -o pgmblur -lnetpbm

pgmblur2: pgmblur2.c pgmutils.c
	clang pgmblur2.c pgmutils.c $(FLAGS) -o pgmblur2 -lnetpbm

clean:
	rm -f pgmrotate pgmblur pgmblur2
