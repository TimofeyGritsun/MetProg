#include <iostream>
#include <vector>
#include <unordered_set>

// ����������� ����� ��� ��������� ������
class SetImplementation {
public:
    virtual void add(int value) = 0;
    virtual void remove(int value) = 0;
    virtual bool contains(int value) = 0;
};

// ���������� ���������� SetImplementation � �������������� �������
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

// ���������� ���������� SetImplementation � �������������� ���-�������
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

// ����� ���������, ������������ Bridge
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

// ������ �������������
int main() {
    Set set(5); // ���������� ������, ���� ��������� �������� �� ����� 5 ���������
    set.add(10);
    set.add(5);
    set.add(3);

    std::cout << "Contains 5: " << (set.contains(5) ? "true" : "false") << std::endl; // true
    std::cout << "Contains 8: " << (set.contains(8) ? "true" : "false") << std::endl; // false

    set.remove(5);
    std::cout << "Contains 5: " << (set.contains(5) ? "true" : "false") << std::endl; // false

    Set largeSet(20); // ���������� ���-�������, ���� ��������� �������� ����� 20 ���������
    for (int i = 0; i < 30; ++i) {
        largeSet.add(i);
    }

    std::cout << "Contains 10: " << (largeSet.contains(10) ? "true" : "false") << std::endl; // true
    std::cout << "Contains 25: " << (largeSet.contains(25) ? "true" : "false") << std::endl; // true

    largeSet.remove(10);
    std::cout << "Contains 10: " << (largeSet.contains(10) ? "true" : "false") << std::endl; // false

    return 0;
}
