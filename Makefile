    CC     = gcc -g
    CFLAGS = 
    LFLAGS = -lm

      PROG = invmat
      OBJS = utils.o \
             sislin.o \
             methods.o \
             $(PROG).o

.PHONY: limpa faxina clean distclean purge all

%.o: %.c %.h utils.h sislin.h
	$(CC) -c $(CFLAGS) $<

$(PROG):  $(OBJS)
	$(CC) -o $@ $^ $(LFLAGS)

clean:
	@rm -f *~ *.bak

purge:   clean
	@rm -f *.o core a.out
	@rm -f $(PROG)

