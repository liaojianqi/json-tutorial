CC = gcc

default:
	make run
leptjson.o: leptjson.h leptjson.c
	$(CC) -c leptjson.c
leptjson_test: test.c leptjson.o
	$(CC) test.c leptjson.o -o leptjson_test
run: leptjson_test
	./leptjson_test
clean:
	rm *.o leptjson_test *.out