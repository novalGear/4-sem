#pragma once

#include "property.hpp"

#include <string>
#include <iostream>
#include <unordered_map>
#include <memory>

/**
 * @brief Названия классов
 * @details Используется в Object - отражает по шаблону
 *          для какого класса построен Object
 */
enum class CLASS_TYPE {
    // добавить нужные типы
    UNKNOWN
};

class Object {
private:
    std::string class_type;                                    ///< по шаблону какого класса построен объект
    // std::vector<std::unique_ptr<Property>> properties_;     ///< список указателей на свойства
    //
    std::unordered_map<std::string, std::unique_ptr<Property>> properties_;

public:
    /**
     * @brief Распечатать имена всех свойств
     */
     void print_properties() {
        std::cout << "Object <" << class_type << "> properties:\n";
        // Исправлено: другой синтаксис for для map
        for (const auto& [name, prop] : properties_) {
            std::cout << name << "\n";
        }
    };

    /**
     * @brief Получить указатель на свойство по имени
     * @note пример: my_object("parent")
     */
    Property* operator()(const std::string& property_name) {
        auto it = properties_.find(property_name);
        if (it != properties_.end()) {
            return it->second.get();
        }
        return nullptr;
    }
}
