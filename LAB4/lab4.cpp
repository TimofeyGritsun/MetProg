#include <iostream>
#include <vector>
#include <cmath>
#include <chrono>

// ����� 1: ���������������� ������������ �����
std::vector<double> congruential_generate(int size) {
    const unsigned int a = 1664525; // ���������
    const unsigned int c = 1013904223; // ����������
    const unsigned int m = pow(2, 31); // ������

    std::vector<double> sample;
    unsigned int seed = 42; // ��������� �������� ������

    for (int i = 0; i < size; i++) {
        seed = (a * seed + c) % m;
        double value = static_cast<double>(seed) / m;
        sample.push_back(value);
    }

    return sample;
}

// ����� 2: ���������������� xorshift ��������
std::vector<double> xorshift_generate(int size) {
    const unsigned int a = 13;
    const unsigned int b = 7;
    const unsigned int c = 17;

    std::vector<double> sample;
    unsigned int seed = 42;

    for (int i = 0; i < size; i++) {
        seed ^= seed << a;
        seed ^= seed >> b;
        seed ^= seed << c;
        double value = static_cast<double>(seed) / UINT_MAX;
        sample.push_back(value);
    }

    return sample;
}

// ������� ��� ���������� �������� ��������
double calculateMean(const std::vector<double>& sample) {
    double sum = 0.0;
    for (double value : sample) {
        sum += value;
    }
    return sum / sample.size();
}

// ������� ��� ���������� ������������ ����������
double calculateStdDev(const std::vector<double>& sample, double mean) {
    double sum = 0.0;
    for (double value : sample) {
        double diff = value - mean;
        sum += diff * diff;
    }
    return sqrt(sum / sample.size());
}

// ������� ��� ���������� ������������ ��������
double calculateCoefficientOfVariation(double stdDev, double mean) {
    return (stdDev / mean) * 100.0;
}

// ������� ��� �������� ������������� ������������� � ������� �������� ��-�������
bool checkUniformity(const std::vector<double>& sample) {
    const int numIntervals = 10; // ���������� ����������
    const int expectedCount = sample.size() / numIntervals; // ��������� ���������� ��������� � ������ ���������

    std::vector<int> observedCounts(numIntervals, 0); // ���������� ����������� ��������� � ������ ���������

    for (double value : sample) {
        int interval = static_cast<int>(value * numIntervals);
        observedCounts[interval]++;
    }

    double chiSquared = 0.0;

    for (int count : observedCounts) {
        double diff = count - expectedCount;
        chiSquared += (diff * diff) / expectedCount;
    }

    // �������� � ������� �������� ��-������� � ����������� ��������� ��� ��������� ������ ����������
    const double criticalValue = 16.919; // ��� ������ ���������� 0.05 � 9 �������� �������

    return chiSquared < criticalValue;
}

int main() {
    const int numSamples = 10;
    const int sampleSize = 50;

    for (int i = 0; i < numSamples; i++) {
        // ���������� ������� ������� 1
        std::vector<double> sample1 = congruential_generate(sampleSize);

        // ��������� ������� ��������, ����������� ���������� � ����������� ��������
        double mean1 = calculateMean(sample1);
        double stdDev1 = calculateStdDev(sample1, mean1);
        double cv1 = calculateCoefficientOfVariation(stdDev1, mean1);

        // ��������� ������������� �������������
        bool isUniform1 = checkUniformity(sample1);

        // ���������� ������� ������� 2
        std::vector<double> sample2 = xorshift_generate(sampleSize);
        for (int i = 0; i < 15; i++) {
            std::cout << sample2[i] << std::endl;
        }

        // ��������� ������� ��������, ����������� ���������� � ����������� ��������
        double mean2 = calculateMean(sample2);
        double stdDev2 = calculateStdDev(sample2, mean2);
        double cv2 = calculateCoefficientOfVariation(stdDev2, mean2);

        // ��������� ������������� �������������
        bool isUniform2 = checkUniformity(sample2);

        // ������� ����������
        std::cout << "Sample " << (i + 1) << " (Method 1):" << std::endl;
        std::cout << "Mean: " << mean1 << std::endl;
        std::cout << "Standard Deviation: " << stdDev1 << std::endl;
        std::cout << "Coefficient of Variation: " << cv1 << "%" << std::endl;
        std::cout << "Uniform Distribution: " << (isUniform1 ? "Yes" : "No") << std::endl;
        std::cout << std::endl;

        std::cout << "Sample " << (i + 1) << " (Method 2):" << std::endl;
        std::cout << "Mean: " << mean2 << std::endl;
        std::cout << "Standard Deviation: " << stdDev2 << std::endl;
        std::cout << "Coefficient of Variation: " << cv2 << "%" << std::endl;
        std::cout << "Uniform Distribution: " << (isUniform2 ? "Yes" : "No") << std::endl;
        std::cout << std::endl;
    }

    // ��������� ������� ��������� �������

    std::vector<int> sampleSizes = { 1000, 10000, 100000, 1000000 };

    std::vector<double> method1Times;
    std::vector<double> method2Times;
    std::vector<double> standardMethodTimes;

    for (int size : sampleSizes) {
        auto startTime = std::chrono::high_resolution_clock::now();
        std::vector<double> sample1 = congruential_generate(size);
        auto endTime = std::chrono::high_resolution_clock::now();
        double method1Time = std::chrono::duration<double, std::milli>(endTime - startTime).count();
        method1Times.push_back(method1Time);

        startTime = std::chrono::high_resolution_clock::now();
        std::vector<double> sample2 = xorshift_generate(size);
        endTime = std::chrono::high_resolution_clock::now();
        double method2Time = std::chrono::duration<double, std::milli>(endTime - startTime).count();
        method2Times.push_back(method2Time);

        startTime = std::chrono::high_resolution_clock::now();
        endTime = std::chrono::high_resolution_clock::now();
        double standardMethodTime = std::chrono::duration<double, std::milli>(endTime - startTime).count();
        standardMethodTimes.push_back(standardMethodTime);
    }


    return 0;
}
