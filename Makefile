FLAGS := -std=gnu99 -Wall

all: pgmrotate pgmblur pgmblur2

clobber: clean

pgmrotate: pgmrotate.c pgmutils.c
	gcc pgmrotate.c pgmutils.c $(FLAGS) -o pgmrotate -lnetpbm

pgmblur: pgmblur.c pgmutils.c
	gcc pgmblur.c pgmutils.c $(FLAGS) -o pgmblur -lnetpbm

pgmblur2: pgmblur2.c pgmutils.c
	gcc pgmblur2.c pgmutils.c $(FLAGS) -o pgmblur2 -lnetpbm

clean:
	rm -f pgmrotate pgmblur pgmblur2

dist:
	make clobber
	tar czvf ../3042_assignment1.tgz *.c *.h README Makefile
