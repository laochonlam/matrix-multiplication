EXEC = native_parallel_multiplication strassens_parallel_multiplication
CC ?= gcc 
CFLAGS = 
SRCS_common = main.c

native_parallel_multiplication: $(SRCS_common)
	$(CC) $(CFLAGS) -DNATIVE_PARALLEL -o $@ $(SRCS_common)

strassens_parallel_multiplication: $(SRCS_common)
	$(CC) $(CFLAGS) -DSTRASSENS_PARALLEL -o $@ $(SRCS_common)

all: $(EXEC)