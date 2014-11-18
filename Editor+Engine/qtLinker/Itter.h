#include <iostream>
#include <complex>
#include <cmath>
#include <iterator>
using namespace std;

unsigned int bitReverse(unsigned int x, int log2n)

{
	int n = 0;
	int mask = 0x1;
	for (int i = 0; i < log2n; i++)

	{
		n <<= 1;
		n |= (x & 1);
		x >>= 1;
	}
	return n;
}
const double PI = 3.1415926536;
template<class Iter_T>
extern void fft(Iter_T a, Iter_T b, int log2n)
{
	std::complex<double> J(0,1);
	

	int n = 1 << log2n;
	for (unsigned int i = 0; i < n; ++i) {
		b[bitReverse(i, log2n)] = a[i];
	}

	for (int s = 1; s <= log2n; ++s)

	{
		int m = 1 << s;
		int m2 = m >> 1;
		std::complex<double> w(1, 0);
		std::complex<double> wm = std::exp(-J * (PI / m2));
		for (int j = 0; j < m2; ++j)

		{
			for (int k = j; k < n; k += m)

			{
				std::complex<double> t = w * b[k + m2];
				std::complex<double> u = b[k];
				b[k] = u + t;
				b[k + m2] = u - t;
			}
			w *= wm;
		}
	}
}
