#include <iostream>
#include <ctime>
#include <fstream>
#include <vector>
#include <chrono>
#include <algorithm>
#include <string>
#include <map>

using namespace std;

string input = "inputFile.txt";
string output = "outputFile.txt";
int amountD = 7;
int D[7] = { 100, 1000, 5000, 10000, 20000, 50000, 100000 };

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
    Flower()
    {
        this->name = makeName(10);
        this->colour = makeColour();
        this->smell = makeSmell();
        this->region = makeRegion();
    }

    friend bool operator== (const Flower& a, const Flower& b) {
        if (a.smell == b.smell && a.name == b.name && a.colour == b.colour)
            return true;
        return false;
    }

    friend bool operator< (const Flower& a, const Flower& b) {
        if (a.name < b.name || ((a.smell < b.smell)
            && (a.name == b.name)) ||
            ((a.smell == b.smell) && (a.colour < b.colour)
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

    friend ostream& operator<<(ostream& os, string a) {
        os << a;
        return os;
    }

    string getName() const {
        return this->name;
    }
};

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

void writeTime(string title, std::chrono::steady_clock::time_point start, std::chrono::steady_clock::time_point end) {
    ofstream fout(output, ios::app);

    fout << title << endl;

    fout << chrono::duration_cast<chrono::microseconds>(end - start).count() << " [микросекунд]\n\n";
}

vector<int> straightSearch(vector<Flower>& v, string key) {
    vector<int> res;

    for (int i = 0; i < v.size(); ++i)
        if (v[i].getName() == key)
            res.push_back(i);

    return res;
}

int getLowerBound(vector<Flower>& v, string key) {
    int mid = 0, left = 0, right = v.size();
    while (1)
    {
        mid = (left + right) / 2;

        if (mid < 0 || mid >= v.size())
            return -1;

        if (key <= v[mid].getName())
            right = mid - 1;
        else if (key > v[mid].getName() && mid + 1 < v.size() && key == v[mid + 1].getName())
            return mid;
        else if (key > v[mid].getName())
            left = mid + 1;

        if (left > right)
            return -1;
    }
}

int getUpperBound(vector<Flower>& v, string key) {
    int mid = 0, left = 0, right = v.size();
    while (1)
    {
        mid = (left + right) / 2;

        if (mid < 0 || mid >= v.size())
            return -1;

        if (key < v[mid].getName() && mid - 1 >= 0 && key == v[mid - 1].getName())
            return mid;
        else if (key < v[mid].getName())
            right = mid - 1;
        else if (key >= v[mid].getName())
            left = mid + 1;

        if (left > right)
            return -1;
    }
}

vector<Flower> binarySearch(vector<Flower>& v, string key) {
    int left = getLowerBound(v, key);
    int right = getUpperBound(v, key);

    if (left == -1 || right == -1)
        return vector<Flower>();

    auto first = v.cbegin() + left + 1;
    auto last = v.cbegin() + right;

    vector<Flower> res(first, last);

    return res;
}

bool comp(Flower a, Flower b) { return (a.getName() < b.getName()); }

int main()
{
    setlocale(LC_ALL, "Russian");

    vector<vector<Flower>> initialArray = readInputFile();

    vector<string> selectedKeys = { "pxjerqfiia", "0", "oqswlkyhcx", "lbpscnmzft", "agileqwjqf", "fikkklmhvx", "wberddjlrm"};

    std::chrono::steady_clock::time_point start, end;
    Flower foundValue;
    ofstream fout(output, ios::app);

    fout << "Прямой поиск:\n\n";

    start = std::chrono::steady_clock::now();

    for (int i = 0; i < amountD; ++i) {
        vector<int> temp = straightSearch(initialArray[i], selectedKeys[i]);
        end = std::chrono::steady_clock::now();
        fout << "Искался ключ: " << selectedKeys[i] << endl;
        if (!temp.empty())
            for (int j = 0; j < temp.size(); ++j)
                fout << "Найден элемент: " << initialArray[i][temp[j]] << endl;
        writeTime("На прямой поиск с " + to_string(initialArray[i].size()) + " значениями ушло: ", start, end);
        start = end;
    }

    fout << "Сортировка:" << endl << endl;

    start = std::chrono::steady_clock::now();

    for (int i = 0; i < amountD; ++i) {
        std::sort(initialArray[i].begin(), initialArray[i].end(), comp);
        binarySearch(initialArray[i], selectedKeys[i]);
        end = std::chrono::steady_clock::now();
        writeTime("На сортировку и двоичный поиск " + to_string(initialArray[i].size()) + " значениий ушло: ", start, end);
        start = end;
    }

    fout << "Бинарный поиск в отсортированном массиве:" << endl;

    start = std::chrono::steady_clock::now();

    for (int i = 0; i < amountD; ++i) {
        vector<Flower> temp = binarySearch(initialArray[i], selectedKeys[i]);
        end = std::chrono::steady_clock::now();
        fout << "Искался ключ: " << selectedKeys[i] << "\n";
        if (!temp.empty())
            for (int j = 0; j < temp.size(); ++j)
                fout << "Найден элемент: " << temp[j];
        writeTime("На бинарный поиск с " + to_string(initialArray[i].size()) + " значениями ушло: ", start, end);
        start = end;
    }

    vector<multimap<string, Flower>> m;

    for (int i = 0; i < amountD; ++i) {
        multimap<string, Flower> temp;
        for (int j = 0; j < initialArray[i].size(); ++j)
            temp.insert({ initialArray[i][j].getName(), initialArray[i][j] });
        m.push_back(temp);
    }

    initialArray.clear();

    fout << "Поиск в ассоциативном массиве:\n\n";

    start = std::chrono::steady_clock::now();

    for (int i = 0; i < amountD; ++i) {
        auto bounds = m[i].equal_range(selectedKeys[i]);
        end = std::chrono::steady_clock::now();
        fout << "Искался ключ: " << selectedKeys[i] << "\n";
        if (bounds.first != m[i].end())
            for (auto it = bounds.first; it != bounds.second; ++it)
                fout << "Найден элемент: " << it->second;
        writeTime("На поиск в ассоциативном массиве с " + to_string(m[i].size()) + " значениями ушло: ", start, end);
        start = end;
    }

    return 0;
}
