#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define BUF_SIZE 1024
#define PI 3.1415926535897932384626433832795028841971693993751058
#define E  2.7182818284590452353602874713526624977572470936999595
#define MIN_STEP 1E-15L

#ifndef NAN
#define NAN ((double)(-1l))
#endif

#define ABS(x) ((x)<0?-(x):(x))

double sin_IE1(double);
double cos_IE1(double);
double tan_IE1(double);
double exp_IE1(double);
double sqrt_IE1(double);
double ln_IE1(double);
double pow_IE1(double, double);

typedef double (*m_func)(double);
enum
{
	IE_SIN,
	IE_COS,
	IE_TAN,
	IE_EXP,
	IE_SQRT,
	IE_LN,
	M_SIN,
	M_COS,
	M_TAN,
	M_EXP,
	M_SQRT,
	M_LN,
	MATH_LAST
};

double relative_error(double, double);
double P_PI(void);
void print_ej(double, double, double);

int main(int argc, char *argv[])
{
	int i;
	double x, v[MATH_LAST];
	char buf[BUF_SIZE];
	m_func funcs[MATH_LAST] = { sin_IE1, cos_IE1, tan_IE1, exp_IE1, sqrt_IE1, ln_IE1,
								sin,     cos,     tan,     exp,     sqrt,     log};

	while(1)
	{	
		printf("Enter double: ");

		fflush(stdin);

		if(!gets(buf))
		{
			fprintf(stderr, "ERR: Couldn't read from stdin.\nAbort.\n");
			return EXIT_FAILURE;
		}

		putchar('\n');

		if(strcmp(buf, "quit") == 0 || (buf[0] == 'q' && buf[1] == '\0'))
		{
			break;
		}

		if(!sscanf(buf, "%lf", &x))
		{
			fprintf(stderr, "ERR: Invalid input.\n'%s' is not a valid decimal number.\n\n", buf);
			continue;
		}

		if(argc >= 2 && strcmp(argv[1], "-d") == 0)
		{
			x = (double) (x * P_PI() / 180.0L);
		}

		printf("! x == %.10lf\n", x);

		for(i = 0 ; i < MATH_LAST ; i++)
		{
			v[i] = funcs[i](x);
		}

#define RE(i) relative_error(v[i],v[(i)+MATH_LAST/2])
		printf("# =============================================================================================================\n");
		printf("         | %14s %14s %16s %16s %16s %16s\n", "sin", "cos", "tan", "exp", "sqrt", "ln");
		printf("---------+-----------------------------------------------------------------------------------------------------\n");
		printf("%8s | %14.10lf %14.10lf %16.10lf %16.10lf %16.10lf %16.10lf\n", 
				"IE1",       v[IE_SIN],  v[IE_COS],  v[IE_TAN],  v[IE_EXP],  v[IE_SQRT],  v[IE_LN]);
		printf("%8s | %14.10lf %14.10lf %16.10lf %16.10lf %16.10lf %16.10lf\n", 
				"math",      v[ M_SIN],  v[ M_COS],  v[ M_TAN],  v[ M_EXP],  v[ M_SQRT],  v[ M_LN]);
		printf("%8s | %14.10lf %14.10lf %16.10lf %16.10lf %16.10lf %16.10lf\n", 
				"rel. err", RE(IE_SIN), RE(IE_COS), RE(IE_TAN), RE(IE_EXP), RE(IE_SQRT), RE(IE_LN));
#undef RE

		printf("         |   ");
		
		print_ej(x, v[IE_COS], v[IE_SIN]);

		putchar('\n');
	}

	struct timeval start, ie1, syst;
	gettimeofday(&start, NULL);

	for(i = 1 ; i < 100000 ; i++)
	{
//		sin_IE1(i * P_PI() / 180.0);
		tan_IE1(i);
	}

	gettimeofday(&ie1, NULL);

	for(i = 1 ; i < 100000 ; i++)
	{
//		sin(i * P_PI() / 180.0);
		tan(i);
	}

	gettimeofday(&syst, NULL);

	printf("Time of sin for ie1: %lu/10**9s, sys: %lu/10**9s\n", ie1.tv_usec - start.tv_usec, syst.tv_usec - ie1.tv_usec);

	return EXIT_SUCCESS;
}

