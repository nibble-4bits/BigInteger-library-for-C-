#include "BigInteger.h"

BigInteger::BigInteger()
	: m_digits("0"),
	isSigned(false)
{
}

BigInteger::BigInteger(const std::string& integer)
	: m_digits(integer),
	isSigned(false)
{
	if (m_digits[0] == '-') {
		isSigned = true;
		m_digits = m_digits.substr(1); // Remove the minus sign
	}

	std::size_t firstNotOfLeadingZero = m_digits.find_first_not_of('0');
	if (firstNotOfLeadingZero != std::string::npos) { // Remove all leading zeroes in case there are
		m_digits = m_digits.substr(firstNotOfLeadingZero);
	}
	else {
		m_digits = "0";
	}
}

BigInteger::BigInteger(const std::string& integer, bool sign)
	: m_digits(integer),
	isSigned(sign)
{
	if (m_digits[0] == '-') {
		m_digits = m_digits.substr(1); // Remove the minus sign
	}

	std::size_t firstNotOfLeadingZero = m_digits.find_first_not_of('0');
	if (firstNotOfLeadingZero != std::string::npos) { // Remove all leading zeroes in case there are
		m_digits = m_digits.substr(firstNotOfLeadingZero);
	}
	else {
		m_digits = "0";
	}
}

bool BigInteger::isEqualTo(const BigInteger& other) const
{
	return (m_digits == other.m_digits && isSigned == other.isSigned);
}

bool BigInteger::isNotEqualTo(const BigInteger& other) const
{
	return !isEqualTo(other);
}

bool BigInteger::isGreaterThan(const BigInteger& other) const
{
	if (!isSigned && other.isSigned) {       // this is positive and other is negative
		return true;
	}
	else if (isSigned && !other.isSigned) {  // this is negative and other is positive
		return false;
	}
	else if (!isSigned && !other.isSigned) { // both are positive
		if (m_digits.length() > other.m_digits.length()) {
			return true;
		}
		else if (m_digits.length() < other.m_digits.length()) {
			return false;
		}

		for (size_t i = 0; i < m_digits.length(); i++) {
			if (m_digits[i] > other.m_digits[i]) {
				return true;
			}
			else if (m_digits[i] < other.m_digits[i]) {
				return false;
			}
		}
	}
	else {                                   // both are negative
		if (m_digits.length() < other.m_digits.length()) {
			return true;
		}
		else if (m_digits.length() > other.m_digits.length()) {
			return false;
		}

		for (size_t i = 0; i < m_digits.length(); i++) {
			if (m_digits[i] < other.m_digits[i]) {
				return true;
			}
			else if (m_digits[i] > other.m_digits[i]) {
				return false;
			}
		}
	}
	return false;
}

bool BigInteger::isLessThan(const BigInteger& other) const
{
	return (!isGreaterThan(other) && isNotEqualTo(other));
}

bool BigInteger::isGreaterThanOrEqualTo(const BigInteger& other) const
{
	return (isGreaterThan(other) || isEqualTo(other));
}

bool BigInteger::isLessThanOrEqualTo(const BigInteger& other) const
{
	return (isLessThan(other) || isEqualTo(other));
}

bool BigInteger::isZero() const
{
	return (m_digits == "0");
}

BigInteger BigInteger::abs(const BigInteger& integer)
{
	return BigInteger(integer.m_digits, false);
}

BigInteger BigInteger::add(BigInteger addend1, BigInteger addend2)
{
	if (addend1.isZero()) {
		return addend2;
	}
	else if (addend2.isZero()) {
		return addend1;
	}
	else if (addend1.m_digits == addend2.m_digits && addend1.isSigned != addend2.isSigned) { // a plus (-a) = 0
		return BigInteger("0");
	}
	else if (!addend1.isSigned && addend2.isSigned) { // positive plus negative
		return BigInteger::subtract(addend1, BigInteger::abs(addend2));
	}
	else if (addend1.isSigned && !addend2.isSigned) { // negative plus positive
		return BigInteger::subtract(addend2, BigInteger::abs(addend1));
	}
	else { // positive plus positive OR negative plus negative
		std::string add = "";
		bool additionSign = false;

		if (addend1.isSigned && addend2.isSigned) { // both are negative
			additionSign = true;
		}

		if (addend1.m_digits.length() > addend2.m_digits.length()) {
			const int lengthDifference = addend1.m_digits.length() - addend2.m_digits.length();

			for (int i = addend1.m_digits.length() - 1; i >= lengthDifference; i--) {
				char digitsSum = (addend1.m_digits[i] - '0') + (addend2.m_digits[i - lengthDifference] - '0');
				add.insert(0, 1, (digitsSum % 10) + '0');

				if (digitsSum > 9) {
					addend1.m_digits[i - 1]++;
				}
			}

			add = addend1.m_digits.substr(0, lengthDifference) + add;
		}
		else {
			const int lengthDifference = addend2.m_digits.length() - addend1.m_digits.length();

			for (int i = addend2.m_digits.length() - 1; i >= lengthDifference; i--) {
				char digitsSum = (addend2.m_digits[i] - '0') + (addend1.m_digits[i - lengthDifference] - '0');
				add.insert(0, 1, (digitsSum % 10) + '0');

				if (digitsSum > 9) { // adding the carry to the next digit
					if (i > 0) {
						addend2.m_digits[i - 1]++;
					}
					else {
						add.insert(0, 1, '1');
					}
				}
			}

			add = addend2.m_digits.substr(0, lengthDifference) + add;
		}

		return BigInteger(add, additionSign);
	}
}

BigInteger BigInteger::subtract(BigInteger minuend, BigInteger subtrahend)
{
	if (minuend.isEqualTo(subtrahend)) {
		return BigInteger("0");
	}

	std::string diff = "";

	if (minuend.isLessThan(subtrahend)) {
		const int lengthDifference = subtrahend.m_digits.length() - minuend.m_digits.length();

		for (int i = subtrahend.m_digits.length() - 1; i >= lengthDifference; i--) {
			if (subtrahend.m_digits[i] < minuend.m_digits[i - lengthDifference]) {
				subtrahend.m_digits[i] += 10;
				subtrahend.m_digits[i - 1]--;
			}

			diff = static_cast<char>((subtrahend.m_digits[i] - '0') - (minuend.m_digits[i - lengthDifference] - '0') + '0') + diff;
		}

		diff = subtrahend.m_digits.substr(0, lengthDifference) + diff;
		return BigInteger(diff, true);
	}
	else { // minuend is greater than subtrahend
		const int lengthDifference = minuend.m_digits.length() - subtrahend.m_digits.length();

		for (int i = minuend.m_digits.length() - 1; i >= lengthDifference; i--) {
			if (minuend.m_digits[i] < subtrahend.m_digits[i - lengthDifference]) {
				minuend.m_digits[i] += 10;
				minuend.m_digits[i - 1]--;
			}

			diff = static_cast<char>((minuend.m_digits[i] - '0') - (subtrahend.m_digits[i - lengthDifference] - '0') + '0') + diff;
		}

		diff = minuend.m_digits.substr(0, lengthDifference) + diff;
		return BigInteger(diff, false);
	}
}

/*BigInteger BigInteger::multiply(BigInteger multiplicand, BigInteger multiplier)
{
}
*/
std::string BigInteger::toString()
{
	// if negative, prepend the minus sign, otherwise return the digits as they are
	return isSigned ? '-' + m_digits : m_digits;
}
