CFLAGS  = 
LDFLAGS =

all: test1 test2 test3 test4 test5

test1: test1.o mvalloc.o
	$(CC) $(LDFLAGS) -o test1  mvalloc.o test1.o

test2: test2.o mvalloc.o
	$(CC) $(LDFLAGS) -o test2  mvalloc.o test2.o

test3: test3.o mvalloc.o
	$(CC) $(LDFLAGS) -o test3  mvalloc.o test3.o

test4: test4.o mvalloc.o
	$(CC) $(LDFLAGS) -o test4  mvalloc.o test4.o

test5: test5.o mvalloc.o
	$(CC) $(LDFLAGS) -o test5  mvalloc.o test5.o

clean:
	/bin/rm -f *.o test1 test2 test3 test4 test5

test1.o: mvalloc.h
test2.o: mvalloc.h
test3.o: mvalloc.h
test4.o: mvalloc.h
test5.o: mvalloc.h
mvalloc.o: mvalloc.h