void print_ej(double x, double re, double im)
{
	printf("e ** ");

	if(x == 0.0)
	{
		printf("0 == 1");
	}
	else
	{
		printf("%cj", x < 0 ? '-' : '\0');
		
		if(ABS(x) != 1.0)
		{
			printf("%.3lf", ABS(x));
		}

		printf(" == ");
		
		if(re != 0.0)
		{
			printf("%.3lf ", re);
		}

		if(im != 0.0)
		{
			printf("%c%cj", im < 0.0 ? '-' : (re != 0.0 ? '+' : '\0'), re != 0.0 ? ' ' : '\0');

			if(ABS(im) != 1.0) printf("%.3lf", ABS(im));
		}

		if(re == 0.0 && im == 0.0)
		{
			printf("0");
		}
	}

	putchar('\n');
}

double relative_error(double v1, double v2)
{
	if(ABS(ABS(v1)-ABS(v2)) < MIN_STEP) return 0.0;
	if(ABS(v2) < MIN_STEP) return NAN;

	return ABS(v1 - v2) / v2;
}

// # ==========================================================================

double exp_IE1(double x)
{
	int i, j, inv;
	long double e = 1.0L, tmp;

	if(x == 0.0) return 1.0;

	// If x < 0, then 1 / e**|x| will be calculated
	inv = x < 0 ? 1 : 0;
	x = ABS(x);

	i = 1;
	do
	{
		tmp = 1.0L;

		for(j = 1 ; j <= i ; j++)
		{
			tmp *= x / (long double) j;
		}

		e += tmp;

		i++;
	} while(tmp > MIN_STEP);

	if(inv) { e = 1.0L / e; }

//	printf("#[%d turns!]\n", i);

	return (double) e;
}

double tan_IE1(double x) { return sin_IE1(x) / sin_IE1(x + 0.5 * P_PI()); }
double cos_IE1(double x) { return sin_IE1(x + 0.5 * P_PI()); }
double sin_IE1(double x) 
{
	int i, j;
	long double sin = 0, tmp;

	// Shifting until x is an element of the
	// interval [-PI,PI)
	while(x > P_PI())  x -= 2 * P_PI();
	while(x < -P_PI()) x += 2 * P_PI();

	if(ABS(x) < MIN_STEP || ABS(ABS(x) - P_PI()) < 1E-8L) return 0.0;

	// With x=max(|[-PI,PI)|) the summands are larger than 1E-9
	// for the first 21 (= 2 * 10 + 1) iterations.
	i = 0;
	do
	{
		// Tyler series.

		tmp = (i & 1) ? -1.0L : 1.0L;

		for(j = 1 ; j <= 2 * i + 1 ; j++)
		{
			tmp *= x / (long double) j;
		}

		sin += tmp;

		i++;
	} while(ABS(tmp) > MIN_STEP);

//	printf("![%d turns!]\n", i);

	return (double) sin;
}

double sqrt_IE1(double x)
{
	long double t, s = x;

	if(x < 0.0) return NAN;
	if(x == 0.0) return 0.0;

	// Heron's method (AKA babylonian method.)
	do
	{
		t = s;
		s = (t + x / t) * 0.5;
	} while(ABS(t - s) >= MIN_STEP);

	return (double) s;
}

long double int_pow(long double x, int n) { return n == 0 ? 1.0L : (n < 0 ? 1.0L / int_pow(x, -n) : x * int_pow(x, n - 1)); }

double ln_IE1(double x)
{
	long double v = 0.0L, s, y;
	int i = 1;

	if(x <= 0.0) return NAN;
	if(x == 1.0) return 0.0;

	if(x < 1.0)
	{ // Tylor series
		y = x - 1.0;
		
		do
		{
			s = 1.0L / i;
			s = (i & 1) ? s : -s;

			s *= int_pow(y, i);

			v += s;

			i++;

		} while(ABS(s) >= MIN_STEP);
	}
	else
	{ // Euler transformation
		y = x / (long double) (x - 1.0);

		do
		{
			s = 1.0L / y;
			s = int_pow(s, i);
			s /= i;

			i++;

			v += s;
		} while(ABS(s) >= MIN_STEP);
	}

	return (double) v;
}

double pow_IE1(double a, double b)
{
	if(b == 0.0) return 1.0;
	if(a == 0.0) return 0.0;
	if(a < 0.0) return NAN;

	return exp_IE1(b * ln_IE1(a));
}

double P_PI(void)
{
	static double pi = NAN;
	
	if(pi != pi)
	{
		pi = 4.0L * atan(1.0);
	}

	return pi;
}

