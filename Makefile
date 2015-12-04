all: safePrimeGen feistel

run: feistel
#~ 	./feistel safe512.txt random.txt out.txt
	./feistel safe128.txt random.txt out.txt

generate: safePrimeGen
#~ 	./safePrimeGen 512 safe512.txt
	./safePrimeGen 128 safe128.txt

feistel: feistel.cpp
	g++ -O3 -o feistel feistel.cpp -lgmp -lm

safePrimeGen: safePrimeGen.cpp
	g++ -O3 -o safePrimeGen safePrimeGen.cpp -lgmp -lm

clean:
	rm -f safePrimeGen
	rm -f feistel
	rm -f *.o
