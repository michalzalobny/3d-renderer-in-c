build:
	gcc -Wall -std=c99 ./src/*.c -o renderer

run:
	./renderer

clean:
	rm renderer