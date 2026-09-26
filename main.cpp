#include "Class.h"
#include "Module3_TruyXuat.cpp"

int main() {
    Course gdqp = Course("GDQP110131_02", "Giáo dục quốc phòng 1", "Thứ 3: 1->4, 8->11");
    Course dstt = Course("MATH143001_01", "Đại số tuyến tính và cấu trúc đại số", "Thứ 2: 7->11; Thứ 4: 7->10");
    Course nmcntt = Course("INIT130185_02", "Nhập môn ngành CNTT", "Thứ 4: 1->4; Thứ 5: 7->11");
    Student hai = Student("25162028", "Ronaldo Bui");
    hai.addAttendingCourse(gdqp);
    hai.addCompletedCourse(dstt);
    hai.addCompletedCourse(nmcntt);
    Classroom dsttClass = Classroom("MATH143001_01", "V503", 70);
    Classroom nmcnttClass = Classroom("GDQP110131_02", "A5-204", 73);
    dsttClass.addStudent(hai);
    nmcnttClass.addStudent(hai);
    vector<Classroom> classes = {dsttClass, nmcnttClass};
    truyXuatDanhSachLop("dsst01", classes);
    return 0;
}