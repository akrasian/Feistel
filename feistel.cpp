#include <gmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

char * getPrimeFromFile(const char * safePrimeFile);
char * getSeedFromFile (const char * randomSeedFile);
void generate (const mpz_t primeString, const mpz_t seedString, mpz_t output, int rightValue);
void psfunc(mpz_t prime, mpz_t seed, mpz_t output, mpz_t key);

size_t primeLen = 0;
const int rounds = 3;

int main(int argc, const char *argv[]){
	printf("Number of arguments: %d\n", argc-1);
	if (argc < 4){
		printf("Program expects three arguments, <file containing prime group> <file containing random seed> <file for output>\n");
		printf("Prime and random input should both be in hex, prime file is generated by ./safePrimeGen");
		printf("Example: ./feistel prime.txt random.txt out.txt\n");
		printf("Quitting\n\n");
		exit(1);
	}
	
	const char * safePrimeFile = argv[1];
	const char * randomSeedFile = argv[2];
	const char * outputFile = argv[3];
	
	printf("File for primes: %s\n", safePrimeFile);
	printf("File for random input: %s\n", randomSeedFile);
	printf("File for output: %s\n", outputFile);
	
	
	char * primeString = getPrimeFromFile(safePrimeFile);
	char * seedString = getSeedFromFile(randomSeedFile);
	
	printf("primeString: [%s]\n", primeString);
	printf("seedString: [%s]\n", seedString);
	mpz_t output, key; //THIS KEY NEEDS TO BE GOTTEN FROM A KEY SCHEDULE


	mpz_init(output);

	mpz_init_set_str(key, "2a190485720987748910038470198347", 16); 	//Using random value for now not sure what we really use to decide left or right branch
																	//A key schedule will need to be created 
	mpz_t prime, seed;
	mpz_init_set_str(prime, primeString, 16);
	mpz_init_set_str(seed, seedString, 16);
	
	//Feistel Network implementation 
	//The seed for the left and right parts of the feistel network is psfuncs L and R output

	//Initialize stuff
	mpz_t l[rounds+1], r[rounds+1], funcOut[rounds+1];
	for(int i = 0; i <= rounds; i++){
		mpz_init(l[i]);
		mpz_init(r[i]);
		mpz_init(funcOut[i]);
	}

	generate(prime, seed, l[0], 0);
	generate(prime, seed, r[0], 1);
	//Feistel rounds v2
	for(int i = 1; i <= rounds; i++){
		mpz_init_set(l[i], r[i-1]);
		psfunc(prime, r[i-1], funcOut[i], key);
		mpz_xor(r[i], l[i-1], funcOut[i]);
	}

	//Concat these two numbers for the new psudorandom number
	gmp_printf("L3: %Zx\n", l[3]);
	gmp_printf("R3: %Zx\n", r[3]);

	
	free (primeString);
	free (seedString);
	gmp_printf ("Final output value: %Zx\n", output);
	printf("All processing complete\n");
}

