/* crack */
/* sip */
#include <stdio.h>
#include <string.h>

#define PI 3.1415
#define WIDTH 80

float power(float base, int exponent)
{
	register float res = base;

	if(!exponent) return 1;

	if(exponent < 0)
		return 1/power(base, -exponent);

	while(--exponent)
		res *= base;

	return res;
}

long factorial(long n)
{
	register long res = n;

	if(!n) return 1;

	while(--n)
		res *= n;

	return res;
}

float taylor_sin(float rad, int prec)
{
	register int   a;
	register float sign, pr;
	register long  fac;
	register float res = 0;

	while(prec--) {
		a    = (2*prec) + 1;
		sign = power(-1, prec);
		pr   = power(rad, a);
		fac  = factorial(a);

		res += (sign*pr)/fac;
	}

	return res;
}

int main(int argc, char **argv)
{
	if(argc<2) return 1;

	const int center   = WIDTH/2;
	const int half_len = strlen(argv[1])/2;
	const int travel   = center - half_len;
	const float step   = PI/20;

	float pvt = 0.0;
	int   off = 0.0;

	for(;;) {
		pvt += step;
		if(pvt > 2*PI)
			pvt -= 2*PI;

		off = travel * taylor_sin(pvt, 9);
		printf("%*s\n",  center+half_len+off, argv[1]);
	}
}
