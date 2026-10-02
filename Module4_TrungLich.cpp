#include "Class.h"
#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

using namespace std;

// Cấu trúc lưu trữ thông tin mốc thời gian học
struct TimeSlot {
    int thu;          // Thứ 2 đến Thứ 7 (Mặc định Chủ nhật nghỉ)
    int tietBatDau;   // Tiết bắt đầu (Ví dụ: 1, 7)
    int tietKetThuc;  // Tiết kết thúc (Ví dụ: 4, 11)
};

// Hàm hỗ trợ tách chuỗi theo ký tự phân cách
vector<string> splitString(const string& str, char delimiter) {
    vector<string> tokens;
    stringstream ss(str);
    string token;
    while (getline(ss, token, delimiter)) {
        // Xóa khoảng trắng thừa 2 đầu chuỗi
        size_t start = token.find_first_not_of(" \t");
        size_t end = token.find_last_not_of(" \t");
        
        if (start < token.size() && end < token.size()) {
            tokens.push_back(token.substr(start, end - start + 1));
        }
    }
    return tokens; 
}

// Hàm parse chuỗi schedule thành danh sách TimeSlot
vector<TimeSlot> parseSchedule(const string& scheduleStr) {
    vector<TimeSlot> slots;
    if (scheduleStr == "NONE" || scheduleStr.empty()) return slots;

    // Tách các ngày học cách nhau bằng dấu ';'
    vector<string> daySchedules = splitString(scheduleStr, ';');

    for (const string& daySched : daySchedules) {
        size_t colonPos = daySched.find(':');
        if (colonPos >= daySched.size()) continue;

        // Trích xuất Thứ (Lấy chữ số đầu tiên trong đoạn "Thứ X")
        string dayPart = daySched.substr(0, colonPos);
        int thu = 0;
        for (char c : dayPart) {
            if (isdigit(c)) {
                thu = c - '0';
                break;
            }
        }

        // Ràng buộc: Chỉ học từ Thứ 2 đến Thứ 7 (Bỏ qua nếu Thứ < 2 hoặc > 7)
        if (thu < 2 || thu > 7) continue;

        // Trích xuất danh sách tiết học (Ví dụ: "1->4, 8->11")
        string periodPart = daySched.substr(colonPos + 1);
        vector<string> periodRanges = splitString(periodPart, ',');

        for (const string& range : periodRanges) {
            size_t arrowPos = range.find("->");
            if (arrowPos < range.size()) {
                int tietBD = stoi(range.substr(0, arrowPos));
                int tietKT = stoi(range.substr(arrowPos + 2));
                slots.push_back({thu, tietBD, tietKT});
            }
        }
    }
    return slots;
}

// Lớp xử lý Đăng ký Học phần & Kiểm tra Trùng lịch
class RegistrationService {
public:
    // Kiểm tra trùng lịch và tiến hành đăng ký
    // Constraint: 0 < N (đã kiểm tra danh sách không rỗng)
    static bool registerCourses(Student& student, const vector<Course>& requestedCourses) {
        if (requestedCourses.empty()) {
            cout << "[LỖI] Danh sách môn học đăng ký phải lớn hơn 0 (N > 0)!\n";
            return false;
        }

        // Ma trận thời khóa biểu: [Thứ (2..7)][Tiết (1..14)]
        // Giá trị true đại diện cho việc tiết đó đã có môn đăng ký
        bool thoiKhoaBieu[8][15] = {false};
        
        // Lưu thông tin Mã môn học tương ứng từng tiết để xuất câu báo lỗi chi tiết
        string courseAtSlot[8][15];

        // 1. Nạp các môn sinh viên ĐÃ ĐĂNG KÝ TRƯỚC ĐÓ vào ma trận
        for (const Course& existingCourse : student.getAttendingCourse()) {
            vector<TimeSlot> slots = parseSchedule(existingCourse.getSchedule());
            for (const TimeSlot& slot : slots) {
                for (int p = slot.tietBatDau; p <= slot.tietKetThuc; ++p) {
                    thoiKhoaBieu[slot.thu][p] = true;
                    courseAtSlot[slot.thu][p] = existingCourse.getCourseName() + " (" + existingCourse.getCourseId() + ")";
                }
            }
        }

        // 2. Kiểm tra danh sách N môn MỚI yêu cầu đăng ký
        for (const Course& newCourse : requestedCourses) {
            vector<TimeSlot> slots = parseSchedule(newCourse.getSchedule());

            for (const TimeSlot& slot : slots) {
                for (int p = slot.tietBatDau; p <= slot.tietKetThuc; ++p) {
                    // Nếu tiết này ĐÃ ĐƯỢC ĐÁNH DẤU -> Báo lỗi trùng lịch
                    if (thoiKhoaBieu[slot.thu][p]) {
                        cout << "\n================ ĐĂNG KÝ THẤT BẠI ================\n";
                        cout << "[LỖI TRÙNG LỊCH] Sinh viên: " << student.getStudentName() 
                             << " (MSSV: " << student.getStudentId() << ")\n";
                        cout << "-> Môn muốn đăng ký: " << newCourse.getCourseName() << " [" << newCourse.getCourseId() << "]\n";
                        cout << "-> Thời gian bị trùng: Thứ " << slot.thu << ", Tiết " << p << "\n";
                        cout << "-> Trùng với môn: " << courseAtSlot[slot.thu][p] << "\n";
                        cout << "Hệ thống từ chối toàn bộ đợt đăng ký này.\n";
                        cout << "====================================================\n\n";
                        return false;
                    }
                }
            }

            // Tạm thời ghi nhận môn mới này vào ma trận để phát hiện nếu trùng với các môn tiếp theo trong chính danh sách đăng ký N
            for (const TimeSlot& slot : slots) {
                for (int p = slot.tietBatDau; p <= slot.tietKetThuc; ++p) {
                    thoiKhoaBieu[slot.thu][p] = true;
                    courseAtSlot[slot.thu][p] = newCourse.getCourseName() + " (" + newCourse.getCourseId() + ")";
                }
            }
        }

        // 3. Nếu KHÔNG CÓ TRÙNG LỊCH -> Tiến hành đăng ký tất cả các môn vào tài khoản sinh viên
        for (const Course& newCourse : requestedCourses) {
            student.addAttendingCourse(newCourse);
        }

        cout << "\n================ ĐĂNG KÝ THÀNH CÔNG ================\n";
        cout << "Sinh viên " << student.getStudentName() << " đã đăng ký thành công " 
             << requestedCourses.size() << " học phần mới.\n";
        cout << "====================================================\n\n";

        return true;
    }
};