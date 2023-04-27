.PHONY: run_no_clear
run_no_clear:
	make clean
	make compile
	./main

run:
	clear
	make run_no_clear

compile: main.c
	gcc \
	main.c \
	-o main \
	`sdl2-config --cflags --libs` \
	-lm

debug:
	clear
	make compile
	valgrind \
		--main-stacksize=99999999 \
		./main

clean:
	rm -f main
