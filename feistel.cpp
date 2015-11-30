#include <stdio.h>
#include <gmp.h>
#include <stdlib.h>
#include <string.h>

void testExponentiation(){
	mpz_t g,x,m,c;		/* working numbers */
	
	const int EXPANSION = 512;
	int hc_bits [EXPANSION];

	//Known safe primes... from http://tools.ietf.org/html/rfc3526#section-2
	const char* safe2048bit = "FFFFFFFF FFFFFFFF C90FDAA2 2168C234 C4C6628B 80DC1CD1 29024E08 8A67CC74 020BBEA6 3B139B22 514A0879 8E3404DD EF9519B3 CD3A431B 302B0A6D F25F1437 4FE1356D 6D51C245 E485B576 625E7EC6 F44C42E9 A637ED6B 0BFF5CB6 F406B7ED EE386BFB 5A899FA5 AE9F2411 7C4B1FE6 49286651 ECE45B3D C2007CB8 A163BF05 98DA4836 1C55D39A 69163FA8 FD24CF5F 83655D23 DCA3AD96 1C62F356 208552BB 9ED52907 7096966D 670C354E 4ABC9804 F1746C08 CA18217C 32905E46 2E36CE3B E39E772C 180E8603 9B2783A2 EC07A28F B5C55DF0 6F4C52C9 DE2BCBF6 95581718 3995497C EA956AE5 15D22618 98FA0510 15728E5A 8AACAA68 FFFFFFFF FFFFFFFF";
	const char* safe1536bit = "FFFFFFFF FFFFFFFF C90FDAA2 2168C234 C4C6628B 80DC1CD1       29024E08 8A67CC74 020BBEA6 3B139B22 514A0879 8E3404DD       EF9519B3 CD3A431B 302B0A6D F25F1437 4FE1356D 6D51C245       E485B576 625E7EC6 F44C42E9 A637ED6B 0BFF5CB6 F406B7ED       EE386BFB 5A899FA5 AE9F2411 7C4B1FE6 49286651 ECE45B3D       C2007CB8 A163BF05 98DA4836 1C55D39A 69163FA8 FD24CF5F       83655D23 DCA3AD96 1C62F356 208552BB 9ED52907 7096966D       670C354E 4ABC9804 F1746C08 CA237327 FFFFFFFF FFFFFFFF";
	
	
	const char* safe512bit =  "CF46878617AB98C7C468DE3A2364ECA8B7C1485D9B9FE201D80148449E89032AD7D1EAC6CFA1BCA91A6754471291C7DE43404DE743E4C218F3BEA2613A5AEB7B";
	
	mpz_init_set_str(g, "2", 10);	/* Assume decimal integers */
	mpz_init_set_str(x, "13407807929942597099574024998205846127479365820592393377723561443721764030073546976801874298166903427690031858186486050853753882811946569946433649006084094", 10);	/* Assume decimal integers */

	//512 bit safe prime (too small for much use really) - this is a terrible practice.
	mpz_init_set_str(m, safe512bit, 16);	/* Assume hex integers */
	
	gmp_printf ("Generator:\n%Zd\n\n", g);
	gmp_printf ("Exponent :\n%Zd\n\n", x);
	gmp_printf ("Modulus  :\n%Zd\n\n", m);
	
	mpz_init(c);
	
	mpz_t half_point;
	mpz_init(half_point);
	
	mpz_t two;
	mpz_init_set_str(two, "2", 10);	/* Assume decimal integers */
	
	mpz_sub_ui (half_point, m, 1); //Subtract 1 from modulus
	mpz_divexact (half_point, half_point, two); //Divide in half exactly (g is 2)
	
	gmp_printf ("Halfpoint: %Zd\n\n", half_point);
	
	for(int i = 0; i<EXPANSION; i++){
		
		//hp,g(y) = 1 if y < (p-1)/2, 0 if y >= (p-1)/2, where y = fp,g(x)
		int comparison = mpz_cmp(x, half_point);
		if (comparison < 0){
			hc_bits[i] = 0;
		} else {
			hc_bits[i] = 1;
		}
		
		
		mpz_powm (x, g, x, m);
		//~ gmp_printf ("%Zd\n", x);
	}
	
	gmp_printf ("%Zd\n", x);
}

char * getPrimeFromFile(const char * safePrimeFile);
char * getSeedFromFile (const char * randomSeedFile);


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
	
	free (primeString);
	free (seedString);
	
	printf("All processing complete\n");
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
