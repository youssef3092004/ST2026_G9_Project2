#include <iostream>
#include <string>
#include <algorithm>
#include <stdexcept>
using namespace std;

class BigInt {
    string number;    // Stores the number as a string
    bool isNegative;  // True if number is negative

    // Remove unnecessary leading zeros from the number string
    void removeLeadingZeros() {
        while (number.length() > 1 && number[0] == '0') {
            number.erase(0, 1);
        }
    }

    // Compare absolute values of two BigInts (ignore signs)
    // Returns: 1 if |this| > |other|, 0 if equal, -1 if |this| < |other|
    int compareMagnitude(const BigInt& other) const {
        if (number.length() > other.number.length()) return 1;
        if (number > other.number) return 1;
        if (number.length() == other.number.length()) return 0;
        if (number.length() < other.number.length()) return -1;
        if (number < other.number) return -1;
        return 0;
    }

public:
    // Default constructor - initialize to zero
    BigInt() {
        number = "0";
        isNegative = false;
    }

    // Constructor from 64-bit integer
    BigInt(int64_t value) {
        if (value < 0) {
            isNegative = true;
            value = -value;
            number = to_string(value);
        } else {
            isNegative = false;
            number = to_string(value);
        }
        removeLeadingZeros();
    }

    // Constructor from string representation
    BigInt(const string& str) {
        if (str[0] == '-') {
            isNegative = true;
            number = str.substr(1);
        }
        else {
            isNegative = false;
            number = str;
        }
    }

    // Copy constructor
    BigInt(const BigInt& other) {
        number = other.number;
        isNegative = other.isNegative;
    }

    // Destructor
    ~BigInt() {
    }

    // Assignment operator
    BigInt& operator=(const BigInt& other) {
        if (this != &other) {
            number = other.number;
            isNegative = other.isNegative;
        }
        return *this;
    }

    // Unary negation operator (-x)
    BigInt operator-() const {
        BigInt result = *this;
        if (result.number != "0") {
            result.isNegative = !result.isNegative;
        }
        return result;
    }

    // Unary plus operator (+x)
    BigInt operator+() const {

        return *this;
    }

    // Addition assignment operator (x += y)
    BigInt& operator+=(const BigInt& other) {
        if (isNegative == other.isNegative) {
            string result;
            int i = (int)number.size() - 1;
            int j = (int)other.number.size() - 1;
            int carry = 0;
            while (i >= 0 || j >= 0 || carry) {
                int digitA = (i >= 0) ? (number[i] - '0') : 0;
                int digitB = (j >= 0) ? (other.number[j] - '0') : 0;
                int sum = digitA + digitB + carry;
                carry = sum / 10;
                result.push_back(char('0' + (sum % 10)));
                i--; j--;
            }
            reverse(result.begin(), result.end());
            number = result;
        }
        else {
            int cmp = compareMagnitude(other);
            if (cmp == 0) {
                number = "0";
                isNegative = false;
            }
            else {
                const string& larger = (cmp > 0) ? number : other.number;
                const string& smaller = (cmp > 0) ? other.number : number;
                string result;
                int i = (int)larger.size() - 1;
                int j = (int)smaller.size() - 1;
                int borrow = 0;
                while (i >= 0) {
                    int digitA = larger[i] - '0';
                    int digitB = (j >= 0) ? (smaller[j] - '0') : 0;
                    int diff = digitA - digitB - borrow;
                    if (diff < 0) { diff += 10; borrow = 1; }
                    else { borrow = 0; }
                    result.push_back(char('0' + diff));
                    i--; j--;
                }
                reverse(result.begin(), result.end());
                number = result;
                if (cmp < 0) isNegative = other.isNegative;
            }
        }
        removeLeadingZeros();
        return *this;
    }

    // Subtraction assignment operator (x -= y)
    BigInt& operator-=(const BigInt& other) {
        if (isNegative == false && other.isNegative == true) {
            BigInt temp = other;
            temp.isNegative = false;

            *this += temp;
            return *this;
        }
        if (isNegative == true && other.isNegative == false) {

            BigInt temp = other;
            temp.isNegative = true;

            *this += temp;
            return *this;
        }
        int cmp = compareMagnitude(other);
        if (cmp == 0) {
            number = "0";
            isNegative = false;
            return *this;
        }
        string result = "";
        if (cmp > 0) {
            // |this| > |other|
            int i = number.length() - 1;
            int j = other.number.length() - 1;
            int borrow = 0;
            while (i >= 0) {
                int digit1 = number[i] - '0' - borrow;
                int digit2 = (j >= 0) ? other.number[j] - '0' : 0;
                if (digit1 < digit2) {
                    digit1 += 10;
                    borrow = 1;
                }
                else {
                    borrow = 0;
                }
                result += char('0' + (digit1 - digit2));
                i--;
                j--;
            }
        }
		else {
			// |other| > |this|
			int i = other.number.length() - 1;
			int j = number.length() - 1;
			int borrow = 0;
			while (i >= 0) {
				int digit1 = other.number[i] - '0' - borrow;
				int digit2 = (j >= 0) ? number[j] - '0' : 0;
				if (digit1 < digit2) {
					digit1 += 10;
					borrow = 1;
				}
				else {
					borrow = 0;
				}
				result += char('0' + (digit1 - digit2));
				i--;
				j--;
			}
			isNegative = !isNegative; // Result takes the sign of the larger magnitude
		}
        reverse(result.begin(), result.end());
        number = result;
        removeLeadingZeros();
        return *this;
    }

