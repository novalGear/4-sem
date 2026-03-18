#pragma once

#include <string>

enum class VALUE_TYPE {
    UNKNOWN = -1;
    RELATIVE,
    ROLE,
};

/**
 * @brief Таблица виртуальных функций для конкретного типа
 * @note Для каждого нового типа нужно реализовать функции из ValueVTable
 *       и добавить структуру с указателями на них в Value
 */
struct ValueVTable {
    std::function<bool>         test;
    std::function<std::string>  to_str;
    std::function<void>         from_str;
    std::function<void>         print_values;

    ValueVTable(
        decltype(test)          t,
        decltype(to_str)        ts,
        decltype(from_str)      fs,
        decltype(print_values)  p,
    ): test(t), to_str(ts), from_str(fs), print_values(p) {}
};

class Value {
    size_t id_;              ///< уникальный идентификатор value
    VALUE_TYPE type_;        ///< тип value

    /**
     * @brief Таблица виртуальных функций
     * @details vtable[type] хранит указатели на реализации
     * функций обработчиков для данного типа значения.
     *
     * Таблица общая для всех экземпляров класса (shared)
     */
    std::shared_ptr<ValueVTable> vtable_;

    /**
     * @brief Протестировать валидность значения
     */
    bool test() const {
        return vtable_[static_cast<size_t>(type)]->test();
    }

    /**
     * @brief Перевести значение в уникальную строку
     */
    std::string to_str() const {
        return vtable_[static_cast<size_t>(type)]->to_str();
    };

    /**
     * @brief Прочитать значение из строки
     * @note После использования нужно проверить валидность с помощью test
     */
    void from_str(std::string s) {
        return vtable_[static_cast<size_t>(type)]->from_str(s);
    }

    /**
     * @brief Распечатать все возможные варианты значений VALUE
     */
    void print() {
        return vtable_[static_cast<size_t>(type)]->print();
    }
};
