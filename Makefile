FLAGS := -std=c99

all: pgmrotate pgmblur

pgmrotate: pgmrotate.c pgmutils.c
	clang pgmrotate.c pgmutils.c $(FLAGS) -o pgmrotate -lnetpbm

pgmblur: pgmblur.c pgmutils.c
	clang pgmblur.c pgmutils.c $(FLAGS) -o pgmblur -lnetpbm

clean:
	rm -f pgmrotate pgmblur
