.PHONY: run_no_clear
ifndef $(CC)
	CC = gcc
endif

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
	clear
	make compile
	valgrind \
		--main-stacksize=99999999 \
		./main

clean:
	rm -f main
