//Name: Josh Varisco
// Class: (CECS 325-01)
// Prog 6: BigInt
// 05/07/25
//
// I ceritfy that this is my program with my original work.
// I further certify that I typed each line of code in the program.


#include<iostream>
#include<vector>
#include<iomanip>
#include<climits> //for INT_MAX
using namespace std;


class BigInt {
private:
	vector<char> v;

public:
	//Constructors
	BigInt() {
		v.push_back(0);
	}

	BigInt(int num) {
		if (num == 0) {
			v.push_back(0);
			return;
		}
		while (num > 0) {
			v.insert(v.begin(), num % 10);
			num /= 10;
		}
	}

	BigInt(string s) {
		for (char c : s) {
			if (isdigit(c)) {
				v.push_back(c - '0');
			}
		}
	}

	int size() {
		return v.size();
	}

	void print() {
		for (char digit : v) {
			cout << (int)digit;
		}
	}

	//Operators
	BigInt operator+ (BigInt);
	BigInt operator- (BigInt);
	BigInt operator- (int);
	BigInt operator* (BigInt);

	BigInt operator/ (BigInt);
	BigInt operator% (BigInt);
	bool   operator== (const BigInt& other) const;
	bool   operator<= (const BigInt& other) const;
	bool   operator< (const BigInt& other) const;
	BigInt operator++ (int);
	BigInt operator++ ();
	BigInt operator[] (int); //index

	//friends, etc.
	BigInt fibo(BigInt);
	BigInt fact();
	friend BigInt operator+ (int, BigInt);
	friend ostream& operator<<(ostream&, const BigInt&);

};

/*													*/		
/*		just to seperate the code from above		*/
/*													*/

BigInt BigInt::operator+(BigInt other) {
	//c = a + b
	BigInt result;
	result.v.clear(); 

	int carry = 0;
	int i = this->v.size() - 1;
	int j = other.v.size() - 1;

	while (i >= 0 || j >= 0 || carry) {
		int digit1 = (i >= 0) ? this->v[i] : 0;
		int digit2 = (j >= 0) ? other.v[j] : 0;

		int sum = digit1 + digit2 + carry;
		carry = sum / 10;
		result.v.insert(result.v.begin(), sum % 10);

		i--;
		j--;
	}

	return result;
}

BigInt BigInt::operator-(BigInt other) {
	//c = a - b
	BigInt result;
	result.v.clear();

	int borrow = 0;
	int i = this->v.size() - 1;
	int j = other.v.size() - 1;

	while (i >= 0) {
		int digit1 = this->v[i];
		int digit2 = (j >= 0) ? other.v[j] : 0;
		int diff = digit1 - digit2 - borrow;
		
		if (diff < 0) {
			diff += 10;
			borrow = 1;
		}
		else {
			borrow = 0;
		}

		result.v.insert(result.v.begin(), diff);
		i--; 
		j--;
	}

	while (result.v.size() > 1 && result.v[0] == 0) {
		result.v.erase(result.v.begin());
	}

	return result;
}

BigInt BigInt::operator-(int x) {
	//c = a - x
	return *this - BigInt(x);
}

BigInt BigInt::fibo(BigInt n) {
	//fibo function (fix)

	BigInt a(0), b(1);

	if (n <= BigInt(1)) {
		return n;
	}
	
	for (BigInt i = 2; i <= n; i = i + 1) {
		BigInt temp = a + b;
		a = b;
		b = temp;
	}

	return b;
}

BigInt BigInt::fact() {
	//fact function (fix) (5! = 5 * 4 * ...)
	BigInt result(1);
	BigInt count = *this;
	BigInt one(1);

	while (BigInt(0) < count) {
		//cout << "count: " << count << endl;
		result = result * count;
		count = count - one;
	}
	
	return result;
}

BigInt BigInt::operator*(BigInt other) {
	//c = a * b
	BigInt result;
	result.v.clear();
	int start = 0;

	int n = this->v.size();
	int m = other.v.size();

	vector<int> temp(n + m, 0);
	
	for (int i = n - 1; i >= 0; --i) {
		for (int j = m - 1; j >= 0; --j) {

			int prod = this->v[i] * other.v[j];
			int sum = temp[i + j + 1] + prod;
			temp[i + j + 1] = sum % 10;	//result
			temp[i + j] += sum / 10;	//carry

		}
	}

	while (start < (int)temp.size() - 1 && temp[start] == 0) {
		++start;
	}

	for (int k = start; k < (int)temp.size(); ++k) {
		result.v.push_back(char(temp[k]));
	}

	return result;
}

