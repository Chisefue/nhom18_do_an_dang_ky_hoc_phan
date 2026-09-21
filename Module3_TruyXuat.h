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

public:
    // Constructor khởi tạo
    SinhVien(string id = "", string name = "") : mssv(id), hoTen(name) {}

    // Getter lấy dữ liệu
    string getMSSV() const { return mssv; }
    string getHoTen() const { return hoTen; }
};

// 2. Class Lớp học phần
class LopHocPhan {
private:
    string maLop;                 // Định dạng: MaHocPhan_SoThuTuLop
    vector<SinhVien> danhSachSV;  // Vector lưu danh sách sinh viên theo thứ tự đăng ký

public:
    LopHocPhan(string id = "") : maLop(id) {}

    // Thêm sinh viên vào lớp
    void themSinhVien(const SinhVien& sv) {
        danhSachSV.push_back(sv);
    }

    string getMaLop() const { return maLop; }
    const vector<SinhVien>& getDanhSachSV() const { return danhSachSV; }
};

// Khai báo các hàm của Module 3
bool kiemTraDinhDang(string s);
void truyXuatDanhSachLop(string s, const vector<LopHocPhan> & danhSachCacLop);

