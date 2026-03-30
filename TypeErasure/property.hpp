#pragma once

#include "value.hpp"
#include <string>

/**
 * @brief Свойство объекта
 *
 * Связывает имя со значением. Может быть расширен метаданными:
 * - is_readonly — только для чтения
 * - unit — единица измерения
 * - description — описание
 * - min_value / max_value — ограничения
 * - и т.д.
 */
class Property {
    std::string name_;      ///< имя свойства
    Value value_;           ///< значение свойства

public:
    // ===== Конструкторы =========================================================================

    Property() = default;

    Property(const std::string& name, Value value)
        : name_(name), value_(std::move(value)) {}

    Property(std::string&& name, Value value)
        : name_(std::move(name)), value_(std::move(value)) {}

    // ===== Геттеры ==============================================================================

    const std::string& getName() const { return name_; }
    const Value& getValue() const { return value_; }
    Value& getValue() { return value_; }

    // ===== Сеттеры ==============================================================================

    void setName(const std::string& name) { name_ = name; }
    void setValue(const Value& value) { value_ = value; }
    void setValue(Value&& value) { value_ = std::move(value); }

    // ===== Делегирующие методы (удобство) =======================================================

    std::string to_str() const { return value_.to_str(); }
    bool test() const { return value_.test(); }
    bool from_str(const std::string& str) { return value_.from_str(str); }
    void print_possible() const { value_.print_possible(); }

    const char* type_name() const { return value_.type_name(); }
    bool empty() const { return value_.empty(); }

    // ===== Type-safe доступ =====================================================================

    template<typename T>
    T* get_if() { return value_.get_if<T>(); }

    template<typename T>
    const T* get_if() const { return value_.get_if<T>(); }
};
