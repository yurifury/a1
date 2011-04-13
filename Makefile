FLAGS := -std=c99

all: pgmrotate

pgmrotate: pgmrotate.c
	gcc pgmrotate.c pgmutils.c $(FLAGS) -o pgmrotate -lnetpbm

clean:
	rm -f pgmrotate
