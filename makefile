complete: compile run clean
run:
	./run
compile:
	gcc -c menu.c
	gcc -o run menu.o

clean:
	rm -rf run menu.o
