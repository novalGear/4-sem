#include "fraction.hpp"
#include <stdexcept>
#include <iostream>
#include <cassert>
#include <cmath>
#include <limits>

// вспомогательные функции ============================================================================================//

// НОД — Greatest Common Divisor
int GCD(int a, int b) {
    a = std::abs(a);
    b = std::abs(b);
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

int LCM(int a, int b) {
    if (a == 0 || b == 0) return 0;
    return (std::abs(a) / GCD(a, b)) * std::abs(b);
}


//  методы Fraction ===================================================================================================//

Fraction::Fraction(int num, int denom) {
    if (denom == 0) {
        throw std::invalid_argument("Denominator cannot be zero");
    }
    // Приводим знак к числителю
    if (denom < 0) {
        num = -num;
        denom = -denom;
    }
    num_ = num;
    denom_ = denom;
}

Fraction::Fraction(float x) {
    if (std::isnan(x) || std::isinf(x)) {
        throw std::invalid_argument("Cannot convert NaN or infinity to Fraction");
    }

    if (std::abs(x) < std::numeric_limits<float>::epsilon()) {
        num_ = 0;
        denom_ = 1;
        return;
    }

    bool negative = x < 0.0f;
    x = std::abs(x);

    // Точность: 6 знаков после запятой
    const float scale = 1'000'000.0f; // 10^6

    // Округляем, чтобы избежать ошибок из-за неточного представления
    long long num = static_cast<long long>(std::round(x * scale));
    long long denom = static_cast<long long>(scale);

    if (negative) num = -num;

    Fraction temp(static_cast<int>(num), static_cast<int>(denom));
    *this = temp.reduced();
}

bool Fraction::is_ok() const {
    return denom_ > 0;
}

Fraction Fraction::reduced() const {
    if (num_ == 0) {
        return Fraction(0, 1);
    }
    int gcd = GCD(num_, denom_);
    return Fraction(num_ / gcd, denom_ / gcd);
}

void Fraction::print() const {
    std::cout << num_ << "/" << denom_ << "\n";
}

float Fraction::asFloat() const {
    return static_cast<float>(num_) / static_cast<float>(denom_);
}

// перегрузка для печати

std::ostream& operator<<(std::ostream& os, const Fraction& f) {
    return os << f.numerator() << "/" << f.denominator();
}

// свободные функции с имплементацией арифметики ======================================================================//

Fraction operator+(const Fraction& a, const Fraction& b) {
    assert(a.is_ok() && b.is_ok());

    int res_nom = a.numerator() * b.denominator() + b.numerator() * a.denominator();
    int res_denom = a.denominator() * b.denominator();

    Fraction result(res_nom, res_denom);
    assert(result.is_ok());
    return result.reduced();
}

Fraction operator-(const Fraction& a, const Fraction& b) {
    assert(a.is_ok() && b.is_ok());

    int res_nom = a.numerator() * b.denominator() - b.numerator() * a.denominator();
    int res_denom = a.denominator() * b.denominator();

    Fraction result(res_nom, res_denom);
    assert(result.is_ok());
    return result.reduced();
}

Fraction operator*(const Fraction& a, const Fraction& b) {
    assert(a.is_ok() && b.is_ok());

    int res_nom   = a.numerator() * b.numerator();
    int res_denom = a.denominator() * b.denominator();

    Fraction result(res_nom, res_denom);
    assert(result.is_ok());
    return result.reduced();
}


Fraction operator/(const Fraction& a, const Fraction& b) {
    assert(a.is_ok() && b.is_ok());

    if (b.numerator() == 0) {
        throw std::invalid_argument("division by zero");
    }

    int res_nom   = a.numerator() * b.denominator();
    int res_denom = a.denominator() * b.numerator();

    Fraction result(res_nom, res_denom);
    assert(result.is_ok());
    return result.reduced();
}

// сравнения ==========================================================================================================//

bool operator==(const Fraction& a, const Fraction& b) {
    assert(a.is_ok() && b.is_ok());
    return a.numerator() * b.denominator() == b.numerator() * a.denominator();
}

bool operator>(const Fraction& a, const Fraction& b) {
    assert(a.is_ok() && b.is_ok());

    Fraction difference = a - b;
    return (difference.numerator() > 0);
}

bool operator<(const Fraction& a, const Fraction& b) {
    assert(a.is_ok() && b.is_ok());

    Fraction difference = a - b;
    assert(difference.is_ok());
    return (difference.numerator() < 0);
}



/* дописать класс и поддтердить тестом
Fraction test program results
frac1 =3/4
frac2 = 20/23
Sum of fraс1 and frac2 = frac3 = 149/92
Accessing methods test
frac3 nomerator = 149 frac3 denominator = 92
Comparison methods
 frac2 greater than frac1
 frac1 less than frac3
 frac4 equal to frac3
More arithmetic methods
Difference of frac1 and frac2 = -11/92
Product of frac1 and frac2 = 15/23
Quotient of frac2 / frac3 = 80/149
*/
