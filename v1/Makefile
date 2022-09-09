    CC     = gcc -g
    CFLAGS = -O3 -mavx -march=native $(LIKWID_C_FLAGS)
    LFLAGS = -lm $(LIKWID_O_FLAGS)
    LIKWID_O_FLAGS = -L${LIKWID_LIB} -llikwid
    LIKWID_C_FLAGS = -DLIKWID_PERFMON -I${LIKWID_INCLUDE}

    PROG = invmat
    OBJS = ./utils/utils.o \
            ./sislin/sislin.o \
            ./interface/interface.o \
            ./methods/methods.o \
            ./memoryAlloc/memoryAlloc.o \
            ./sisUtils/sisUtils.o \
            ./$(PROG).o

.PHONY: limpa faxina clean distclean purge all

%.o: ./%/%.c ./%/%.h
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

