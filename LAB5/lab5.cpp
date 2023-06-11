#include <iostream>
#include <vector>
#include <unordered_set>

// Абстрактный класс для структуры данных
class SetImplementation {
public:
    virtual void add(int value) = 0;
    virtual void remove(int value) = 0;
    virtual bool contains(int value) = 0;
};

// Конкретная реализация SetImplementation с использованием массива
class ArraySet : public SetImplementation {
private:
    std::vector<int> elements;

public:
    void add(int value) override {
        elements.push_back(value);
    }

    void remove(int value) override {
        for (auto it = elements.begin(); it != elements.end(); ++it) {
            if (*it == value) {
                elements.erase(it);
                break;
            }
        }
    }

    bool contains(int value) override {
        for (int element : elements) {
            if (element == value) {
                return true;
            }
        }
        return false;
    }
};

// Конкретная реализация SetImplementation с использованием хэш-таблицы
class HashSet : public SetImplementation {
private:
    std::unordered_set<int> elements;

public:
    void add(int value) override {
        elements.insert(value);
    }

    void remove(int value) override {
        elements.erase(value);
    }

    bool contains(int value) override {
        return elements.count(value) > 0;
    }
};

// Класс множество, использующий Bridge
class Set {
private:
    SetImplementation* implementation;

public:
    Set() {
        implementation = nullptr;
    }

    Set(int threshold) {
        if (threshold < 10) {
            implementation = new ArraySet();
        }
        else {
            implementation = new HashSet();
        }
    }

    ~Set() {
        delete implementation;
    }

    void add(int value) {
        implementation->add(value);
    }

    void remove(int value) {
        implementation->remove(value);
    }

    bool contains(int value) {
        return implementation->contains(value);
    }
};

// Пример использования
int main() {
    Set set(5); // Используем массив, если множество содержит не более 5 элементов
    set.add(10);
    set.add(5);
    set.add(3);

    std::cout << "Contains 5: " << (set.contains(5) ? "true" : "false") << std::endl; // true
    std::cout << "Contains 8: " << (set.contains(8) ? "true" : "false") << std::endl; // false

    set.remove(5);
    std::cout << "Contains 5: " << (set.contains(5) ? "true" : "false") << std::endl; // false

    Set largeSet(20); // Используем хэш-таблицу, если множество содержит более 20 элементов
    for (int i = 0; i < 30; ++i) {
        largeSet.add(i);
    }

    std::cout << "Contains 10: " << (largeSet.contains(10) ? "true" : "false") << std::endl; // true
    std::cout << "Contains 25: " << (largeSet.contains(25) ? "true" : "false") << std::endl; // true

    largeSet.remove(10);
    std::cout << "Contains 10: " << (largeSet.contains(10) ? "true" : "false") << std::endl; // false

    return 0;
}
