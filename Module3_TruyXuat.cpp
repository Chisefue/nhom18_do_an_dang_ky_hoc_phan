
#include "Class.h"

// Định nghĩa constructor cho Course
Course::Course() : courseId(""), courseName(""), schedule("") {}

Course::Course(string maHP, string tenHP, string thoiGian) {
    courseId = maHP;
    courseName = tenHP;
    schedule = thoiGian;
}

// Kiểm tra định dạng: 0 < độ dài <= 13 và có dạng MaHocPhan_SoThuTuLop
bool kiemTraDinhDang(string s) {
    if (s.empty() || s.length() > 13) {
        return false;
    }

    int viTriGachDuoi = -1;
    int soDauGachDuoi = 0;

    for (int i = 0; i < (int)s.length(); i++) {
        if (s[i] == '_') {
            soDauGachDuoi++;
            viTriGachDuoi = i;
        }
    }

    if (soDauGachDuoi != 1 || viTriGachDuoi == 0 || viTriGachDuoi == (int)s.length() - 1) {
        return false;
    }

    return true;
}

// Truy xuất danh sách sinh viên theo mã lớp
void truyXuatDanhSachLop(string s, const vector<Classroom>& danhSachCacLop) {
    if (!kiemTraDinhDang(s)) {
        cout << "Ma lop khong hop le! (Yeu cau: 0 < do dai <= 13 va co dang MaHocPhan_SoThuTuLop)\n";
        return;
    }

    bool timThay = false;
    for (int i = 0; i < (int)danhSachCacLop.size(); i++) {
        if (danhSachCacLop[i].getClassId() == s) {
            timThay = true;
            cout << "\n=== DANH SACH SINH VIEN LOP: " << s << " ===\n";
            const vector<Student>& ds = danhSachCacLop[i].getStudentList();
            
            if (ds.empty()) {
                cout << "(Lop hoc hien chua co sinh vien nao dang ky)\n";
            } else {
                for (int j = 0; j < (int)ds.size(); j++) {
                    cout << j + 1 << ". MSSV: " << ds[j].getStudentId() 
                         << " | Ho ten: " << ds[j].getStudentName() << "\n";
                }
            }
            break;
        }
    }

    if (!timThay) {
        cout << "[LOI] Khong tim thay lop hoc phan co ma: " << s << "\n";
    }
}