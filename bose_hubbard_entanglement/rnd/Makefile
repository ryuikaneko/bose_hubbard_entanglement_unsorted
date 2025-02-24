PROGRAM = main
OBJS = main.o pcg_basic.o

CC = gcc
CFLAGS = -O3

#CC = icc
#CFLAGS = -fast

LDLIBS = -lm

.SUFFIXES: .c .o

$(PROGRAM): $(OBJS)
	$(CC) -o $(PROGRAM) $(LDFLAGS) $^ $(LDLIBS)

%: %.c
	$(CC) $(CFLAGS) $(LDFLAGS) $^ $(LDLIBS) -o $@

.PHONY: clean
clean:
	$(RM) $(OBJS)
#	$(RM) $(PROGRAM) $(OBJS)
