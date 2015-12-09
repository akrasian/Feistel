all: safePrimeGen feistel

#~ run: generate
run: runFeistel
	
runFeistel: feistel
#~ 	./feistel safe128.txt random128.txt temp/plaintext17.txt temp/output17.txt
	./feistel safe128.txt random128.txt temp/plaintext22.txt temp/output22.txt
	./feistel safe128.txt random128.txt temp/plaintext23.txt temp/output23.txt
	./feistel safe128.txt random128.txt temp/plaintext24.txt temp/output24.txt

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