BigInt BigInt::operator/(BigInt other) {
	//c = a / b 
	BigInt result;
	BigInt remainder;

	if (other == BigInt(0)) {
		throw runtime_error("Can't divide by zero");
	}

	for (size_t i = 0; i < this->v.size(); ++i) {
		
		remainder.v.push_back(this->v[i]);

		while (remainder.v.size() > 1 && remainder.v[0] == 0) {
			remainder.v.erase(remainder.v.begin());
		}

		int count = 0;
		while (other <= remainder) {
			remainder = remainder - other;
			count++;
		}

		result.v.push_back(count);
	}

	while (result.v.size() > 1 && result.v[0] == 0) {
		result.v.erase(result.v.begin());
	}

	return result;
}

BigInt BigInt::operator%(BigInt other) {
	//c = a % b
	BigInt result;
	BigInt remainder;

	if (other == BigInt(0)) {
		throw runtime_error("Can't divide by zero");
	}

	for (size_t i = 0; i < this->v.size(); ++i) {

		remainder.v.push_back(this->v[i]);

		while (remainder.v.size() > 1 && remainder.v[0] == 0) {
			remainder.v.erase(remainder.v.begin());
		}

		while (other <= remainder) {
			remainder = remainder - other;
		}
	}

	while (result.v.size() > 1 && result.v[0] == 0) {
		remainder.v.erase(remainder.v.begin());
	}

	return remainder;
}

bool BigInt::operator<=(const BigInt& other) const {
	// a <= b
	return (*this < other) || (*this == other);
}

bool BigInt::operator<(const BigInt& other) const {
	// a < b

	if (v.size() != other.v.size())
		return v.size() < other.v.size();

	for (size_t i = 0; i < v.size(); ++i) {
		if (v[i] != other.v[i])
			return v[i] < other.v[i];
	}
	return false; // they are equal
}

bool BigInt::operator==(const BigInt& other) const {
	//a == b
	if (this->v.size() != other.v.size()) {
		return false;
	}

	for (size_t i = 0; i < v.size(); ++i) {
		if (v[i] != other.v[i]) {
			return false;
		}
	}

	return true;
}

BigInt BigInt::operator++(int) {
	//a++
	BigInt temp = *this;
	*this = *this + BigInt(1);
	return temp;
}

BigInt BigInt::operator++() {
	//++a
	*this = *this + BigInt(1);
	return *this;
}

BigInt BigInt::operator[](int x) {
	//index
	BigInt result;
	result.v.push_back(this->v[x]);

	return result;
}

BigInt operator+(int x, BigInt b) {
	//c = x + a
	return BigInt(x) + b;
}

ostream& operator<<(ostream& out, const BigInt& b) {
	//cout
	size_t num_digits = b.v.size();

	if (num_digits > 12) {
		out << int(b.v[0]) << '.';
		for (size_t i = 1; i < 6 && i < num_digits; ++i) {
			out << int(b.v[i]);
		}
		out << 'e' << (num_digits - 1);
	}
	else {
		for (char digit : b.v) {
			out << int(digit);
		}
	}

	return out;
}


/*													*/
/*		  this is just fo main to look nice  		*/
/*													*/


int main() {

	int space = 10;
	cout << "\a\nTestUnit:\n" << flush;
	system("whoami");
	system("date");

	// initialize variables
	BigInt n1(25);
	BigInt s1("25");
	BigInt n2(1234);
	BigInt s2("1234");
	BigInt n3(n2);
	BigInt fibo(12345);
	BigInt fact(50);
	BigInt imax = INT_MAX;
	BigInt big("9223372036854775807");
	
	// display variables
	cout << "n1(int) :" << setw(space) << n1 << endl;
	cout << "s1(str) :" << setw(space) << s1 << endl;
	cout << "n2(int) :" << setw(space) << n2 << endl;
	cout << "s2(str) :" << setw(space) << s2 << endl;
	cout << "n3(n2) :" << setw(space) << n3 << endl;
	cout << "fibo(12345):" << setw(space) << fibo << endl;
	cout << "fact(50) :" << setw(space) << fact << endl;
	cout << "imax :" << setw(space) << imax << endl;
	cout << "big :" << setw(space) << big << endl;
	cout << "big.print(): "; big.print(); cout << endl;
	
	// perform operations
	cout << n2 << "/" << n1 << " = " << n2 / n1 << " rem " << n2 % n1 << endl;
	cout << "fibo(" << fibo << ") = " << fibo.fibo(fibo) << endl;
	cout << "fact(" << fact << ") = " << fact.fact() << endl;
	cout << "10 + n1 = " << 10 + n1 << endl;
	cout << "n1 + 10 = " << n1 + 10 << endl;
	cout << "n2 - 100 = " << n2 - 100 << endl;
	cout << "(n1 == s1)? --> " << ((n1 == s1) ? "true" : "false") << endl;
	cout << "n1++ = ? --> before:" << n1++ << " after:" << n1 << endl;
	cout << "++s1 = ? --> before:" << ++s1 << " after:" << s1 << endl;
	cout << "s2 * big = ? --> " << s2 * big << endl;
	cout << "big * s2 = ? --> " << big * s2 << endl;
	
}


