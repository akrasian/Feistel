all: safePrimeGen feistel

#~ run: generate
run: runFeistel
	
runFeistel: feistel
	./feistel safe128.txt random128.txt plaintext256.txt out.txt

generate: safePrimeGen
#~ 	./safePrimeGen 128 safe128.txt
#~ 	./safePrimeGen 256 safe256.txt
#~ 	./safePrimeGen 512 safe512.txt
	./safePrimeGen 1024 safe1024.txt

feistel: feistel.cpp
	g++ -g -O3 -o feistel feistel.cpp -lgmp -lm

safePrimeGen: safePrimeGen.cpp
	g++ -g -O3 -o safePrimeGen safePrimeGen.cpp -lgmp -lm

clean:
	rm -f safePrimeGen
	rm -f feistel
	rm -f *.o
