#pragma once

#include "property.hpp"
#include <string>
#include <unordered_map>
#include <memory>
#include <iostream>
#include <vector>

/**
 * @brief Объект с динамическими свойствами
 *
 * Хранит коллекцию Property (имя + значение). Свойства могут
 * добавляться динамически в runtime.
 */
class Object {
    std::string class_type_;                               ///< тип объекта
    std::unordered_map<std::string, Property> properties_; ///< свойства по имени

public:
    // ===== Конструкторы =====

    Object() : class_type_("Unknown") {}

    explicit Object(const std::string& type) : class_type_(type) {}

    explicit Object(std::string&& type) : class_type_(std::move(type)) {}

    // ===== Управление свойствами =====

    /**
     * @brief Добавить или обновить свойство
     */
    void setProperty(const std::string& name, Value value) {
        properties_[name] = Property(name, std::move(value));
    }

    /**
     * @brief Добавить или обновить свойство
     */
    void setProperty(const std::string& name, const Property& prop) {
        properties_[name] = prop;
    }

    /**
     * @brief Добавить или обновить свойство (перемещением)
     */
    void setProperty(Property&& prop) {
        properties_[prop.getName()] = std::move(prop);
    }

    /**
     * @brief Получить свойство по имени
     * @return Указатель на Property или nullptr если не найдено
     */
    Property* getProperty(const std::string& name) {
        auto it = properties_.find(name);
        return it != properties_.end() ? &it->second : nullptr;
    }

    const Property* getProperty(const std::string& name) const {
        auto it = properties_.find(name);
        return it != properties_.end() ? &it->second : nullptr;
    }

    /**
     * @brief Оператор доступа к свойству
     */
    Property* operator()(const std::string& name) {
        return getProperty(name);
    }

    const Property* operator()(const std::string& name) const {
        return getProperty(name);
    }

    /**
     * @brief Проверить наличие свойства
     */
    bool hasProperty(const std::string& name) const {
        return properties_.find(name) != properties_.end();
    }

    /**
     * @brief Удалить свойство
     */
    bool removeProperty(const std::string& name) {
        return properties_.erase(name) > 0;
    }

    // ===== Получение информации =================================================================

    /**
     * @brief Получить тип объекта
     */
    const std::string& getType() const { return class_type_; }

    /**
     * @brief Установить тип объекта
     */
    void setType(const std::string& type) { class_type_ = type; }

    /**
     * @brief Получить все имена свойств
     */
    std::vector<std::string> getPropertyNames() const {
        std::vector<std::string> names;
        names.reserve(properties_.size());
        for (const auto& [name, _] : properties_) {
            names.push_back(name);
        }
        return names;
    }

    /**
     * @brief Получить количество свойств
     */
    size_t propertyCount() const { return properties_.size(); }

    // ===== Печать ===============================================================================

    /**
     * @brief Распечатать объект и все его свойства
     */
    void print() const {
        std::cout << "Object <" << class_type_ << ">\n";
        if (properties_.empty()) {
            std::cout << "  (no properties)\n";
        } else {
            for (const auto& [name, prop] : properties_) {
                std::cout << "  " << name << " (" << prop.type_name() << ") = "
                          << prop.to_str();
                if (!prop.test()) {
                    std::cout << " [INVALID]";
                }
                std::cout << "\n";
            }
        }
    }

    /**
     * @brief Распечатать только имена свойств
     */
    void printPropertyNames() const {
        std::cout << "Object <" << class_type_ << "> properties:\n";
        for (const auto& [name, _] : properties_) {
            std::cout << "  - " << name << "\n";
        }
    }

    // ===== Утилиты ==============================================================================

    /**
     * @brief Очистить все свойства
     */
    void clear() {
        properties_.clear();
    }
};
