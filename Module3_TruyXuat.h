#ifndef MODULE3_TRUYXUAT_H
#define MODULE3_TRUYXUAT_H

#include <iostream>
#include <vector>
#include <string>

using namespace std;

// 1. Class lưu thông tin Sinh viên
class SinhVien {
private:
    string mssv;
    string hoTen;

struct LopHocPhan {
    string maLop;
    vector<SinhVien> danhSachSV;
};

// Khai báo các hàm của Module 3
bool kiemTraDinhDang(string s);
void truyXuatDanhSachLop(string s, const vector<LopHocPhan> & danhSachCacLop);