    // Multiplication assignment operator (x *= y)
    BigInt& operator*=(const BigInt& other) {
        // TODO: Implement this operator
        if (number == "0" || other.number == "0") {
            number = "0";
            isNegative = false;
            return *this;
        }

        bool resultNegative = (isNegative != other.isNegative);

        int n = number.size();
        int m = other.number.size();

        string result(n + m, '0');

        for (int i = n - 1; i >= 0; i--) {
            int carry = 0;

            for (int j = m - 1; j >= 0; j--) {
                int pos = i + j + 1;

                int product = (number[i] - '0') * (other.number[j] - '0')
                            + (result[pos] - '0') + carry;

                result[pos] = char('0' + (product % 10));
                carry = product / 10;
            }

            result[i] += carry;
        }

        number = result;
        isNegative = resultNegative;
        removeLeadingZeros();

        return *this;
    }

    // Division assignment operator (x /= y)
    BigInt& operator/=(const BigInt& other) {
        // TODO: Implement this operator
        if (other.number == "0") {
            throw runtime_error("Division by zero");
        }

        if (number == "0") {
            return *this;
        }

        bool resultNegative = (isNegative != other.isNegative);

        BigInt dividend(number);
        BigInt divisor(other.number);

        string quotient = "";
        BigInt remainder(0);

        for (char digit : dividend.number) {
            if (remainder.number == "0") {
                remainder.number = string(1, digit);
            }
            else {
                remainder.number += digit;
            }

            remainder.removeLeadingZeros();

            int count = 0;

            while (remainder.compareMagnitude(divisor) >= 0) {
                remainder -= divisor;
                count++;
            }

            quotient += char('0' + count);
        }

        number = quotient;
        removeLeadingZeros();

        if (number == "0") {
            isNegative = false;
        }
        else {
            isNegative = resultNegative;
        }

        return *this;
    }

    // Modulus assignment operator (x %= y)
    BigInt& operator%=(const BigInt& other) {
        // TODO: Implement this operator
        if (other.number == "0") {
            throw runtime_error("Division by zero");
        }

        bool dividendNegative = isNegative;

        BigInt quotient = *this / other;
        BigInt result = *this - (quotient * other);

        *this = result;

        if (number == "0") {
            isNegative = false;
        }
        else {
            isNegative = dividendNegative;
        }

        return *this;
    }

    // Pre-increment operator (++x)
    BigInt& operator++() {
      *this += BigInt(1);
        return *this;
    }

    // Post-increment operator (x++)
    BigInt operator++(int) {
        BigInt temp=*this;
        ++(*this);
        return temp;
    }

    // Pre-decrement operator (--x)
    BigInt& operator--() {
      *this -= BigInt(1);
        return *this;
    }

    // Post-decrement operator (x--)
    BigInt operator--(int) {
        BigInt temp=*this;
        --(*this);
        return temp;
    }

    // Convert BigInt to string representation
    string toString() const {
        // TODO: Implement this function
        if (isNegative && number != "0") {
            return "-" + number;
        }
        return number;
    }

    // Output stream operator (for printing)
    friend ostream& operator<<(ostream& os, const BigInt& num) {
        // TODO: Implement this operator
        if (num.isNegative && num.number != "0") {
            os << '-';
        }

        os << num.number;
        return os;
    }

    // Input stream operator (for reading from input)
    friend istream& operator>>(istream& is, BigInt& num) {
        // TODO: Implement this operator
        string str;
        is >> str;
        num = BigInt(str);
        return is;
    }

    // Friend declarations for comparison operators
    friend bool operator==(const BigInt& lhs, const BigInt& rhs);
    friend bool operator<(const BigInt& lhs, const BigInt& rhs);
};

// Binary addition operator (x + y)
BigInt operator+(BigInt lhs, const BigInt& rhs) {
    lhs += rhs;
    return lhs;
}

// Binary subtraction operator (x - y)
BigInt operator-(BigInt lhs, const BigInt& rhs) {
    BigInt result;
    // TODO: Implement this operator
    lhs -= rhs;
    return lhs;
}

// Binary multiplication operator (x * y)
BigInt operator*(BigInt lhs, const BigInt& rhs) {
    BigInt result;
    // TODO: Implement this operator
    lhs *= rhs;
    return lhs;
}

// Binary division operator (x / y)
BigInt operator/(BigInt lhs, const BigInt& rhs) {
    BigInt result;
    // TODO: Implement this operator
    lhs /= rhs;
    return lhs;
}

// Binary modulus operator (x % y)
BigInt operator%(BigInt lhs, const BigInt& rhs) {
    BigInt result;
    // TODO: Implement this operator
    lhs %= rhs;
    return lhs;
}

