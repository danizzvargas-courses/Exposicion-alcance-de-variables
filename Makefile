CC      = clang
CFLAGS  = -Xpreprocessor -fopenmp -I/opt/homebrew/opt/libomp/include
LDFLAGS = -L/opt/homebrew/opt/libomp/lib -lomp

all: ThreadPrivate FirstPrivate LastPrivate

ThreadPrivate:
	$(CC) $(CFLAGS) $(LDFLAGS) src/ThreadPrivate.c -o ThreadPrivate

FirstPrivate:
	$(CC) $(CFLAGS) $(LDFLAGS) src/FirstPrivate.c -o FirstPrivate

LastPrivate:
	$(CC) $(CFLAGS) $(LDFLAGS) src/LastPrivate.c -o LastPrivate

run-thread: ThreadPrivate
	./ThreadPrivate

run-first: FirstPrivate
	./FirstPrivate

run-last: LastPrivate
	./LastPrivate

clean:
	rm -f ThreadPrivate FirstPrivate LastPrivate
