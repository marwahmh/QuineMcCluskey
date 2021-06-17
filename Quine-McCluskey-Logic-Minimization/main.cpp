#include <iostream>
#include <fstream>
#include "binary_number.h"
#include<string>
#include<cmath>
#include<vector>
using namespace std;
void create_MinTable(vector<vector<binary_number>>& A, vector<unsigned>&, int size);
void print_MinTable(vector<vector<binary_number>>& A, int Binsize);
void create_combined(vector<vector<binary_number>>& Initial_table, vector<vector<binary_number>>& mid_table);
void print_combined(vector<vector<binary_number>>& mid_table, vector<binary_number>& printed_numbers);
void create_final_table(vector<vector<binary_number>>& Initial_table, vector<vector<binary_number>>& mid_table, vector<vector<binary_number>>& final_table, vector<binary_number>& printed_numbers);
void print_final_table(vector<vector<binary_number>>& final_table, vector<binary_number>& printed_numbers);
void create_essential_table(vector<binary_number>& essentials, vector<vector<binary_number>>& final_table, vector<unsigned>& minterms);
bool is_printed(binary_number n, vector<binary_number>& printed_numbers);
bool is_dc(int x, vector<unsigned>& dontcares);
bool minterm_exists(binary_number t, int x, int group, int pos, vector<vector<binary_number>>& primes, vector<unsigned>& minterms);
bool is_minterm(int x, vector<unsigned>& minterms);
bool in_essentials(binary_number x, vector<binary_number>essentials);
bool covered_by_essential(int x, vector<binary_number> essentials);
bool out_of_Range(int x);
int read_from_file(ifstream& minterms_file, vector<unsigned>& minterms, vector<unsigned>& dontcares);

int num_of_var; //number of variables
int main()
{
	vector<vector<binary_number>> Initial_table;   //vector of vectors - each vector inside represents a group according to the number of ones
	vector<vector<binary_number>> mid_table;  // vector of vectors - each vector inside represents a group according to the number of ones and this is the mid process table
	vector<binary_number> printed_numbers; // vector that has printed numbers
	vector<vector<binary_number>> final_table;  // vector of vectors - each vector inside represents a group according to the number of ones and this is the final table for prime implicants
	vector<unsigned> minterms, dontcares, inputs;   //vector of minterms and dont cares

	ifstream minterms_file("minterms.txt");

	if (!read_from_file(minterms_file, minterms, dontcares))
		return 0;

	minterms_file.close();

	int total_num = minterms.size() + dontcares.size();

	for (int i = 0; i < minterms.size(); i++)
		inputs.push_back(minterms[i]);
	for (int i = 0; i < dontcares.size(); i++)
		inputs.push_back(dontcares[i]);


	create_MinTable(Initial_table, inputs, total_num);

	cout << "The minterms and Don't cares represented in binary format are: " << endl;
	print_MinTable(Initial_table, num_of_var);
	cout << "The Prime Implicants are: " << endl;

	create_final_table(Initial_table, mid_table, final_table, printed_numbers); //includes the loop

	print_final_table(final_table, printed_numbers); //prints prime implicants and its covered minterms and don't cares

	vector<binary_number> essentials;

	create_essential_table(essentials, final_table, minterms);


	cout << "Essential Prime Implicants :" << endl;

	for (int i = 0; i < essentials.size(); i++)
	{
		essentials[i].print_with_dashes(num_of_var);
		cout << endl;
	}

	cout << "Minterms that are not covered by essential prime implicants: " << endl;

	for (int i = 0; i < minterms.size(); i++)
		if (!(covered_by_essential(minterms[i], essentials)))
			cout << minterms[i] << "   ";
	cout << endl;


	system("pause");

	return 0;
}

int read_from_file(ifstream& minterms_file, vector<unsigned>& minterms, vector<unsigned>& dontcares)
{

	int mint;  //minterm 
	int dont_c;  //don't care

	int lines = 1; //variable to count lines in file
	if (minterms_file.is_open())
	{
		minterms_file >> num_of_var;
		char c;
		minterms_file.get(c);
		minterms_file.get(c);
		bool flag = false;

		while (!minterms_file.eof())  //end of line
		{

			if (!(c == ',' || c == ' ' || c == '\n') && !flag)
			{
				string a = "";

				while (!(c == ',' || c == ' ' || c == '\n') && !minterms_file.eof())
				{
					a = a + c;
					minterms_file.get(c);
					if (c == '\n')
						lines++;
				}

				mint = atoi(a.c_str());
				if (out_of_Range(mint))
				{
					cout << " ERROR!! a minterm is out of the range of combinations (2^n) , please recheck your file and run again" << endl;
					system("pause");
					return 0;
				}

				if (is_minterm(mint, minterms))
				{
					cout << " ERROR!! a minterm is duplicated. we believe that this a mistake , please recheck your file and run again" << endl;
					system("pause");
					return 0;
				}
				minterms.push_back(mint);
			}
			if (c == '\n')
			{
				flag = true;
				lines++;
			}

			if (flag)
			{
				string b = "";

				while (!(c == ',' || c == ' ' || c == '\n') && !minterms_file.eof())
				{
					b = b + c;
					minterms_file.get(c);
					if (c == '\n')
						lines++;
				}

				if (b != "")
				{

					dont_c = atoi(b.c_str());
					if (out_of_Range(dont_c))
					{
						cout << " ERROR!! a Don't care is out of the range of combinations (2^n) , please recheck your file and run again" << endl;
						system("pause");
						return 0;
					}
					if (is_minterm(dont_c, minterms))
					{
						cout << " ERROR!! a number cannot be used for both don't cares and minterms, please recheck your file and run again" << endl;
						system("pause");
						return 0;
					}
					if (is_dc(dont_c, dontcares))
					{
						cout << " ERROR!! a Don't care term is duplicated. we believe that this a mistake , please recheck your file and run again" << endl;
						system("pause");
						return 0;
					}
					dontcares.push_back(dont_c);

				}

			}

			if (lines > 3)
			{
				cout << " ERROR!! wrong file format " << endl;
				system("pause");
				return 0;
			}

			minterms_file.get(c);
			if (c == '\n')
				lines++;
		}

	}

}

