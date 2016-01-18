#include "include.h"

// Is the character c somewhere within the string l?
int isIn(const char *l, char c) { while(*l) { if(*l == c) return 1; l++; } return 0; }

//FQP evalNumber(const char **src)
//{
//	const char *digits = (const char *) "fedcba9876543210";
//	const char *line = *src;
//
//	long double val = 0.0;
//	int base = 10, off = 1;
//	int dec = 0, f = 0, v;
//
//	// Read next character
//	char c = *line++;
//
//	// If the first character is a zero ...
//	// (Indicates either a hexadecimal number '0x...',
//	//  an binary number '0b...' or an octal number '0...'
//	if(c == '0')
//	{
//		c = *line++;
//
//		// A single zero is just zero.
//		if(c == '\0')
//		{
//			*src = line - 1;
//
//			return (FQP) 0;
//		}
//		// A '0' followed by an 'x' -> Hexadecimal
//		else if(c == 'x')
//		{
//			c = *line++;
//			base = 16;
//		}
//		// A '0' followed by a 'b' -> Binary
//		else if(c == 'b')
//		{
//			c = *line++;
//			base = 2;
//		}
//		// A '0' followed by a '.' -> Not an octal number 
//		else if(c != '.')
//		{
//			base = 8;
//			val = 0.0L;
//			f = 1;
//		}
//		// Decimal number, set flag for reading the
//		// fractional part.
//		else
//		{
//			c = *line++;
//			dec = 1;
//		}
//	}
//
//	// While c is a valid character for the specified base
//	while(c && ((dec == 0 && c == '.') || isIn(digits + 16 - base, c)))
//	{
//		// In case of a dot, set flag for the fractional part of the number
//		if(c == '.')
//		{
//			dec = 1;
//			c = *line++;
//
//			// Reset 'successfull read'-flag, since a number
//			// may not end with a dot
//			f = 0;
//
//			continue;
//		}
//
//		// Number representation -> Number
//		v = c;
//		if(c >= '0' && c <= '9') v -= '0';
//		if(c >= 'a' && c <= 'f') v -= 'a' - 10;
//
//		// With hexadecimal as largest numeral system,
//		// no single digit can be larger than 15(10)
//		assert(v<16);
//
//		// If we are reading the fractional part of the number
//		if(dec)
//		{
//			// Shift offset left one digit
//			off *= base;
//			// Add the number (shifted right by offset) to the result
//			val += (long double)v / off;
//		}
//		else
//		{
//			// Shift result left one digit
//			val *= base;
//			// Add the number to the result
//			val += v;
//		}
//
//		// Set flag indication a successfull read
//		f = 1;
//		// Read next character
//		c = *line++;
//	}
//
//	// Assure at least one digit has been read
//	assert(f);
//
//	// Update string pointer
//	*src = line - 1;
//
//	// Cast to desired number format and return result
//	return (FQP) val;
//}

// Prints a number in base 'b' with at most 'MAX_DIGITS'
// digits after the decimal point (if necessary)
void printNumber(FQP n, int b, void (*pf)(char))
{
	// All possible digits; allows bases up to 34
	const char *digits = (const char *) "0123456789abcdefghijklmnopqrstuvwxyz";
	char buf[1024];
	int i, j, k, t;
	long long p;

	if(n < 0)
	{
		pf('-');
		n = -n;
	}

	// Adjust incorrect bases; default to 10/decimal
	if(b <= 0 || b >= strlen(digits)) b = 10;

	// Extract digit closest to the decimal point until
	// only zero is left (left of the decimal point)
	i = 0;
	do
	{
		p = (long long) n; // Cast to long long to get rid of the fractional part of the decimal
		t = p % b; // Get first digit left of the decimal point
		n = (n - p) + (FQP) (p / b); // Shift p right one digit in base 'b', add fractional part
		buf[i++] = digits[t]; // Store char representing the extracted digit
	}
	while(n >= 1);

	// Print the extracted digits in reverse order
	// (left to right)
	while(i > 0)
	{
		pf(buf[--i]);
	}

	// If the is a fractional part left
	if(n > 0)
	{
		// Print decimal point
		pf('.');

		// Print 'MAX_DIGITS' digits at most
		for(i = 0, j = 0 ; i < *MAX_DIGITS() ; i++)
		{
			n *= b; // Shift number left one digit in base 'b'
			t = (int) n; // Discard fractional part to extract the former first digit to the right of the decimal point
			n -= t; // Discard integer part of the number

			buf[j++] = digits[t]; // Buffer char representing extracted digit
			
			// If current digit is not zero, flush buffer
			if(t > 0)
			{
				for(k = 0 ; k < j ; k++) pf(buf[k]);
				j = 0;
			}
		}
	}
}

int *MAX_DIGITS(void)
{
	static int v = 8;
	return &v;
}

