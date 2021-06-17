#include "binary_number.h"
#include<string>

binary_number::binary_number()
{
	num = 0;
	dashes = 0;
	is_used = false;
	covered_mins.resize(0);
}

binary_number::binary_number(int dec)
{
	num = dec;
	dashes = 0;
	is_used = false;
	covered_mins.resize(0);
	covered_mins.push_back(dec);
}

int binary_number::count_ones(int n)
{
	short bit = 0;
	int count = 0;
	while (n > 0)
	{
		bit = n % 2;
		n >>= 1;
		if (bit) {
			count++;
		}
	}
	return count;
}
void binary_number::print_number(int Bits_num)
{
	int n = num;
	vector<unsigned> bits = { 0 };
	bits.resize(Bits_num);
	int count = 0;
	while (n > 0 || count < Bits_num)
	{
		bits[count] = n % 2;
		n >>= 1;
		count++;
	}
	for (int i = count - 1; i >= 0; i--)
		cout << bits[i];
}

void binary_number::print_with_dashes(int Bits_num)
{
	int n = num;
	int d = dashes;
	vector<unsigned> bits = { 0 };
	bits.resize(Bits_num);
	int count = 0;
	while (n > 0 || count < Bits_num) {
		if (!(d % 2))
			bits[count] = n % 2;
		else
			bits[count] = 2;
		n >>= 1;
		d >>= 1;
		count++;
	}
	for (int i = count - 1; i >= 0; i--) {
		if (bits[i] != 2)
			cout << bits[i];
		else
			cout << "-";
	}
}

binary_number::~binary_number()
{
}
