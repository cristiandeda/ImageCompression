build: main.o quadtree.o quadtree.h
	gcc -std=c11 -o quadtree main.o quadtree.o -g

main.o: main.c
	gcc -c main.c

quadtree.o: quadtree.c
	gcc -c quadtree.c

clean:
	rm -r 'quadtree'
	rm -r *.o
