#include <iostream>
#include <vector>
#include <string>

using namespace std;

// Cấu trúc dữ liệu dùng chung
struct SinhVien {
    string mssv;
    string hoTen;
};

struct LopHocPhan {
    string maLop;
    vector<SinhVien> danhSachSV;
};

// Khai báo các hàm của Module 3
bool kiemTraDinhDang(string s);
void truyXuatDanhSachLop(string s, const vector<LopHocPhan> & danhSachCacLop);

