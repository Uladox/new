all: clean libtest.so
clean:
	rm -f test.o libtest.so
libtest.so: test.o
	gcc -shared -o libtest.so test.o
test.o: test.c
	gcc -c -Wall -Werror -fpic test.c
