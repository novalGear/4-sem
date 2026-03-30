# Type Erasure

https://github.com/boostcon/cppnow_presentations_2024/tree/main

Type Erasure: техника программирования, удаление explicit информации о типе из программы. Это абстракция, которая обеспечивает независимость программы от типов данных (explicitly).

## Зачем использовать?
### Как паттерн дизайна: разделение интерфейса и его реализации
- код ожидает определенное поведение
- Код написан в условиях абстракции которая предоставляет данное поведение
- Многие типы могут реализовывать такое поведение
- Удалим все свойства, не являющиеся существенными для этого поведения

В отличие от наследования (external polymorphism), type erasure - non-intrusive

### Decoupling dependancies
Если реализация не опирается на какой-либо тип внешне (explicitly), нет зависимости между исходниками => не нужно перекомпилировать проект при изменении части реализации.

## Implementation

Есть три основных способа реализации *Type Erasure*:
- Inheritance
- Static functions
- VTable

Сначала рассмотрим простой пример.

### Базовый пример: C qsort
```cpp
void qsort(void* base, size_t nmemb, size_t size, int (*compare)(const void*, const void*));

int less(const void* a, const void* b) {
    return *(const int*)a - * (const int*)b;
}

int a[10] = {1, 10, 2, 9, 8, 3, 4, 7, 5, 0};
qsort(a, 10, sizeof(int), less);
```
- Информация о типе явно используется при вызове в первый и последний раз.

- Функция less восстанавливает информацию о типе (*type reification*), но её интерфейс не типо-специфичный (*type agnostic*)

В C++ *Type Reification* автоматизировано и корректно по построению (при генерации) или выдаст Compile Error.

### Inheritance
Type erasure через inheritance — это способ реализовать runtime-полиморфизм для произвольных типов, не требуя от них явного наследования.

**Essence**
Constructor is a template and
1.  Deduces the type to be erased (or *Deleter* for smart pointers)
2.  Creates a derived object that:
   2.1 The Function body uses erased type and correct by construction
   2.2 The interface of the function is type-agnostic
3. The derived object is accessed by base-pointer
4. If a default value for erased type is allowed, the base pointer is default-initialized with the default action

Copying: the destroy hierarchy needs a virtual clone() function
Moving: transfer the deleter to the new object

In general, need another virtual function to compare deleters


### Static Functions



### VTable



## Sources:

[C++ Type Erasure Demystified - Fedor G Pikus - C++ Now 2024] (https://www.youtube.com/watch?v=p-qaf6OS_f4)
