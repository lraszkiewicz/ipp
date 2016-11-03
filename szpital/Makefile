CFLAGS=-c -Wall -std=c99 -O2

hospital: hospital.o parse.o structure.o
	gcc -o hospital hospital.o parse.o structure.o

hospital.dbg: hospital_dbg.o parse_dbg.o structure_dbg.o
	gcc -g -o hospital.dbg hospital_dbg.o parse_dbg.o structure_dbg.o

.PHONY: debug
debug: hospital.dbg

.c.o:
	gcc $(CFLAGS) $<

hospital.o: hospital.c parse.h structure.h
parse.o: parse.c
structure.o: structure.c

hospital_dbg.o: hospital.c parse.h structure.h
	gcc $(CFLAGS) -g hospital.c -o hospital_dbg.o

parse_dbg.o: parse.c
	gcc $(CFLAGS) -g parse.c -o parse_dbg.o

structure_dbg.o: structure.c
	gcc $(CFLAGS) -g structure.c -o structure_dbg.o

.PHONY: clean
clean:
	@rm -f hospital hospital.dbg hospital.o hospital_dbg.o parse.o parse_dbg.o structure.o structure_dbg.o
