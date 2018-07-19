#ifndef BIGINTEGER_H
#define BIGINTEGER_H

#include <cstdint>
#include <string>

class BigInteger {
	private:
		std::string m_digits; // string representation of the BigInteger object
		bool isSigned;
	public:
		BigInteger();
		BigInteger(const std::string& integer);
		BigInteger(const std::string& integer, bool sign);
		bool isEqualTo(const BigInteger& other) const;
		bool isNotEqualTo(const BigInteger& other) const;
		bool isGreaterThan(const BigInteger& other) const;
		bool isLessThan(const BigInteger& other) const;
		bool isGreaterThanOrEqualTo(const BigInteger& other) const;
		bool isLessThanOrEqualTo(const BigInteger& other) const;
		bool isZero() const;
		static BigInteger abs(const BigInteger& integer);
		static BigInteger add(BigInteger addend1, BigInteger addend2);
		static BigInteger subtract(BigInteger minuend, BigInteger subtrahend);
		std::string toString();
};

#endif // BIGINTEGER_H