bool covered_by_essential(int x, vector<binary_number> essentials)
{

	for (int i = 0; i < essentials.size(); i++)
	{
		for (int j = 0; j < essentials[i].covered_mins.size(); j++)
			if (essentials[i].covered_mins[j] == x)
				return true;
	}
	return false;
}

bool in_essentials(binary_number x, vector<binary_number>essentials)
{
	for (int i = 0; i < essentials.size(); i++)
	{
		if (x.num == essentials[i].num && x.dashes == essentials[i].dashes)
			return true;
	}
	return false;
}

bool minterm_exists(binary_number t, int x, int group, int pos, vector<vector<binary_number>>& primes, vector<unsigned>& minterms)
{
	for (int i = 0; i < primes.size(); i++)
		for (int j = 0; j < primes[i].size(); j++)
		{
			binary_number r = primes[i][j];
			if ((t.num != primes[i][j].num) && (t.dashes != primes[i][j].dashes))
			{
				for (int k = 0; k < primes[i][j].covered_mins.size(); k++)
					if (is_minterm(primes[i][j].covered_mins[k], minterms))
						if (x == primes[i][j].covered_mins[k])
							return true;
			}


		}

	return false;
}

bool is_minterm(int x, vector<unsigned>& minterms)
{
	//bool flag = false;

	for (int i = 0; i < minterms.size(); i++)
	{
		if (x == minterms[i])
			return true;
	}

	return false;
}

void create_MinTable(vector<vector<binary_number>>& A, vector<unsigned>& inputs, int size)
{

	binary_number temp;
	int number_of_ones;

	for (int i = 0; i < size; i++)
	{
		number_of_ones = 0;
		binary_number temp(inputs[i]);
		number_of_ones = temp.count_ones(inputs[i]); //****
		if (number_of_ones + 1 > A.size())
			A.resize(number_of_ones + 1);
		A[number_of_ones].push_back(temp);

	}



}

void print_MinTable(vector<vector<binary_number>>& A, int s)
{

	for (int i = 0; i < A.size(); i++)
		for (int j = 0; j < A[i].size(); j++)
		{
			A[i][j].print_number(s);
			cout << endl;
		}

	cout << "\n-------------------------------------" << endl;
}

void create_combined(vector<vector<binary_number>>& Initial_table, vector<vector<binary_number>>& mid_table)
{
	short temp;
	//binary_number temp_num;
	for (int i = 0; i < Initial_table.size() - 1; i++) {

		for (int j = 0; j < Initial_table[i].size(); j++) {



			for (int k = 0; k < Initial_table[i + 1].size(); k++)
			{
				binary_number temp_num;

				if (Initial_table[i][j].dashes == Initial_table[i + 1][k].dashes) {
					temp_num.num = Initial_table[i][j].num & Initial_table[i + 1][k].num;
					temp_num.dashes = Initial_table[i][j].num ^ Initial_table[i + 1][k].num;
					if (temp_num.count_ones(temp_num.dashes) == 1)
					{
						temp_num.dashes ^= Initial_table[i][j].dashes; //***

						Initial_table[i][j].is_used = true;
						Initial_table[i + 1][k].is_used = true;

						Initial_table[i][j].covered_mins.push_back(Initial_table[i][j].num);//**
						Initial_table[i + 1][k].covered_mins.push_back(Initial_table[i + 1][k].num);//**													
																									/*temp_num.covered_mins.push_back(Initial_table[i][j].num);
																									temp_num.covered_mins.push_back(Initial_table[i+1][k].num);*/


						temp = temp_num.count_ones(temp_num.num);
						if (temp + 1 > mid_table.size())
							mid_table.resize(temp + 1);

						temp_num.is_used = false;

						for (int c = 0; c < Initial_table[i][j].covered_mins.size(); c++)
						{
							//temp_num.covered_mins.resize(temp_num.covered_mins.size() + 1);
							std::vector<unsigned>::iterator it = std::find(temp_num.covered_mins.begin(), temp_num.covered_mins.end(), Initial_table[i][j].covered_mins[c]);
							if (it == temp_num.covered_mins.end())
								temp_num.covered_mins.push_back(Initial_table[i][j].covered_mins[c]);
						}
						for (int c = 0; c < Initial_table[i + 1][k].covered_mins.size(); c++)
						{
							std::vector<unsigned>::iterator it2 = std::find(temp_num.covered_mins.begin(), temp_num.covered_mins.end(), Initial_table[i + 1][k].covered_mins[c]);
							if (it2 == temp_num.covered_mins.end())
								temp_num.covered_mins.push_back(Initial_table[i + 1][k].covered_mins[c]);
							/*cout << Initial_table[i][j].covered_mins[c];
							cout << Initial_table[i + 1][k].covered_mins[c];*/
						}

						//Initial_table[i][j].covered_mins.clear();
						//Initial_table[i + 1][k].covered_mins.clear();
						mid_table[temp].push_back(temp_num);
					}
				}
			}
		}
	}

}

