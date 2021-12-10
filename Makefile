# This is a comment line

mips: proj1.o 
	gcc -o mips proj1.o 

proj1.o: *.c
	gcc -c -std=c99 -O2 *.c



clean:
	rm *.o mips
