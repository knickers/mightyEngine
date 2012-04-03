#pragma once

#include <fstream>
using namespace std;

class frac
{
private:
	int mNumerator;
	int mDenominator;
public:
	frac(int num, int den);
	int  num() const ;
	int  den() const ;
	void num(int const & n);
	void den(int const & d);
	void reduce();
	void inverse();

	frac & operator = (const frac & rhs);
	frac & operator = (const int & rhs);

	friend frac operator + (const frac& lhs, const frac& rhs);
	friend frac operator + (const frac& lhs, const int & rhs);
	friend frac operator + (const int & lhs, const frac& rhs);
	friend frac operator - (const frac& lhs, const frac& rhs);
	friend frac operator - (const frac& lhs, const int & rhs);
	friend frac operator - (const int & lhs, const frac& rhs);
	friend frac operator * (const frac& lhs, const frac& rhs);
	friend frac operator * (const frac& lhs, const int & rhs);
	friend frac operator * (const int & lhs, const frac& rhs);
	friend frac operator / (const frac& lhs, const frac& rhs);
	friend frac operator / (const frac& lhs, const int & rhs);
	friend frac operator / (const int & lhs, const frac& rhs);

	friend bool operator == (const frac& lhs, const frac& rhs);
	friend bool operator == (const frac& lhs, const int & rhs);
	friend bool operator == (const int & lhs, const frac& rhs);
	friend bool operator  > (const frac& lhs, const frac& rhs);
	friend bool operator  < (const frac& lhs, const frac& rhs);
	friend bool operator >= (const frac& lhs, const frac& rhs);
	friend bool operator <= (const frac& lhs, const frac& rhs);
	friend bool operator != (const frac& lhs, const frac& rhs);

	friend ostream & operator << (ostream & out, const frac & rhs);
	friend istream & operator >> (istream & in, frac & rhs);
};
