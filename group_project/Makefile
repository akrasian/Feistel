CC     = g++

#CFLAGS = -Wall -pedantic -s -O2 -I.. -DTTMATH_DONT_USE_WCHAR
#ttmath gives a ton of warnings for unused vars, will have to check them out later
CFLAGS = -s -O3 -I.. -DTTMATH_DONT_USE_WCHAR

.SUFFIXES: .cpp .o

.cpp.o:
	$(CC) -c $(CFLAGS) $<

run: feistel
	./feistel

all: feistel

feistel.o:	feistel.cpp


clean:
	rm -f *.o
	rm -f *.s
	rm -f feistel
# on MS Windows can automatically be added suffixes .exe to the names of output programs
	rm -f *.exe
