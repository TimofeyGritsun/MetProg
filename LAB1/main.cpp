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
        //����� ����� �������
        fout << D[i] << "\n";
        for (int j = 0; j < D[i]; ++j) {
            //����� ����� �� �������
            fout << makeName(10) << "\n";   //���
            fout << makeColour() << "\n";   //����
            fout << makeSmell() << "\n";   //�����
            fout << makeRegion() << "\n"; //������
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

    int min = 0; // ������ ������������ ��������

    for (int i = 0; i < a.size(); i++)
    {
        min = i; // ���������� min - ����� ������� ������

        for (int j = i + 1; j < a.size(); j++)
            min = (a[j] < a[min]) ? j : min;

        // ����� ���������� ������������ ��������, �������� ��� ������� � �������

        std::swap(a[i], a[min]);
    }

    return a;
}


// ������� ��� ��������� ������� � �������� ������� �������� ��������
int partition(vector<Flower>& vec, int low, int high) {
    vector<Flower>& arr = vec;
    Flower pivot = arr[high];  // �������� ��������� ������� � �������� ��������
    int i = (low - 1);  // ������ �������� ��������
    for (int j = low; j <= high - 1; j++) {
        // ���� ������� ������� ������ ��� ����� ��������
        if (arr[j] <= pivot) {
            i++;  // ����������� ������ �������� ��������
            std::swap(arr[i], arr[j]);
        }
    }
    std::swap(arr[i + 1], arr[high]);
    return (i + 1);
}

// ������� ������� ����������
void quickSort(vector<Flower>& vec, int low, int high) {
    vector<Flower>& arr = vec;
    if (low < high) {
        // pi - ������ �������� �������� ����� ���������
        int pi = partition(arr, low, high);

        // ���������� ��������� �������� �� ��������� � ����� ���������
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}


// ���������� �������� arr[i] � ���-���� � ������ � ������� i.
// n - ������ �������, i - ������ ����� ���-����.
void heapify(vector<Flower>& vec, int n, int i) {
    vector<Flower>& arr = vec;
    int largest = i; // �������������� ���������� ������� ��� ������
    int left = 2 * i + 1; // ����� �������
    int right = 2 * i + 2; // ������ �������

    // ���� ����� ������� ������ �����
    if (left < n && arr[left] > arr[largest]) {
        largest = left;
    }

    // ���� ������ ������� ������, ��� ���������� ������� �� ������ ������
    if (right < n && arr[right] > arr[largest]) {
        largest = right;
    }

    // ���� ���������� ������� �� �������� ������
    if (largest != i) {
        swap(arr[i], arr[largest]);

        // ���������� ��������� � ���-����
        heapify(arr, n, largest);
    }
}

// �������� �������, ����������� �������� ���������� ���������
void heapSort(vector<Flower>& vec, int n) {
    vector<Flower>& arr = vec;
    // ���������� �������� (��������������� �������)
    for (int i = n / 2 - 1; i >= 0; i--) {
        heapify(arr, n, i);
    }

    // ���������� ��������� �� �������� �� ������
    for (int i = n - 1; i >= 0; i--) {
        // ���������� ������� ������ � �����
        swap(arr[0], arr[i]);

        // �������� ��������� ���������� ��� ����������� ��������
        heapify(arr, i, 0);
    }
}


void writeOutput(string title, vector<vector<Flower>>& res) {
    ofstream fout(output, ios::app);

    fout << title << "\n";

    for (int i = 0; i < amountD; ++i) {
        fout << "������: " << res[i].size() << "\n";
        for (int j = 0; j < res[i].size(); ++j) {
            fout << res[i][j] << "\n";
        }
    }
}

void writeTime(string title, vector<std::chrono::steady_clock::time_point> time) {
    ofstream fout(timeFile, ios::app);

    fout << title << "\n";

    for (int i = 0; i < time.size() - 1; ++i)
        fout << "����� ��� ���������� " << D[i] << " ��������: " << chrono::duration_cast<chrono::milliseconds>(time[i + 1] - time[i]).count() << " [ms]\n";

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
    writeOutput("���������� �������:\n", selectSortRes);
    writeTime("���������� �������:\n", time);
    time.clear();

    time.push_back(std::chrono::steady_clock::now());
    for (int i = 0; i < amountD; ++i) {
        quickSort(initialArray[i], 0, (initialArray[i].size() - 1));
        quickSortRes.push_back(initialArray[i]);
        time.push_back(std::chrono::steady_clock::now());
    }
    writeOutput("������� ����������:\n", quickSortRes);
    writeTime("������� ����������:\n", time);
    time.clear();

    time.push_back(std::chrono::steady_clock::now());
    for (int i = 0; i < amountD; ++i) {
        heapSort(initialArray[i], initialArray[i].size());
        heapSortRes.push_back(initialArray[i]);
        time.push_back(std::chrono::steady_clock::now());
    }
    writeOutput("���������� ���������:\n", heapSortRes);
    writeTime("���������� ���������:\n", time);
    time.clear();
  
    return 0;
}
