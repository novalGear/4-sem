#pragma once
#include <iosfwd>

class Fraction
{
private:
    int num_;
    int denom_;  // не unsigned чтобы отлавливать некорректные инициализации отрицательными числами


public:

    // Возвращает данную дробь с наименьшим общим знаменателем

    Fraction reduced( void ) const;
    Fraction(void): num_(0), denom_(1) {}
    Fraction(int decimal): num_(decimal), denom_(1) {}
    Fraction (int anom, int aDenom);
    Fraction (float x);
    ~Fraction(void) = default;

    // проверяет корректность дроби (возможна ли такая дробь)
    bool is_ok() const;

    // Методы доступа (inline! )
    int numerator( void )   const { return num_; }
    int denominator( void ) const { return denom_; }

    // Методы преобразования
    // Преобразует данную дробь в число с плавающей точкой
    float asFloat( void ) const;
    // Методы печати
    // Печатает данную дробь в формате num/denom
    void print( void ) const;
};

Fraction operator+(const Fraction& a, const Fraction& b);
Fraction operator-(const Fraction& a, const Fraction& b);
Fraction operator*(const Fraction& a, const Fraction& b);
Fraction operator/(const Fraction& a, const Fraction& b);

bool operator==(const Fraction& a, const Fraction& b);
bool operator<(const Fraction& a, const Fraction& b);
bool operator>(const Fraction& a, const Fraction& b);

std::ostream& operator<<(std::ostream& os, const Fraction& f);
