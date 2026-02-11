#include "fraction.hpp"
#include <iostream>
#include <cassert>
#include <cmath>

bool floatEquals(float a, float b, float epsilon = 1e-5f) {
    return std::abs(a - b) < epsilon;
}

int main() {
    Fraction frac1(3, 4);
    Fraction frac2(20, 23);

    std::cout << "frac1 = " << frac1 << "\n";
    std::cout << "frac2 = " << frac2 << "\n";

    Fraction frac3 = frac1 + frac2;
    std::cout << "Sum of frac1 and frac2 = frac3 = " << frac3 << "\n";

    std::cout << "frac3 numerator = " << frac3.numerator()
              << " frac3 denominator = " << frac3.denominator() << "\n";

    std::cout << "Comparison methods:\n";
    if (frac2 > frac1) std::cout << "frac2 greater than frac1\n";
    if (frac1 < frac3) std::cout << "frac1 less than frac3\n";

    Fraction frac4 = frac3;
    if (frac4 == frac3) std::cout << "frac4 equal to frac3\n";

    std::cout << "More arithmetic methods:\n";
    std::cout << "Difference of frac1 and frac2 = " << (frac1 - frac2) << "\n";
    std::cout << "Product of frac1 and frac2 = " << (frac1 * frac2) << "\n";
    std::cout << "Quotient of frac2 / frac3 = " << (frac2 / frac3) << "\n";


    std::cout << "\n--- Additional tests ---\n";

    // Тест 1: 2 + Fraction(1,3) == Fraction(1,3) + 2 == Fraction(7,3)
    Fraction f1(1, 3);
    Fraction expected1(7, 3);
    Fraction sum1_left = 2 + f1;      // int + Fraction
    Fraction sum1_right = f1 + 2;     // Fraction + int

    std::cout << "2 + 1/3 = " << sum1_left << "\n";
    std::cout << "1/3 + 2 = " << sum1_right << "\n";
    std::cout << "Expected: " << expected1 << "\n";

    assert(sum1_left == expected1);
    assert(sum1_right == expected1);
    assert(sum1_left == sum1_right);
    std::cout << "Test 1 passed: integer + fraction symmetry\n";

    // Тест 2 и 3: 3.14 + Fraction(11, 100)
    Fraction f2(11, 100); // 0.11
    Fraction sum2 = 3.14f + f2; // float + Fraction

    std::cout << "3.14 + 11/100 = " << sum2 << "\n";
    std::cout << "As float: " << sum2.asFloat() << "\n";

    // Проверяем, что результат ≈ 3.25
    float resultAsFloat = sum2.asFloat();
    assert(floatEquals(resultAsFloat, 3.25f));
    std::cout << "Test 2-3 passed: float + fraction ≈ 3.25\n";

    std::cout << "\nAll tests passed!\n";

    return 0;
}
