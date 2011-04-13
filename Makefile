FLAGS := -std=c99

all: pgmrotate

pgmrotate: pgmrotate.c
	gcc pgmrotate.c $(FLAGS) -o pgmrotate -lnetpbm

clean:
	rm -f pgmrotate
