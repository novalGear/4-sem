#pragma once

#include "vtable.hpp"

#include <iostream>
#include <string>
#include <utility>
#include <type_traits>

/**
 * @brief Type-erased значение с ручным vtable
 *
 * Хранит указатель на данные (владеет) и указатель на vtable.
 * Все операции делегируются в vtable.
 */
class Value {
private:
    void* data_;                 // указатель на данные (владеет)
    const ValueVTable* vptr_;    // указатель на таблицу функций

public:
    // ===== Конструкторы =========================================================================

    /**
     * @brief Пустое значение
     */
    Value() : data_(nullptr), vptr_(nullptr) {}

    /**
     * @brief Конструктор от значения (тип выводится автоматически)
     * @tparam T Тип значения
     * @param value Значение для хранения
     */
    template<typename T>
    explicit Value(T value)
        : data_(new T(std::move(value)))
        , vptr_(&vtable_holder<T>::vtable) {}       // сохраняем vtable

    /**
     * @brief Конструктор с явным указанием vtable (для runtime-типов)
     * @param data Указатель на данные (Value становится владельцем)
     * @param vtable Указатель на таблицу функций
     */
    Value(void* data, const ValueVTable* vtable)
        : data_(data), vptr_(vtable) {}

    /**
     * @brief Конструктор копирования (глубокое копирование)
     */
    Value(const Value& other)
        : data_(other.vptr_ && other.vptr_->clone ? other.vptr_->clone(other.data_) : nullptr)
        , vptr_(other.vptr_) {}

    /**
     * @brief Конструктор перемещения
     */
    Value(Value&& other) noexcept
        : data_(other.data_)
        , vptr_(other.vptr_) {
        other.data_ = nullptr;
        other.vptr_ = nullptr;
    }

    /**
     * @brief Деструктор
     */
    ~Value() {
        if (data_ && vptr_ && vptr_->destroy) {
            vptr_->destroy(data_);
        }
    }

    // ===== Операторы присваивания ===============================================================

    /**
     * @brief Копирующее присваивание
     */
    Value& operator=(const Value& other) {
        if (this != &other) {
            // Освобождаем старые данные
            if (data_ && vptr_ && vptr_->destroy) {
                vptr_->destroy(data_);
            }
            // Копируем новые
            data_ = other.vptr_ && other.vptr_->clone ? other.vptr_->clone(other.data_) : nullptr;
            vptr_ = other.vptr_;
        }
        return *this;
    }

    /**
     * @brief Перемещающее присваивание
     */
    Value& operator=(Value&& other) noexcept {
        if (this != &other) {
            if (data_ && vptr_ && vptr_->destroy) {
                vptr_->destroy(data_);
            }
            data_ = other.data_;
            vptr_ = other.vptr_;
            other.data_ = nullptr;
            other.vptr_ = nullptr;
        }
        return *this;
    }

    // ===== Операции =============================================================================

    /**
     * @brief Преобразовать значение в строку
     */
    std::string to_str() const {
        if (vptr_ && vptr_->to_str && data_) {
            return vptr_->to_str(data_);
        }
        return "";
    }

    /**
     * @brief Проверить валидность значения
     */
    bool test() const {
        if (vptr_ && vptr_->test && data_) {
            return vptr_->test(data_);
        }
        return false;
    }

    /**
     * @brief Установить значение из строки
     * @param str Строковое представление значения
     * @return true если парсинг успешен
     */
    bool from_str(const std::string& str) {
        if (vptr_ && vptr_->from_str && data_) {
            return vptr_->from_str(data_, str);
        }
        return false;
    }

    /**
     * @brief Распечатать все возможные значения для этого типа
     */
    void print_possible() const {
        if (vptr_ && vptr_->print_possible) {
            vptr_->print_possible();
        } else {
            std::cout << "unknown type\n";
        }
    }

    // ===== Информация о состоянии ===============================================================

    /**
     * @brief Проверить, пустое ли значение
     */
    bool empty() const { return data_ == nullptr; }

    /**
     * @brief Получить имя типа
     */
    const char* type_name() const {
        return vptr_ ? vptr_->type_name : "empty";
    }

    /**
     * @brief Получить указатель на vtable (для низкоуровневого доступа)
     */
    const ValueVTable* vtable() const { return vptr_; }

    // ===== Type-safe доступ =====================================================================

    /**
     * @brief Получить указатель на данные с проверкой типа
     * @tparam T Ожидаемый тип
     * @return Указатель на данные или nullptr если тип не совпадает
     */
    template<typename T>
    T* get_if() {
        if (!data_ || !vptr_) return nullptr;
        if (vptr_ == &vtable_holder<T>::vtable) {
            return static_cast<T*>(data_);
        }
        return nullptr;
    }

    template<typename T>
    const T* get_if() const {
        return const_cast<Value*>(this)->get_if<T>();
    }

    /**
     * @brief Получить значение с приведением (без проверки!)
     * @warning Используйте только если уверены в типе!
     */
    template<typename T>
    T& as() { return *static_cast<T*>(data_); }

    template<typename T>
    const T& as() const { return *static_cast<const T*>(data_); }
};
