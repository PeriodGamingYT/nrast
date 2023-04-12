make: main.c
	gcc \
	main.c \
	-o main \
	`sdl2-config --cflags --libs` \
	-lm

clean:
	rm -f main

run:
	clear
	make clean
	make
	./main
