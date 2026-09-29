#include "Class.h"
#include "Function.h"
#include <iostream>

int main() {
    cout << "=== HE THONG DANG KY HOC PHAN & QUAN LY LOP (NHOM 18) ===\n\n";

    // 1. Tạo các môn học
    Course gdqp("GDQP110131_02", "Giao duc quoc phong 1", "Thu 3: 1->4, 8->11");
    Course dstt("MATH143001_01", "Dai so tuyen tinh va cau truc dai so", "Thu 2: 7->11; Thu 4: 7->10");
    Course nmcntt("INIT130185_02", "Nhap mon nganh CNTT", "Thu 4: 1->4; Thu 5: 7->11");

    // 2. Khởi tạo và thiết lập môn tiên quyết (Module của bạn)
    PrerequisiteService prereqService;
    // Để học Nhập môn ngành CNTT (INIT130185) -> Cần học Đại số tuyến tính (MATH143001)
    prereqService.addPrerequisite("INIT130185", "MATH143001");

    // 3. Tạo sinh viên
    Student hai("25162028", "Ronaldo Bui");
    Student chieu("25110152", "Chieu Dep Trai");
    Student khoa("25110241", "Messi Khoa");

    // Sinh viên Hải đã hoàn thành môn Đại số tuyến tính
    hai.addCompletedCourse("MATH143001");
    // Chiều và Khoa chưa hoàn thành môn nào

    // 4. KIỂM TRA ĐIỀU KIỆN TIÊN QUYẾT KHI ĐĂNG KÝ HỌC PHẦN
    cout << "--- [TEST MODULE 2] KIEM TRA MON TIEN QUYET KHI DANG KY INIT130185 ---\n";
    
    // Test cho Hải (Đã học MATH143001 -> Hợp lệ)
    auto resHai = prereqService.checkEligibility("INIT130185", hai.getCompletedCourseId());
    cout << "Hai dang ky: " << resHai.message << "\n";

    // Test cho Chiều (Chưa học MATH143001 -> Bị từ chối)
    auto resChieu = prereqService.checkEligibility("INIT130185", chieu.getCompletedCourseId());
    cout << "Chieu dang ky: " << resChieu.message << "\n\n";

    // 5. Quản lý lớp học (Module 3)
    Classroom dsttClass("MATH143001_01", "V503", 70);
    Classroom nmcnttClass("GDQP110131_02", "A5-204", 73);

    dsttClass.addStudent(hai);
    dsttClass.addStudent(chieu);
    dsttClass.addStudent(khoa);
    nmcnttClass.addStudent(hai);

    vector<Classroom> classes = {dsttClass, nmcnttClass};

    // 6. Truy xuất danh sách sinh viên trong lớp theo MC2
    truyXuatDanhSachLop("MATH143001_01", classes);

    return 0;
}