// Equality comparison operator (x == y)
bool operator==(const BigInt& lhs, const BigInt& rhs) {
    // TODO: Implement this operator
    if (lhs.isNegative != rhs.isNegative) {
        return false;
    }

    return lhs.number == rhs.number;
}

// Inequality comparison operator (x != y)
bool operator!=(const BigInt& lhs, const BigInt& rhs) {
    // TODO: Implement this operator
    return !(lhs == rhs);
}

// Less-than comparison operator (x < y)
bool operator<(const BigInt& lhs, const BigInt& rhs) {
    // TODO: Implement this operator
    if (lhs.isNegative && !rhs.isNegative) {
        return true;
    }

    if (!lhs.isNegative && rhs.isNegative) {
        return false;
    }

    int cmp = lhs.compareMagnitude(rhs);

    if (!lhs.isNegative) {
        return cmp < 0;
    }

    return cmp > 0;
}

// Less-than-or-equal comparison operator (x <= y)
bool operator<=(const BigInt& lhs, const BigInt& rhs) {
    // TODO: Implement this operator
    return (lhs < rhs) || (lhs == rhs);
}

// Greater-than comparison operator (x > y)
bool operator>(const BigInt& lhs, const BigInt& rhs) {
    // TODO: Implement this operator
    return !(lhs <= rhs);
}

// Greater-than-or-equal comparison operator (x >= y)
bool operator>=(const BigInt& lhs, const BigInt& rhs) {
    // TODO: Implement this operator
    return !(lhs < rhs);
}

int main() {
    cout << "=== BigInt Class Test Program ===" << endl << endl;
    cout << "NOTE: All functions are currently empty." << endl;
    cout << "Your task is to implement ALL the functions above." << endl;
    cout << "The tests below will work once you implement them correctly." << endl << endl;

    /*
    // Test 1: Constructors and basic output
    cout << "1. Constructors and output:" << endl;
    BigInt a(12345);              // Should create BigInt from integer
    BigInt b("-67890");           // Should create BigInt from string
    BigInt c("0");                // Should handle zero correctly
    BigInt d = a;                 // Should use copy constructor
    cout << "a (from int): " << a << endl;        // Should print "12345"
    cout << "b (from string): " << b << endl;     // Should print "-67890"
    cout << "c (zero): " << c << endl;            // Should print "0"
    cout << "d (copy of a): " << d << endl << endl; // Should print "12345"

    // Test 2: Arithmetic operations
    cout << "2. Arithmetic operations:" << endl;
    cout << "a + b = " << a + b << endl;          // Should calculate 12345 + (-67890)
    cout << "a - b = " << a - b << endl;          // Should calculate 12345 - (-67890)
    cout << "a * b = " << a * b << endl;          // Should calculate 12345 * (-67890)
    cout << "b / a = " << b / a << endl;          // Should calculate (-67890) / 12345
    cout << "a % 100 = " << a % BigInt(100) << endl << endl; // Should calculate 12345 % 100

    // Test 3: Relational operators
    cout << "3. Relational operators:" << endl;
    cout << "a == d: " << (a == d) << endl;       // Should be true (12345 == 12345)
    cout << "a != b: " << (a != b) << endl;       // Should be true (12345 != -67890)
    cout << "a < b: " << (a < b) << endl;         // Should be false (12345 < -67890)
    cout << "a > b: " << (a > b) << endl;         // Should be true (12345 > -67890)
    cout << "c == 0: " << (c == BigInt(0)) << endl << endl; // Should be true (0 == 0)

    // Test 4: Unary operators and increments
    cout << "4. Unary operators and increments:" << endl;
    cout << "-a: " << -a << endl;                 // Should print "-12345"
    cout << "++a: " << ++a << endl;               // Should increment and print "12346"
    cout << "a--: " << a-- << endl;               // Should print "12346" then decrement
    cout << "a after decrement: " << a << endl << endl; // Should print "12345"

    // Test 5: Large number operations
    cout << "5. Large number operations:" << endl;
    BigInt num1("12345678901234567890");
    BigInt num2("98765432109876543210");
    cout << "Very large addition: " << num1 + num2 << endl;
    cout << "Very large multiplication: " << num1 * num2 << endl << endl;

    // Test 6: Edge cases and error handling
    cout << "6. Edge cases:" << endl;
    BigInt zero(0);
    BigInt one(1);
    try {
        BigInt result = one / zero;               // Should throw division by zero error
        cout << "Division by zero succeeded (unexpected)" << endl;
    } catch (const runtime_error& e) {
        cout << "Division by zero correctly threw error: " << e.what() << endl;
    }
    cout << "Multiplication by zero: " << one * zero << endl;        // Should be "0"
    cout << "Negative multiplication: " << BigInt(-5) * BigInt(3) << endl;  // Should be "-15"
    cout << "Negative division: " << BigInt(-10) / BigInt(3) << endl;       // Should be "-3"
    cout << "Negative modulus: " << BigInt(-10) % BigInt(3) << endl;        // Should be "-1"
    */

    return 0;
}