bool is_dc(int x, vector<unsigned>& dontcares)
{
	for (int i = 0; i < dontcares.size(); i++)
	{
		if (x == dontcares[i])
			return true;
	}

	return false;
}

void print_combined(vector<vector<binary_number>>& mid_table, vector<binary_number>& printed_numbers)
{
	cout << endl << "MID PROCESS COMPUTATION:" << endl;
	for (int i = 0; i < mid_table.size(); i++)
	{
		cout << i;
		for (int j = 0; j < mid_table[i].size(); j++) {
			//print_p_binary(p_group[i][j].number, p_group[i][j].dashes);
			if (!is_printed(mid_table[i][j], printed_numbers))
			{
				cout << "\t\t";
				mid_table[i][j].print_with_dashes(num_of_var);
				printed_numbers.push_back(mid_table[i][j]);
				cout << endl;
			}


		}
		cout << "\n-------------------------------------" << endl;
	}
}

void create_final_table(vector<vector<binary_number>>& Initial_table, vector<vector<binary_number>>& mid_table, vector<vector<binary_number>>& final_table, vector<binary_number>& printed_numbers)
{
	bool final = false;
	binary_number temp_num;


	while (!final)
	{
		create_combined(Initial_table, mid_table);

		for (int i = 0; i < Initial_table.size(); i++)
			for (int j = 0; j < Initial_table[i].size(); j++)
			{
				if (!Initial_table[i][j].is_used)
				{
					temp_num = Initial_table[i][j];
					int temp = temp_num.count_ones(temp_num.num);
					if (temp + 1 > final_table.size())
						final_table.resize(temp + 1);
					final_table[temp].push_back(temp_num);
				}
			}

		if (mid_table.empty())
			final = true;

		Initial_table.clear();
		Initial_table = mid_table;
		mid_table.clear();

	}

}

void print_final_table(vector<vector<binary_number>>& final_table, vector<binary_number>& printed_numbers)
{
	for (int i = 0; i < final_table.size(); i++)
	{
		cout << i;
		for (int j = 0; j < final_table[i].size(); j++) {
			if (!is_printed(final_table[i][j], printed_numbers))
			{
				cout << "\t\t";
				final_table[i][j].print_with_dashes(num_of_var);
				cout << "\t\t" << "covered minterms and Don't cares are: ";
				for (int c = 0; c < final_table[i][j].covered_mins.size(); c++)
					cout << final_table[i][j].covered_mins[c] << ' ';


				cout << endl;
				printed_numbers.push_back(final_table[i][j]);
			}
		}



		cout << "\n-------------------------------------" << endl;
	}

}

void create_essential_table(vector<binary_number>& essentials, vector<vector<binary_number>>& final_table, vector<unsigned>& minterms)
{

	int k = 0;
	bool flag = true;

	for (int i = 0; i < final_table.size(); i++)
		for (int j = 0; j < final_table[i].size(); j++)
		{
			flag = true;
			for (int k = 0; k < final_table[i][j].covered_mins.size() && flag; k++)
				if (is_minterm(final_table[i][j].covered_mins[k], minterms))
				{
					if (!minterm_exists(final_table[i][j], final_table[i][j].covered_mins[k], i, j, final_table, minterms))
					{
						if (!in_essentials(final_table[i][j], essentials))
							essentials.push_back(final_table[i][j]);
						flag = false;
					}
				}
		}
}

bool is_printed(binary_number n, vector<binary_number>& printed_numbers)
{
	for (int i = 0; i < printed_numbers.size(); i++)
		if (n.num == printed_numbers[i].num && n.dashes ==
			printed_numbers[i].dashes)
			return true;
	return false;
}

bool out_of_Range(int x)
{
	int combination_nvar = pow(2, num_of_var); // number of minterms and don't cars for n variables
	if (x >= combination_nvar)
		return true;
	else
		return false;
}
