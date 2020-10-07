CFLAGS=-g -Wall -pipe -O0
LFLAGS=

all: cache_example cache_example-cache-annotate.log cache_example-branch-annotate.log 

cache_example: cache_example.c
	$(CC) -o $@ $< $(CFLAGS) $(LFLAGS)


%-cache.log %-cache.cg: %
	valgrind --log-file=$*-cache.log --tool=cachegrind --cachegrind-out-file=$*-cache.cg --cache-sim=yes ./$^

%-branch.log %-branch.cg: %
	valgrind --log-file=$*-branch.log --tool=cachegrind --cachegrind-out-file=$*-branch.cg --branch-sim=yes --cache-sim=no ./$^

%-branch-annotate.log: %-branch.cg
	callgrind_annotate $^  --auto=yes > $@

%-cache-annotate.log: %-cache.cg
	callgrind_annotate $^  --auto=yes --show=Ir,I1mr,ILmr,Dr,D1mr,DLmr > $@

clean:
	rm -f cache_example cachegrind.out.* cache_example*.log

