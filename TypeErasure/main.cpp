#include "object.hpp"
#include "vtable_spec.hpp"  // специализации для int, string, bool
#include <iostream>

int main() {
    std::cout << "=== Type Erasure with VTable ===\n\n";

    // Создаем объект
    Object player("Player");

    // Добавляем свойства
    player.setProperty("health", Value(100));
    player.setProperty("name", Value(std::string("Hero")));
    player.setProperty("is_alive", Value(true));

    // Печатаем объект
    player.print();
    std::cout << "\n";

    // Работа со свойствами
    auto* health = player.getProperty("health");
    if (health) {
        std::cout << "Health: " << health->to_str() << "\n";

        // Изменяем значение
        health->from_str("75");
        std::cout << "After damage: " << health->to_str() << "\n";

        // Проверка валидности
        std::cout << "Valid: " << (health->test() ? "yes" : "no") << "\n\n";
    }

    // Добавляем новое свойство
    player.setProperty("position_x", Value(10.5));
    player.setProperty("position_y", Value(20.3));

    // Печатаем только имена свойств
    player.printPropertyNames();
    std::cout << "\n";

    // Type-safe доступ
    auto* name_prop = player.getProperty("name");
    if (name_prop) {
        std::string* name = name_prop->get_if<std::string>();
        if (name) {
            std::cout << "Type-safe name: " << *name << "\n";
        }
    }

    // Проверка типов
    auto* int_prop = player.getProperty("health");
    if (int_prop && int_prop->get_if<int>()) {
        std::cout << "Health is int (correct)\n";
    }

    return 0;
}
