make: main.c
	gcc \
	-o main \
	`sdl2-config --cflags --libs` \
	main.c

clean:
	rm -f main

run:
	clear
	make clean
	make
	./main
