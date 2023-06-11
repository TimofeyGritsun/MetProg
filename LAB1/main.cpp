#include <string>
#include <iostream>
#include <ctime>
#include <fstream>
#include <vector>
#include <chrono>

using namespace std;

string input = "inputFile.txt";
string output = "outputFile.txt";
string timeFile = "times.txt";
int amountD = 7;
int D[7] = { 100, 1000, 5000, 10000, 20000, 50000, 100000};

struct Flower {
    string name;
    string colour;
    string smell;
    string region;

    Flower(string name, string colour, string smell, string region)
    {
        this->name = name;
        this->colour = colour;
        this->smell = smell;
        this->region = region;
    }

    friend bool operator== (const Flower& a, const Flower& b) {
        if (a.smell == b.smell && a.name == b.name && a.colour == b.colour)
            return true;
        return false;
    }

    friend bool operator< (const Flower& a, const Flower& b) {
        if (a.name < b.name || ((a.smell < b.smell)
            && (a.name == b.name)) ||
            ((a.smell == b.smell)  && (a.colour < b.colour)
                && (a.name == b.name)))
            return true;
        return false;
    }

    friend bool operator<= (const Flower& a, const Flower& b) {
        if (a < b || a == b)
            return true;
        return false;
    }

    friend bool operator> (const Flower& a, const Flower& b) {
        if (!(a < b) && !(a == b))
            return true;
        return false;
    }

    friend bool operator>= (const Flower& a, const Flower& b) {
        if (!(a < b))
            return true;
        return false;
    }


    friend ostream& operator<<(ostream& os, const Flower& a) {
        os << a.name << ' ' << a.colour << ' ' << a.smell << '\n';
        return os;
    }
};


string makeName(const int len) {
    char symbols[] = "abcdefghijklmnopqrstuvwxyz";
    string tmp_s;
    tmp_s.reserve(len);
    for (int i = 0; i < len; ++i) {
        tmp_s += symbols[rand() % (sizeof(symbols) - 1)];
    }
    return tmp_s;
}

string makeRegion() {
    int a = rand() % 5;
    switch (a) {
    case(0):
        return "Europe";
    case (1):
        return "Africa";
    case (2):
        return "North America";
    case (3):
        return "South America";
    case (4):
        return "Australia";
    default:
        return "Europe";

    }
}

string makeSmell() {
    int a = rand() % 3;
    switch (a) {
    case(0):
        return "weak";
    case (1):
        return "medium";
    case (2):
        return "strong";
    default:
        return "weak";
    }
}

string makeColour()
{
    int a = rand() % 5;
    switch (a) {
    case(0):
        return "Red";
    case (1):
        return "Green";
    case (2):
        return "Blue";
    case (3):
        return "Yellow";
    case (4):
        return "White";
    default:
        return "Red";
    }
}

void makeTxtFile() {
    srand((unsigned)time(NULL));
    ofstream fout(input);

    for (int i = 0; i < amountD; ++i) {
        //Вывод числа записей
        fout << D[i] << "\n";
        for (int j = 0; j < D[i]; ++j) {
            //Вывод полей по порядку
            fout << makeName(10) << "\n";   //Имя
            fout << makeColour() << "\n";   //Цвет
            fout << makeSmell() << "\n";   //Запах
            fout << makeRegion() << "\n"; //Регион
        }
    }
}

vector<vector<Flower>> readInputFile() {
    ifstream fin(input);

    vector<vector<Flower>> result;

    string dim;
    string region, name;
    string colour;
    string smell;

    for (int i = 0; i < amountD; ++i) {
        
        getline(fin, dim);
        vector<Flower> v;
        for (int j = 0; j < D[i]; ++j) {
              
            getline(fin, name);
            getline(fin, colour);
            getline(fin, smell);
            getline(fin, region);
            Flower temp(name, colour, smell, region);
            v.push_back(temp);

        }
        result.push_back(v);
       
    }

    return result;
}

vector<Flower> selectSort(vector<Flower> vec) {
    vector<Flower> a = vec;

    int min = 0; // индекс минимального значения

    for (int i = 0; i < a.size(); i++)
    {
        min = i; // изначально min - номер текущей ячейки

        for (int j = i + 1; j < a.size(); j++)
            min = (a[j] < a[min]) ? j : min;

        // после нахождения минимального элемента, поменяем его местами с текущим

        std::swap(a[i], a[min]);
    }

    return a;
}


