EXEC = native_parallel_multiplication strassens_parallel_multiplication
CC ?= gcc 
CFLAGS = 
SRCS_common = main.c

native_parallel_multiplication: $(SRCS_common)
	$(CC) $(CFLAGS) -fopenmp -DNATIVE_PARALLEL -o $@ $(SRCS_common)

strassens_parallel_multiplication: $(SRCS_common)
	$(CC) $(CFLAGS) -DSTRASSENS_PARALLEL -o $@ $(SRCS_common)

all: $(EXEC)

clean:	
	@rm $(EXEC)

check:
	@./$(EXEC) <  test_data/input > test_data/output
	@diff test_data/output test_data/expected_output 

