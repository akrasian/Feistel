#include <stdio.h>
#include <gmp.h>
#include <stdlib.h>

int getSafePrime(long bits, mpz_t answer);

int main(int argc, const char *argv[]) {
	printf("Number of arguments: %d\n", argc-1);
	if (argc < 3){
		printf("Program expects two arguments, <minimum bits in safe prime> <file for output prime>\n");
		printf("Example: ./safePrimeGen 512 primes.txt\n");
		printf("Quitting\n\n");
		exit(1);
	}
	
	printf("File for output: %s\n", argv[2]);
	
	char * end;
	long length = strtol(argv[1], &end, 10);
	printf("Minimum bits in output: %ld\n", length);
	if (length < 64){
		printf("Invalid length %ld, quitting\n", length);
		exit(1);
	}
	
	mpz_t sprime;
	mpz_init(sprime);
	
	getSafePrime(length, sprime);
	
	//mpz_get_str allocates a string of the right size
	char * result = mpz_get_str(NULL, 16, sprime);
	
	gmp_printf ("Safe Prime: 0x%s\n", result);
	
	FILE * out = fopen(argv[2], "w");

	if (out == NULL) {
		fprintf(stderr, "Can't open output file %s, quitting.\n", argv[2]);
		exit(1);
	} else {
		fprintf(out, "%ld\n%s\n", length, result);
	}

	fclose(out);
	
	free(result);
	return 0;
}


int getSafePrime(long bits, mpz_t answer){
	mpz_t two;
	mpz_init_set_str(two, "2", 10);	//constant 2 in decimal.
		
	mpz_t lower_bound;
	mpz_init(lower_bound);
	
	mpz_pow_ui (lower_bound, two, bits);
	
	gmp_printf ("Lower bound for safe prime generation: 0x%Zx\n", lower_bound);
	printf("Generating a safe prime above lower bound, this might take a while\n\n");
	
	int safePrimeFound = 0;
	mpz_nextprime (answer, lower_bound);
	//Put the first possible prime into answer
	
	int rejectedPrimes = 0;
	
	while(safePrimeFound == 0){
		
		if (rejectedPrimes % 100 == 0){
			printf ("Rejected %d primes...\n", rejectedPrimes);
		}
		
		//check if answer is safe:
		//Is (p-1) / 2 also a prime?
		
		mpz_t half_point;
		mpz_init(half_point);
		
		mpz_sub_ui (half_point, answer, 1); //Subtract 1 from answer
		mpz_divexact (half_point, half_point, two); //Divide in half exactly
		
		//reasonable reps between 15 and 50 apparently.
		int prime_test = mpz_probab_prime_p(half_point, 20);
		
		if (prime_test == 2 || prime_test == 1){
			safePrimeFound = 1;
		} else {
			//~ gmp_printf ("This is not a safe prime: %Zd\n", answer);
			//Find next prime AFTER answer.
			rejectedPrimes++;
			mpz_nextprime (answer, answer);
		}
	}
	
	return 0;
}
