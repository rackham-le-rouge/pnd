CC=gcc
CFLAGS=-Wall -fopenmp
LDFLAGS=-lncurses -ansi -lgmp -lm -fopenmp
EXEC=pnd

all: $(EXEC)
allc: mrproper $(EXEC)

pnd: drawing.o compute.o multithread.o main.o
	@$(CC) -o $@ $^ $(LDFLAGS)
	@echo [LD] $@

%.o: %.c
	@$(CC) -o $@ -c $< $(CFLAGS)
	@echo [CC] $@



.PHONY: clean mrproper

clean:
	@echo ---------------
	@echo      clean
	@echo ---------------
	rm -rf *.o

mrproper: clean
	@echo ---------------
	@echo   clean all
	@echo ---------------
	rm -rf $(EXEC)

