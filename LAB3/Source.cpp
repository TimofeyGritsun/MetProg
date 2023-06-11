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

ofstream fout(output);

const int amountD = 7;
const int power = 400000;
const int maxLenght = 2000;
const int D[7] = { 100, 1000, 5000, 10000, 20000, 50000, 100000};
const int p = 43;
vector<long long> p_pow(maxLenght);

unsigned int naiveHash(string key) {
    unsigned int result = 1;

    for (int i = 0; i < key.length(); ++i)
        result = (result * (key[i] - 'x' + 1)) % power;

    return result % power;
}
unsigned int complicatedHash(string key) {
    unsigned long long hash = 0;
    for (size_t j = 0; j < key.length(); ++j)
        hash = (hash + (key[j] - 'x' + 1) * p_pow[j]);

    return hash % power;
}


string makeName(const int len) {
    static const char symbols[] = "abcdefghijklmnopqrstuvwxyz";
    string temp;
    temp.reserve(len);
    for (int i = 0; i < len; ++i) {
        temp += symbols[rand() % (sizeof(symbols) - 1)];
    }
    return temp;
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
    unsigned int (*hashFunc)(string);
    unsigned int hash;

    Flower(string name, string colour, string smell, string region, unsigned int (*hashFunc)(string))
    {
        this->name = name;
        this->colour = colour;
        this->smell = smell;
        this->region = region;
        this->hashFunc = hashFunc;
        this->hash = hashFunc(name);
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

    string getName() const {
        return this->name;
    }

    unsigned int getHash() const {
        return this->hash;
    }
};

vector<vector<Flower>> readTxtFile(unsigned int (*hashFunc)(string)) {
    ifstream fin(input);

    vector<vector<Flower>> result;

    string dim;
    string name, colour, smell, region;

    for (int i = 0; i < amountD; ++i) {

        getline(fin, dim);
        vector<Flower> v;
        for (int j = 0; j < D[i]; ++j) {
          
            getline(fin, name);
            getline(fin, colour);
            getline(fin, smell);
            getline(fin, region);
            Flower temp(name, colour, smell, region, hashFunc);
            v.push_back(temp);
        }
        result.push_back(v);
    }

    return result;
}

void writeTime(string title, std::chrono::steady_clock::time_point start, std::chrono::steady_clock::time_point end, int divideBy) {
    fout << title;

    fout << chrono::duration_cast<chrono::microseconds>(end - start).count() << " [микросекунд]\n";
}

struct HashItem {
    vector<string> values;
};

class HashTable {
    vector<HashItem> table;

public:
    HashTable() {
        table.resize(power);
    }

    void insert(Flower& item) {
 
        HashItem& currentItem = this->table[item.getHash()];
        for (auto i : currentItem.values)
            if (i == item.getName())
                return;

        currentItem.values.push_back(item.getName());
    }

    vector<string> find(Flower& item) {
        HashItem& currentItem = this->table[item.getHash()];

        return currentItem.values;
    }

    void clear() {
        this->table.clear();
        this->table.resize(power);
    }


    unsigned int getCollisions() {
        unsigned int result = 0;

        for (auto n : this->table)
            if (n.values.size() > 1)
                result += n.values.size() - 1;

        return result;
    }
};


double randomDouble(double fMin, double fMax)
{
    double f = (double)rand();
    return fMin + f * (fMax - fMin);
}

void makeInputFile() {
    srand((unsigned)time(NULL));
    ofstream fout(input);

    for (int i = 0; i < amountD; ++i) {
        
        fout << D[i] << "\n";
        for (int j = 0; j < D[i]; ++j) {
            
            fout << makeName(10) << "\n";   
            fout << makeColour() << "\n";  
            fout << makeSmell() << "\n";   
            fout << makeRegion() << "\n"; 
        }
    }
}

int main()
{
    setlocale(LC_ALL, "Russian");

    makeInputFile();
    
    p_pow[0] = 1;
    for (size_t i = 1; i < p_pow.size(); ++i)
        p_pow[i] = p_pow[i - 1] * p;

    vector<vector<Flower>> naiveArray = readTxtFile(naiveHash);

    HashTable naiveTable;

    std::chrono::steady_clock::time_point start, end;

    fout << "Простая хэш функция:\n" << '\n';

    start = std::chrono::steady_clock::now();

    for (int i = 0; i < amountD; ++i) {

        for (int j = 0; j < naiveArray[i].size(); ++j)
            naiveTable.insert(naiveArray[i][j]);

        for (int j = 0; j < naiveArray[i].size(); ++j) {
            Flower objectToFind = naiveArray[i][j];
            vector<string> foundItems = naiveTable.find(objectToFind);
        }

        end = std::chrono::steady_clock::now();
        fout << "\"Реализация таблицы на " + to_string(naiveArray[i].size()) + " элементах:\n";
        writeTime("Среднее время поиска: ", start, end, naiveArray[i].size());
        fout << "Коллизий: " << naiveTable.getCollisions() << "\n\n";
        naiveTable.clear();
        start = end;
    }

    naiveArray.clear();

    vector<vector<Flower>> complicatedArray = readTxtFile(complicatedHash);

    HashTable complicatedTable;

    fout << "Сложная хэш функция:\n" << '\n';

    start = std::chrono::steady_clock::now();

    for (int i = 0; i < amountD; ++i) {

        for (int j = 0; j < complicatedArray[i].size(); ++j)
            complicatedTable.insert(complicatedArray[i][j]);

        for (int j = 0; j < complicatedArray[i].size(); ++j) {
            Flower objectToFind = complicatedArray[i][j];
            vector<string> foundItems = complicatedTable.find(objectToFind);
        }

        end = std::chrono::steady_clock::now();
        fout << "\"Реализация таблицы на " + to_string(complicatedArray[i].size()) + " элементах:\n";
        writeTime("Среднее время поиска: ", start, end, complicatedArray[i].size());
        fout << "Коллизий: " << complicatedTable.getCollisions() << "\n\n";
        complicatedTable.clear();
        start = end;
    }

    return 0;
}
