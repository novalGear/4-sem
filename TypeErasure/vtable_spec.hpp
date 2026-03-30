#pragma once

#include "vtable.hpp"
#include <iostream>
#include <string>

// ============================================================================
// Специализация для int
// ============================================================================

template<>
struct vtable_holder<int> {
    static std::string to_str(const void* ptr) {
        return std::to_string(*static_cast<const int*>(ptr));
    }

    static bool test(const void* ptr) {
        return true;  // все int валидны
    }

    static bool from_str(void* ptr, const std::string& str) {
        try {
            *static_cast<int*>(ptr) = std::stoi(str);
            return true;
        } catch (...) {
            return false;
        }
    }

    static void print_possible() {
        std::cout << "Possible values: any 32-bit integer\n";
    }

    static void* clone(const void* ptr) {
        return new int(*static_cast<const int*>(ptr));
    }

    static void destroy(void* ptr) {
        delete static_cast<int*>(ptr);
    }

    static const ValueVTable vtable;
};

const ValueVTable vtable_holder<int>::vtable = {
    &vtable_holder<int>::to_str,
    &vtable_holder<int>::test,
    &vtable_holder<int>::from_str,
    &vtable_holder<int>::print_possible,
    &vtable_holder<int>::clone,
    &vtable_holder<int>::destroy,
    "int"
};

// ============================================================================
// Специализация для std::string
// ============================================================================

template<>
struct vtable_holder<std::string> {
    static std::string to_str(const void* ptr) {
        return *static_cast<const std::string*>(ptr);
    }

    static bool test(const void* ptr) {
        const std::string* s = static_cast<const std::string*>(ptr);
        return !s->empty();
    }

    static bool from_str(void* ptr, const std::string& str) {
        *static_cast<std::string*>(ptr) = str;
        return true;
    }

    static void print_possible() {
        std::cout << "Possible values: any string (non-empty is valid)\n";
    }

    static void* clone(const void* ptr) {
        return new std::string(*static_cast<const std::string*>(ptr));
    }

    static void destroy(void* ptr) {
        delete static_cast<std::string*>(ptr);
    }

    static const ValueVTable vtable;
};

const ValueVTable vtable_holder<std::string>::vtable = {
    &vtable_holder<std::string>::to_str,
    &vtable_holder<std::string>::test,
    &vtable_holder<std::string>::from_str,
    &vtable_holder<std::string>::print_possible,
    &vtable_holder<std::string>::clone,
    &vtable_holder<std::string>::destroy,
    "string"
};

// ============================================================================
// Специализация для bool
// ============================================================================

template<>
struct vtable_holder<bool> {
    static std::string to_str(const void* ptr) {
        bool value = *static_cast<const bool*>(ptr);
        return value ? "true" : "false";
    }

    static bool test(const void* ptr) {
        return true;
    }

    static bool from_str(void* ptr, const std::string& str) {
        bool* value = static_cast<bool*>(ptr);
        if (str == "true" || str == "1" || str == "yes" || str == "on") {
            *value = true;
            return true;
        } else if (str == "false" || str == "0" || str == "no" || str == "off") {
            *value = false;
            return true;
        }
        return false;
    }

    static void print_possible() {
        std::cout << "Possible values: true/false, 1/0, yes/no, on/off\n";
    }

    static void* clone(const void* ptr) {
        return new bool(*static_cast<const bool*>(ptr));
    }

    static void destroy(void* ptr) {
        delete static_cast<bool*>(ptr);
    }

    static const ValueVTable vtable;
};

const ValueVTable vtable_holder<bool>::vtable = {
    &vtable_holder<bool>::to_str,
    &vtable_holder<bool>::test,
    &vtable_holder<bool>::from_str,
    &vtable_holder<bool>::print_possible,
    &vtable_holder<bool>::clone,
    &vtable_holder<bool>::destroy,
    "bool"
};

// ============================================================================
// Специализация для double
// ============================================================================

template<>
struct vtable_holder<double> {
    static std::string to_str(const void* ptr) {
        return std::to_string(*static_cast<const double*>(ptr));
    }

    static bool test(const void* ptr) {
        return true;
    }

    static bool from_str(void* ptr, const std::string& str) {
        try {
            *static_cast<double*>(ptr) = std::stod(str);
            return true;
        } catch (...) {
            return false;
        }
    }

    static void print_possible() {
        std::cout << "Possible values: any double\n";
    }

    static void* clone(const void* ptr) {
        return new double(*static_cast<const double*>(ptr));
    }

    static void destroy(void* ptr) {
        delete static_cast<double*>(ptr);
    }

    static const ValueVTable vtable;
};

const ValueVTable vtable_holder<double>::vtable = {
    &vtable_holder<double>::to_str,
    &vtable_holder<double>::test,
    &vtable_holder<double>::from_str,
    &vtable_holder<double>::print_possible,
    &vtable_holder<double>::clone,
    &vtable_holder<double>::destroy,
    "double"
};
