
# Compiler and flags
CC = gcc
CFLAGS = -pthread

# Array Sizes and Thread Sizes
THREADS_1 = 1
THREADS_2 = 2
THREADS_4 = 4
THREADS_8 = 8
THREADS_32 = 32

# Output Executable Name
OUTPUT = sort_program

# Default target
all: sort_1 sort_2 sort_4 sort_8 sort_32

# Compile with 1 thread
sort_1: $(OUTPUT).c
        @echo "Compiling with THREADS=1"
        $(CC) $(CFLAGS) -DTHREADS=1 -o sort_1 $(OUTPUT).c

# Compile with 2 threads
sort_2: $(OUTPUT).c
        @echo "Compiling with THREADS=2"
        $(CC) $(CFLAGS) -DTHREADS=2 -o sort_2 $(OUTPUT).c

# Compile with 4 threads
sort_4: $(OUTPUT).c
        @echo "Compiling with THREADS=4"
        $(CC) $(CFLAGS) -DTHREADS=4 -o sort_4 $(OUTPUT).c

# Compile with 8 threads
sort_8: $(OUTPUT).c
        @echo "Compiling with THREADS=8"
        $(CC) $(CFLAGS) -DTHREADS=8 -o sort_8 $(OUTPUT).c

# Compile with 32 threads
sort_32: $(OUTPUT).c
        @echo "Compiling with THREADS=32"
        $(CC) $(CFLAGS) -DTHREADS=32 -o sort_32 $(OUTPUT).c

# Clean all compiled files
clean:
        rm -f sort_1 sort_2 sort_4 sort_8 sort_32 $(OUTPUT)
