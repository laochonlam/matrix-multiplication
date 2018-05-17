all:
	gcc -o main main.c

test: main
	./main < test_data/input