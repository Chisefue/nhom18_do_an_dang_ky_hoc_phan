#include <iostream>
#include <chrono>
#include <string>
#include <cstdlib>
#include "Class.h"

// Chỉ include Module 1 vì chúng ta cần test Quick Sort và Binary Search
#include "Module1_TimKiem.cpp"

using namespace std;
using namespace std::chrono;

// Hàm hỗ trợ sinh chuỗi ngẫu nhiên
string genRandomString(int length) {
    string s = "";
    static const char alphanum[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    for (int i = 0; i < length; ++i) {
        s += alphanum[rand() % (sizeof(alphanum) - 1)];
    }
    return s;
}

// ==========================================
// 1. BENCHMARK DIYVECTOR
// ==========================================
void benchmarkVector() {
    cout << "[1] Đang test DIYVector với 1,000,000 phần tử..." << endl;
    DIYVector<int> vec;
    
    auto start = high_resolution_clock::now();
    for (int i = 0; i < 1000000; i++) {
        vec.push_back(i);
    }
    auto stop = high_resolution_clock::now();
    
    auto duration = duration_cast<milliseconds>(stop - start);
    cout << " -> Thoi gian them 1,000,000 phan tu: " << duration.count() << " ms\n\n";
}

void benchmarkSet() {
    cout << "[2] Đang test DIYSet (Cây nhị phân) với 100,000 phần tử ngẫu nhiên..." << endl;
    DIYSet<string> mySet;
    DIYVector<string> trackList; 
    
    // Sinh 100,000 chuỗi ngẫu nhiên
    for (int i = 0; i < 100000; i++) {
        trackList.push_back(genRandomString(8));
    }

    // Insert
    auto start_insert = high_resolution_clock::now();
    for (int i = 0; i < 100000; i++) {
        mySet.insert(trackList[i]);
    }
    auto stop_insert = high_resolution_clock::now();
    auto dur_insert = duration_cast<milliseconds>(stop_insert - start_insert);
    cout << " -> Thoi gian Insert 100,000 phan tu: " << dur_insert.count() << " ms\n";

    // Find
    auto start_find = high_resolution_clock::now();
    for (int i = 0; i < 10000; i++) {
        mySet.find(trackList[i]);
    }
    auto stop_find = high_resolution_clock::now();
    auto dur_find = duration_cast<microseconds>(stop_find - start_find);
    cout << " -> Thoi gian Search 10,000 phan tu : " << dur_find.count() << " micro-giay (us)\n\n";
}

// QuickSort và binary search
void benchmarkAlgorithms() {
    cout << "[3] Đang test QuickSort & Binary Search với 100,000 Hoc Phan..." << endl;
    DIYVector<Course> courses;
    
    // Khởi tạo 100,000 Học phần với Mã học phần ngẫu nhiên
    for (int i = 0; i < 100000; i++) {
        string maHP = "IT" + to_string(rand() % 9000 + 1000) + genRandomString(3);
        courses.push_back(Course(maHP, "Ten Mon Hoc", "NONE"));
    }

    // Chèn 1 mã cố định để test tìm kiếm
    courses.push_back(Course("IT9999XYZ", "Mon Test Hieu Nang", "NONE"));

    // Đo thời gian Quick Sort
    auto start_sort = high_resolution_clock::now();
    quickSort(courses, 0, courses.size() - 1);
    auto stop_sort = high_resolution_clock::now();
    auto dur_sort = duration_cast<milliseconds>(stop_sort - start_sort);
    cout << " -> Thoi gian Quick Sort 100,001 hoc phan: " << dur_sort.count() << " ms\n";

    // Đo thời gian Binary Search
    auto start_search = high_resolution_clock::now();
    int index = findFirstMatch(courses, "IT9999");
    auto stop_search = high_resolution_clock::now();
    auto dur_search = duration_cast<microseconds>(stop_search - start_search);
    
    if (index != -1) {
        cout << " -> Thoi gian Binary Search tim thay (Tien to 'IT9999'): " << dur_search.count() << " micro-giay (us)\n\n";
    }
}

int main() {
    srand(time(NULL)); // Khởi tạo seed ngẫu nhiên
    cout << "=================================================\n";
    cout << "       BẰNG CHỨNG HIỆU NĂNG (BENCHMARK)\n";
    cout << "=================================================\n\n";

    benchmarkVector();
    benchmarkSet();
    benchmarkAlgorithms();

    cout << "=================================================\n";
    cout << "           HOÀN TẤT BÀI KIỂM TRA\n";
    cout << "=================================================\n";
    return 0;
}