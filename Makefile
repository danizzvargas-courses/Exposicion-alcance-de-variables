CC      ?= gcc
UNAME_S := $(shell uname -s)

ifeq ($(UNAME_S),Darwin)
  CFLAGS  = -Xpreprocessor -fopenmp -I/opt/homebrew/opt/libomp/include
  LDFLAGS = -L/opt/homebrew/opt/libomp/lib -lomp
else
  CFLAGS  = -fopenmp
  LDFLAGS = -fopenmp
endif

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

run-tests:
	gcc -fopenmp test/test.c -o test_runner && ./test_runner
	rm -f test_runner LastPrivate_test FirstPrivate_test ThreadPrivate_test

clean:
	rm -f ThreadPrivate FirstPrivate LastPrivate test_runner \
	      LastPrivate_test FirstPrivate_test ThreadPrivate_test