// Функция для разбиения массива и возврата индекса опорного элемента
int partition(vector<Flower>& vec, int low, int high) {
    vector<Flower>& arr = vec;
    Flower pivot = arr[high];  // выбираем последний элемент в качестве опорного
    int i = (low - 1);  // индекс меньшего элемента
    for (int j = low; j <= high - 1; j++) {
        // Если текущий элемент меньше или равен опорному
        if (arr[j] <= pivot) {
            i++;  // увеличиваем индекс меньшего элемента
            std::swap(arr[i], arr[j]);
        }
    }
    std::swap(arr[i + 1], arr[high]);
    return (i + 1);
}

// Функция быстрой сортировки
void quickSort(vector<Flower>& vec, int low, int high) {
    vector<Flower>& arr = vec;
    if (low < high) {
        // pi - индекс опорного элемента после разбиения
        int pi = partition(arr, low, high);

        // Рекурсивно сортируем элементы до разбиения и после разбиения
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}


// Погружение элемента arr[i] в под-кучу с корнем в вершине i.
// n - размер массива, i - индекс корня под-кучи.
void heapify(vector<Flower>& vec, int n, int i) {
    vector<Flower>& arr = vec;
    int largest = i; // Инициализируем наибольший элемент как корень
    int left = 2 * i + 1; // Левый потомок
    int right = 2 * i + 2; // Правый потомок

    // Если левый потомок больше корня
    if (left < n && arr[left] > arr[largest]) {
        largest = left;
    }

    // Если правый потомок больше, чем наибольший элемент на данный момент
    if (right < n && arr[right] > arr[largest]) {
        largest = right;
    }

    // Если наибольший элемент не является корнем
    if (largest != i) {
        swap(arr[i], arr[largest]);

        // Рекурсивно погружаем в под-кучу
        heapify(arr, n, largest);
    }
}

// Основная функция, реализующая алгоритм сортировки пирамидой
void heapSort(vector<Flower>& vec, int n) {
    vector<Flower>& arr = vec;
    // Построение пирамиды (перегруппировка массива)
    for (int i = n / 2 - 1; i >= 0; i--) {
        heapify(arr, n, i);
    }

    // Извлечение элементов из пирамиды по одному
    for (int i = n - 1; i >= 0; i--) {
        // Перемещаем текущий корень в конец
        swap(arr[0], arr[i]);

        // Вызываем процедуру погружения для уменьшенной пирамиды
        heapify(arr, i, 0);
    }
}


void writeOutput(string title, vector<vector<Flower>>& res) {
    ofstream fout(output, ios::app);

    fout << title << "\n";

    for (int i = 0; i < amountD; ++i) {
        fout << "Размер: " << res[i].size() << "\n";
        for (int j = 0; j < res[i].size(); ++j) {
            fout << res[i][j] << "\n";
        }
    }
}

void writeTime(string title, vector<std::chrono::steady_clock::time_point> time) {
    ofstream fout(timeFile, ios::app);

    fout << title << "\n";

    for (int i = 0; i < time.size() - 1; ++i)
        fout << "Время для сортировки " << D[i] << " значений: " << chrono::duration_cast<chrono::milliseconds>(time[i + 1] - time[i]).count() << " [ms]\n";

    fout << "\n";
}

int main()
{
    setlocale(LC_ALL, "Russian");
    makeTxtFile();

    vector<vector<Flower>> initialArray = readInputFile();
    vector<vector<Flower>> selectSortRes, quickSortRes, heapSortRes;

    vector<std::chrono::steady_clock::time_point> time;

    time.push_back(std::chrono::steady_clock::now());
    for (int i = 0; i < amountD; ++i) {
        selectSortRes.push_back(selectSort(initialArray[i]));
        time.push_back(std::chrono::steady_clock::now());
    }
    writeOutput("Сортировка выбором:\n", selectSortRes);
    writeTime("Сортировка выбором:\n", time);
    time.clear();

    time.push_back(std::chrono::steady_clock::now());
    for (int i = 0; i < amountD; ++i) {
        quickSort(initialArray[i], 0, (initialArray[i].size() - 1));
        quickSortRes.push_back(initialArray[i]);
        time.push_back(std::chrono::steady_clock::now());
    }
    writeOutput("Быстрая сортировка:\n", quickSortRes);
    writeTime("Быстрая сортировка:\n", time);
    time.clear();

    time.push_back(std::chrono::steady_clock::now());
    for (int i = 0; i < amountD; ++i) {
        heapSort(initialArray[i], initialArray[i].size());
        heapSortRes.push_back(initialArray[i]);
        time.push_back(std::chrono::steady_clock::now());
    }
    writeOutput("Сортировка пирамидой:\n", heapSortRes);
    writeTime("Сортировка пирамидой:\n", time);
    time.clear();
  
    return 0;
}
