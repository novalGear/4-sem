#pragma once

class Property {
    std::string name_;          ///< Имя свойства
    Value       value_;         ///< Значение свойства

    // Геттеры
    Value       get_value();
    std::string get_name();
}
