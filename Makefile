all: safePrimeGen feistel

run: feistel
	./feistel safe128.txt random.txt plaintext.txt out.txt

generate: safePrimeGen
	./safePrimeGen 128 safe128.txt

feistel: feistel.cpp
	g++ -O3 -o feistel feistel.cpp -lgmp -lm

safePrimeGen: safePrimeGen.cpp
	g++ -O3 -o safePrimeGen safePrimeGen.cpp -lgmp -lm

clean:
	rm -f safePrimeGen
	rm -f feistel
	rm -f *.o
