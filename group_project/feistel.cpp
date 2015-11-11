#include <ttmath/ttmath.h>
#include <iostream>

using namespace std;

typedef ttmath::UInt < 100 > hugeInt;

// result = (base ^ exponent) mod modulus
template < typename Integer >
    int powm(const Integer & base, const Integer & exponent,
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
	
	cout << base << endl;
	
	p = base -1;
	x = base -2;
	g = base -3;
	y = "1";
	
	cout << p << endl;
	cout << x << endl;
	cout << g << endl;
	cout << y << endl;

	for(int i = 0; i<100; i++){
		powm(g, x, p, y);
		cout << y << endl;
	}
}
