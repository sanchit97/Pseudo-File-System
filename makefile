complete: compile run clean
run:
	@./final
compile:
	@gcc -c menu.c
	@gcc -c diagnosticapi.c
	@gcc -c fileapi.c
	@gcc -o final menu.o diagnosticapi.o fileapi.o
clean:
	@rm -rf menu.o diagnosticapi.o fileapi.o final
