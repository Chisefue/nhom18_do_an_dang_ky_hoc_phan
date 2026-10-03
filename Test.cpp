#include <iostream>
#include <cassert>
#include "Class.h"

// Bao gồm trực tiếp các file CPP để có thể gọi hàm do bạn không khai báo chúng trong Class.h
#include "Module1_TimKiem.cpp"
#include "Module2_TienQuyet.cpp"
#include "Module3_TruyXuat.cpp"
#include "Module4_TrungLich.cpp"

using namespace std;

// ==========================================
// TEST KIỂU DỮ LIỆU TỰ TẠO
// ==========================================

void test_DIYVector() {
    DIYVector<int> vec;
    assert(vec.empty() == true);
    assert(vec.size() == 0);
    
    vec.push_back(10);
    vec.push_back(20);
    assert(vec.size() == 2);
    assert(vec.empty() == false);
    assert(vec[0] == 10);
    assert(vec[1] == 20);
    
    vec.pop_back();
    assert(vec.size() == 1);
    
    // Test Copy Constructor & Assignment Operator
    DIYVector<int> vec2 = vec; 
    assert(vec2.size() == 1);
    assert(vec2[0] == 10);
    
    cout << "[PASS] DIYVector hoat dong chinh xac." << endl;
}

void test_DIYSet() {
    DIYSet<string> set;
    assert(set.size() == 0);
    
    set.insert("IT001");
    set.insert("IT002");
    set.insert("IT001"); // Thêm trùng lặp
    
    assert(set.size() == 2); // Kích thước vẫn là 2
    assert(set.count("IT001") == 1);
    assert(set.count("IT003") == 0);
    
    cout << "[PASS] DIYSet hoat dong chinh xac." << endl;
}

// ==========================================
// TEST MODULE 1: TÌM KIẾM
// ==========================================
void test_Module1_TimKiem() {
    DIYVector<Course> courses;
    courses.push_back(Course("IT003", "CSDL", "NONE"));
    courses.push_back(Course("IT001", "Lap trinh", "NONE"));
    courses.push_back(Course("IT002", "CTDL", "NONE"));
    courses.push_back(Course("MA001", "Toan", "NONE"));

    // Test 1: Tìm kiếm theo tiền tố
    DIYVector<Course> res1 = searchCourseModule(courses, "IT");
    assert(res1.size() == 3); // Tìm thấy IT001, IT002, IT003

    // Test 2: Tìm kiếm chính xác
    DIYVector<Course> res2 = searchCourseModule(courses, "IT001");
    assert(res2.size() == 1);

    // Test 3: Ràng buộc độ dài ( > 13 ký tự sẽ trả về rỗng)
    DIYVector<Course> res3 = searchCourseModule(courses, "QUADAICHUOI1234");
    assert(res3.size() == 0);
    
    cout << "[PASS] Module 1 (Tim Kiem) hoat dong chinh xac." << endl;
}

// ==========================================
// TEST MODULE 2: HỌC PHẦN TIÊN QUYẾT
// ==========================================
void test_Module2_TienQuyet() {
    PrerequisiteService service;
    service.addPrerequisite("IT002", "IT001"); // IT002 yêu cầu IT001
    
    DIYSet<string> completed;
    
    // Test 1: Chưa học môn tiên quyết
    PrereqResult res1 = service.checkEligibility("IT002", completed);
    assert(res1.isEligible == false);
    assert(res1.missing[0] == "IT001");
    
    // Test 2: Đã học môn tiên quyết
    completed.insert("IT001");
    PrereqResult res2 = service.checkEligibility("IT002", completed);
    assert(res2.isEligible == true);
    
    // Test 3: Môn không có tiên quyết
    PrereqResult res3 = service.checkEligibility("MA001", completed);
    assert(res3.isEligible == true);

    cout << "[PASS] Module 2 (Tien Quyet) hoat dong chinh xac." << endl;
}

// ==========================================
// TEST MODULE 3: TRUY XUẤT (KIỂM TRA ĐỊNH DẠNG)
// ==========================================
void test_Module3_TruyXuat() {
    // Module này chủ yếu cout danh sách nên ta test kỹ hàm kiểm tra định dạng
    assert(kiemTraDinhDang("IT001_01") == true);       // Hợp lệ
    assert(kiemTraDinhDang("IT001") == false);         // Thiếu dấu _
    assert(kiemTraDinhDang("_IT001") == false);        // Dấu _ ở đầu
    assert(kiemTraDinhDang("IT001_01_02") == false);   // Dư dấu _
    assert(kiemTraDinhDang("12345678901234") == false);// Quá 13 ký tự
    
    cout << "[PASS] Module 3 (Kiem Tra Dinh Dang Lop) hoat dong chinh xac." << endl;
}

// ==========================================
// TEST MODULE 4: XỬ LÝ TRÙNG LỊCH
// ==========================================
void test_Module4_TrungLich() {
    Student sv("123", "Nguyen Van A");
    
    // Giả lập sinh viên đang học môn IT001 vào Thứ 2, tiết 1 đến 4
    Course c1("IT001", "Lap trinh", "Thứ 2:1->4");
    sv.addAttendingCourse(c1);

    // Test 1: Đăng ký trùng lịch (Giao nhau ở tiết 4)
    Course c2("IT002", "CTDL", "Thứ 2:4->6");
    DIYVector<Course> req1; 
    req1.push_back(c2);
    assert(RegistrationService::registerCourses(sv, req1) == false);

    // Test 2: Đăng ký thành công (Không trùng lịch)
    Course c3("MA001", "Toan", "Thứ 3:1->4");
    Course c4("PH001", "Vat Ly", "Thứ 3:6->9");
    DIYVector<Course> req2; 
    req2.push_back(c3);
    req2.push_back(c4);
    assert(RegistrationService::registerCourses(sv, req2) == true);
    
    cout << "[PASS] Module 4 (Kiem Tra Trung Lich) hoat dong chinh xac." << endl;
}

// ==========================================
// HÀM MAIN CHẠY TOÀN BỘ TEST
// ==========================================
int main() {
    cout << "========================================\n";
    cout << "  BAT DAU KIEM THU TU DONG (AUTO-TEST)\n";
    cout << "========================================\n\n";

    test_DIYVector();
    test_DIYSet();
    test_Module1_TimKiem();
    test_Module2_TienQuyet();
    test_Module3_TruyXuat();
    test_Module4_TrungLich();

    cout << "\n========================================\n";
    cout << "  TAT CA CAC TEST DEU THANH CONG! 🎉\n";
    cout << "========================================\n";
    return 0;
}