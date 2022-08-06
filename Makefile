    CC     = gcc -g
    CFLAGS = 
    LFLAGS = -lm

      PROG = invmat
      OBJS = utils.o \
             sislin.o \
             interface.o \
             methods.o \
             memoryAlloc.o \
             $(PROG).o

.PHONY: limpa faxina clean distclean purge all

%.o: %.c %.h utils.h sislin.h memoryAlloc.h
	$(CC) -c $(CFLAGS) $<

$(PROG):  $(OBJS)
	$(CC) -o $@ $^ $(LFLAGS)

clean:
	@rm -f *~ *.bak

purge:   clean
	@rm -f *.o core a.out
	@rm -f $(PROG)

