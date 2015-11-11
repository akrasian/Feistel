#include <ttmath/ttmath.h>
#include <iostream>

using namespace std;

//Number of words to use per hugeInt - this directly effects performance, but if you use too small a size for the arrays, this will fail horribly.- assuming 64 bit computers 16 is fine for 512 bit numbers.
//Using 16 word hugeInts and 512 bit keys, I can do 10K modular exponents per second.
typedef ttmath::UInt < 16 > hugeInt;

// result = (base ^ exponent) % modulus
template < typename Integer >
    int modExp(const Integer & base, const Integer & exponent,
	     const Integer & modulus, Integer & result) {
	int c = 0;
	Integer rem, b, e;

	if ((base == 0 && exponent == 0) || modulus == 0)
		return 1;

	result = 1;
	b = base;
	e = exponent;

	while (e > 0) {
		if (e.IsTheLowestBitSet()) {
			c += result.Mul(b);
			c += result.Div(modulus, &rem);
			result = rem;
		}

		e.Rcr(1);

		c += b.Mul(b);
		c += b.Div(modulus, &rem);
		b = rem;
	}

	return (c > 0) ? 1 : 0;
}

int main() {
	//An example of performing modular exponentiation efficiently on huge numbers.
	hugeInt p, x, g,y;
	
	hugeInt base = 2;
	hugeInt exp = 512;
	base.Pow(exp);
	
	cout << "value of 2^512 is \n" << base << endl;
	
	g = base -3;
	x = base -2;
	p = base -1;
	y = "1";

	modExp(g, x, p, y);
	cout << "Value of (2^512 -3) ^ (2^512 -2) mod (2^512 -1) is \n" << y << endl;
}
