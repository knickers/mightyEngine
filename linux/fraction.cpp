#include "fraction.h"

frac::frac(int num, int den)
{
	mNumerator   = num;
	mDenominator = den;
	this->reduce();
}

int  frac::num() const
{
	return mNumerator;
}

void frac::num(int const& n)
{
	mNumerator = n;
}

int  frac::den() const
{
	return mDenominator;
}

void frac::den(int const& d)
{
	if(d != 0)
		mDenominator = d;
}

void frac::reduce()
{
	int devisor = 2;
	while(mNumerator >= devisor && mDenominator >= devisor)
	{
		if(mNumerator%devisor == 0 && mDenominator%devisor == 0) {
			mNumerator   /= devisor;
			mDenominator /= devisor;
		} else {
			if(devisor==2) {
				devisor++;
			} else {
				devisor += 2;
			}
		}
	}
}

void frac::inverse()
{
	swap(mNumerator, mDenominator);
}

frac & frac::operator = (const frac& rhs)
{
	this->num(rhs.num());
	this->den(rhs.den());
	return * this;
}

frac & frac::operator = (const int& rhs)
{
	this->num(rhs);
	this->den(1);
	return * this;
}

frac operator + (const frac& lhs, const frac& rhs)
{
	frac a(lhs.num()*rhs.den() + rhs.num()*lhs.den(), lhs.den()*rhs.den());
	a.reduce();
	return a;
}

frac operator + (const frac& lhs, const int & rhs)
{
	frac a(lhs.num() + lhs.den()*rhs, lhs.den());
	a.reduce();
	return a;
}

frac operator + (const int & lhs, const frac& rhs)
{
	frac a = rhs + lhs;
	a.reduce();
	return a;
}

frac operator - (const frac& lhs, const frac& rhs)
{
	frac a(lhs.num()*rhs.den() - rhs.num()*lhs.den(), lhs.den()*rhs.den());
	a.reduce();
	return a;
}

frac operator - (const frac& lhs, const int & rhs)
{
	frac a(lhs.num() - lhs.den()*rhs, lhs.den());
	a.reduce();
	return a;
}

frac operator - (const int & lhs, const frac& rhs)
{
	frac a(rhs.den()*lhs - rhs.num(), rhs.den());
	a.reduce();
	return a;
}

frac operator * (const frac& lhs, const frac& rhs)
{
	frac a(lhs.num()*rhs.num(), lhs.den()*rhs.den());
	a.reduce();
	return a;
}

frac operator * (const frac& lhs, const int & rhs)
{
	frac a = rhs * lhs;
	a.reduce();
	return a;
}

frac operator * (const int & lhs, const frac& rhs)
{
	frac a(lhs*rhs.num(), rhs.den());
	a.reduce();
	return a;
}

frac operator / (const frac& lhs, const frac& rhs)
{
	frac a = rhs;
	a.inverse();
	return lhs * a;
}

frac operator / (const frac& lhs, const int & rhs)
{
	frac a(1, rhs);
	return lhs * a;
}

frac operator / (const int & lhs, const frac& rhs)
{
	frac a(lhs, 1);
	frac b = rhs;
	b.inverse();
	return a * b;
}

bool operator == (const frac& lhs, const frac & rhs)
{
	return (lhs.num()*rhs.den() == rhs.num()*lhs.den());
}

bool operator == (const frac& lhs, const int & rhs)
{
	frac a(rhs, 1);
	return lhs == a;
}

bool operator == (const int & lhs, const frac& rhs)
{
	frac a(lhs, 1);
	return a == rhs;
}

bool operator  > (const frac & lhs, const frac & rhs)
{
	return (lhs.num()*rhs.den() > rhs.num()*lhs.den());
}

bool operator  < (const frac & lhs, const frac & rhs)
{
	return (lhs.num()*rhs.den() < rhs.num()*lhs.den());
}

bool operator >= (const frac & lhs, const frac & rhs)
{
	return (lhs.num()*rhs.den() >= rhs.num()*lhs.den());
}

bool operator <= (const frac & lhs, const frac & rhs)
{
	return (lhs.num()*rhs.den() <= rhs.num()*lhs.den());
}

bool operator != (const frac & lhs, const frac & rhs)
{
	return (lhs.num()*rhs.den() != rhs.num()*lhs.den());
}

ostream & operator << (ostream & out, const frac & rhs)
{
	out << rhs.num() << "/" << rhs.den();
	return out;
}

istream & operator >> (istream & in, frac & rhs)
{
	int n=0, d=1;
	char c;
	in >> n >> c >> d; // n/d
	rhs.num(n);
	rhs.den(d);
	return in;
}
