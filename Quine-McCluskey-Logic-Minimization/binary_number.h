#pragma once
#include<vector>
#include<iostream>
#include<string>
using namespace std;
class binary_number
{

private:

public:
	unsigned num;   //the binary number 
	unsigned dashes;  // represents the locations of the dashes by ones
	bool is_used;    //flag that indicates whether it is used in combinations
	vector<unsigned> covered_mins; //every number has a vector that includes all the minterms it covers 
	binary_number(int dec);
	binary_number();
	~binary_number();
	void print_with_dashes(int Bits_num); // prints the binary number to Bits_num decimal places with dashes
	int count_ones(int n);  // counts the number of ones in the binary number
	void print_number(int Bits_num); // prints the binary number to Bits_num decimal places

};

