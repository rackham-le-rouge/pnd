#
# \file                 Makefile
# \author               Jerome GRARD
# \version              1.0
# \date                 14/02/14
# \brief                Makefile for pnd
#
# \details              Cleaning and compilation with the 70 options of the libcommon (http://www.thelibcommonproject.org)
#			You can't add optimisation option in CFLAGS : -03 and -march=native because there is a restart issue
#			if you want to change speed during computing. If you wont, you can use these options.



#ignored option
# -Wc++-compat		: because don't stand the private keyword of the openMP call.
# -Wpadded		: need to fix it. There is a problem with the structure, memory is not optimized

CC=gcc
CFLAGS=-fopenmp -W -Waggregate-return -Wall -Warray-bounds -Wbad-function-cast -Wcast-align -Wcast-qual 	\
                 -Wchar-subscripts -Wcomment -Wdeclaration-after-statement -Werror -Wextra -Wfloat-equal        \
                 -Wformat -Wformat-nonliteral -Wformat-security -Wformat-y2k -Wignored-qualifiers -Wimplicit    \
                 -Wimplicit-function-declaration -Wimplicit-int -Winit-self -Winline -Winvalid-pch -Wlogical-op \
                 -Wlong-long -Wmain -Wmissing-braces -Wmissing-declarations -Wmissing-field-initializers        \
                 -Wmissing-include-dirs -Wmissing-noreturn -Wmissing-parameter-type -Wmissing-prototypes        \
                 -Wmultichar -Wnested-externs -Wno-multichar -Wnonnull -Wold-style-definition                   \
                 -Woverlength-strings -Wparentheses -Wpointer-arith -Wpointer-sign                     		\
                 -Wreturn-type -Wsequence-point -Wshadow -Wsign-compare -Wstack-protector                       \
                 -Wswitch -Wswitch-default -Wswitch-enum -Wsync-nand -Wtrigraphs -Wundef -Wuninitialized        \
                 -Wunknown-pragmas -Wunsafe-loop-optimizations -Wunused -Wunused-function -Wunused-label        \
                 -Wunused-parameter -Wunused-value -Wunused-variable -Wvla -Wvolatile-register-var              \
                 -Wwrite-strings -fno-common -fstack-protector-all -pedantic -std=c99 -Wstrict-aliasing=3	

LDFLAGS=-lncurses -ansi -lgmp -lm -fopenmp
EXEC=pnd

all: $(EXEC)
allc: mrproper $(EXEC)
full: $(EXEC) documentation package
installation: $(EXEC) documentation


pnd: drawing.o compute.o multithread.o signalmanagement.o main.o
	@$(CC) -o $@ $^ $(LDFLAGS)
	@echo [LD] $@

documentation:
	@echo [MAN] Man is ok, use ./man pnd.man to see it
	@echo [DOC] Doxygen generation...
	@doxygen conf.doxygen

package: mrproper $(EXEC)
        @echo [DEB] Create directories...
        @mkdir ./pndPKG/usr/src/pnd
	@echo [DEB] Copying files...
	@cp *.c ./pndPKG/usr/src/pnd/
	@cp *.h ./pndPKG/usr/src/pnd/
	@cp Makefile conf.doxygen pnd.man TODO ./pndPKG/usr/src/pnd/
	@dpkg-deb --build pndPKG



%.o: %.c
	@$(CC) -o $@ -c $< $(CFLAGS)
	@echo [CC] $@



.PHONY: clean mrproper documentation package

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