void psfunc(mpz_t prime, mpz_t seed, mpz_t output, mpz_t key){

	clock_t start = clock();
	printf ("Prime Len %lu\n", primeLen);
	//Timing how long it takes to run a PRF on this implementation
	for(int i = 0; i<primeLen; i++){

		if(mpz_tstbit(key, 127-i)){
			//printf("1");
			generate(prime, seed, output, 1);
			//gmp_printf ("Output1 sub %d: %Zx\n", i, output);
		} else {
			//printf("0");
			generate(prime, seed, output, 0);
			//gmp_printf ("Output0 sub %d: %Zx\n", i, output);
		}

		
		//For the next output of the PRF, would choose one of output1, output2
		//arbitrarily take output1 for test
		mpz_init_set(seed, output);
	}
	printf("\n");
	
	printf("Time taken %ld milliseconds\n", (clock() - start) * 1000 / CLOCKS_PER_SEC);
}
//				prime 			seed 			output 		1 for right 0 for left
void generate (const mpz_t m, const mpz_t x, mpz_t output, int rightValue){
	//~ mpz_t g,x,m,c;		/* working numbers */
	
	//Don't overwrite x, 
	mpz_init_set(output, x);
	
	mpz_t g; //In safe prime groups, g can be the generator.
	mpz_init_set_str(g, "2", 10);
	
	const size_t EXPANSION = primeLen;
	char hc_bits [EXPANSION +1]; //Store as an array of 0 and 1 chars at first.
	
	//~ gmp_printf ("Generator:\n%Zx\n\n", g);
	//~ gmp_printf ("Exponent :\n%Zx\n\n", x);
	//~ gmp_printf ("Modulus  :\n%Zx\n\n", m);
	
	//Calculate halfway point for hardcore bits...
	mpz_t half_point;
	mpz_init(half_point);
	mpz_t two;
	mpz_init_set_str(two, "2", 10);
	mpz_sub_ui (half_point, m, 1); //Subtract 1 from modulus
	mpz_divexact (half_point, half_point, two); //Divide in half exactly
	//~ gmp_printf ("Halfpoint: %Zx\n\n", half_point);
	
	for(int i = 0; i< EXPANSION; ++i){
		//hardcore(y) = 1 if y < (p-1)/2, 0 if y >= (p-1)/2, where y = f<p,g>(x)
		int comparison = mpz_cmp(output, half_point);
		
		if (comparison < 0){
			hc_bits[i] = '0';
		} else {
			hc_bits[i] = '1';
		}
		
		mpz_powm (output, g, output, m);
	}
	
	hc_bits[EXPANSION] = '\0';
	//~ printf("O2 = [%s]\n", hc_bits);
	
	if (rightValue){
		mpz_init_set_str(output, hc_bits, 2);
	}
}

char * getPrimeFromFile (const char * safePrimeFile){
	printf("Reading %s\n",safePrimeFile);
	FILE * in = fopen(safePrimeFile, "r");

	if (in == NULL) {
		fprintf(stderr, "Can't open primes file %s, quitting.\n", safePrimeFile);
		exit(1);
	}
	
	char * primeLengthLine = NULL;
	size_t primeLength;
	
	size_t maxBytes = 64;
	if (getline (&primeLengthLine, &maxBytes, in) != -1){
		
		unsigned int uiPrimeLen;
		
		if (sscanf(primeLengthLine, "%d", &uiPrimeLen) ){
			primeLength = (size_t)uiPrimeLen;
			
			primeLen = primeLength;
		} else {
			printf("No num on line\n");
			exit (1);
		}
		
	} else {
		printf ("invalid file");
		exit (1);
	}
	
	char * primeHex = NULL;
	
	ssize_t lineLen = 0;
	if (lineLen = getline (&primeHex, &primeLength, in) != -1) {
		size_t len = strlen(primeHex) -1;
		if (primeHex[len] == '\n'){
			primeHex [len] = '\0';  //strip the newline.
		}
	} else {
		printf ("invalid file");
		exit(1);
	}
	
	fclose(in);
	
	return primeHex;
}

char * getSeedFromFile (const char * randomSeedFile){
	printf("Reading %s\n",randomSeedFile);
	FILE * in = fopen(randomSeedFile, "r");

	if (in == NULL) {
		fprintf(stderr, "Can't open seed file %s, quitting.\n", randomSeedFile);
		exit(1);
	}
	
	char * seed = NULL;
	
	ssize_t lineLen = 0;
	long unsigned int maxBytes= 0;
	
	if (lineLen = getline (&seed, &maxBytes, in) != -1) {
		size_t len = strlen(seed) -1;
		if (seed[len] == '\n'){
			seed [len] = '\0';  //strip the newline.
		}
	} else {
		printf ("invalid seed file");
		exit(1);
	}
	
	fclose(in);
	
	return seed;
}
