CFLAGS=-g -Wall -pipe -lm

# Turn on the gperf profiling
CFLAGS+=-pg

profile_example: profile_example.c
	$(CC) -o $@ $< $(CFLAGS) $(LFLAGS)

%.prof: %
	-./$<
	gprof -b $< > $@

clean:
	rm -f profile_example
	rm -f gmon.out perf.data*