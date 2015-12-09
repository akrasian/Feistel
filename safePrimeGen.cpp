#include <stdio.h>
#include <gmp.h>
#include <stdlib.h>

int getStrongPrime(long bits, mpz_t answer);

int main(int argc, const char *argv[]) {
	printf("Number of arguments: %d\n", argc-1);
	if (argc < 3){
		printf("Program expects two arguments, <minimum bits in strong prime> <file for output prime>\n");
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
	
	getStrongPrime(length, sprime);
	
	//mpz_get_str allocates a string of the right size
	char * result = mpz_get_str(NULL, 16, sprime);
	
	gmp_printf ("Strong Prime: 0x%s\n", result);
	
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


int getStrongPrime(long bits, mpz_t answer){
	mpz_t two;
	mpz_init_set_str(two, "2", 10);	//constant 2 in decimal.
		
	mpz_t lower_bound;
	mpz_init(lower_bound);
	
	mpz_pow_ui (lower_bound, two, bits);
	
	gmp_printf ("Lower bound for strong prime generation: 0x%Zx\n", lower_bound);
	printf("Generating a strong prime above lower bound, this might take a while\n\n");
	
	mpz_set(answer, lower_bound);
	
	int strongPrimeFound = 0;
	int rejectedPrimes = 0;
	
	mpz_t temp;
	mpz_init(temp);

	while(strongPrimeFound == 0){
		mpz_nextprime (answer, answer);
		
		if (++rejectedPrimes % 2000 == 0){
			printf ("Rejected %d primes...\n", rejectedPrimes);
		}
		
		mpz_sub_ui (temp, answer, 1); //Subtract 1 from answer
		mpz_divexact (temp, temp, two); //Divide in half exactly
		
		int prime_test = mpz_probab_prime_p(temp, 20);
		if (prime_test){
			
			//Test for R
			mpz_sub_ui (temp, temp, 1); //Subtract 1 from temporary prime
			mpz_divexact (temp, temp, two); //Divide temp in half exactly
			
			prime_test = mpz_probab_prime_p(temp, 20);
			if(prime_test){
				printf("Rejected a total of %d primes before finding a strong prime\n", rejectedPrimes);
				printf("(p-1)/2 == q, (q-1)/2 == r, where q and r are both prime.\n");
				
				strongPrimeFound = 1;
			}
		}
	}
	
	return 0;
}
