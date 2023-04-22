.PHONY: run_no_clear
CC = gcc-12
run_no_clear:
	make clean
	make compile
	./main

run:
	clear
	make run_no_clear

compile: main.c
	$(CC) \
	main.c \
	-o main \
	`sdl2-config --cflags --libs` \
	-I/opt/local/include/ \
	-lm

debug:
	make compile
	valgrind ./main

clean:
	rm -f main
