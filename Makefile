    CC     = gcc -g
    CFLAGS = 
    LFLAGS = -lm

      PROG = invmat
      OBJS = ./src/utils/utils.o \
             ./src/sislin/sislin.o \
             ./src/interface/interface.o \
             ./src/methods/methods.o \
             ./src/memoryAlloc/memoryAlloc.o \
             ./src/sisUtils/sisUtils.o \
             ./src/$(PROG).o

.PHONY: limpa faxina clean distclean purge all

%.o: ./src/%/%.c ./src/%/%.h
	$(CC) -c $(CFLAGS) $<

$(PROG):  $(OBJS)
	$(CC) -o $@ $^ $(LFLAGS)

clean: purge
	@rm -f *~ *.bak

purge: 
	@rm -f *.o core a.out
	@rm -f *.txt
	@rm -f $(PROG)
	@rm -f $(OBJS)

