#include "Class.h"
#include <fstream>
#include "Module3_TruyXuat.cpp"
#include "json.hpp"

using namespace std;
using json = nlohmann::json;

int main() {
    ifstream input("input/Course.json");
    if (!input.is_open()) {
        cout << "Lỗi, không thể mở được file Course.json\n";
        return 1;
    }
    json jsonData;
    input >> jsonData;
    input.close();

    vector<Course> courseList;

    for (const auto& item : jsonData["courses"]) {
        string courseId = item["courseId"];
        string courseName = item["courseName"];
        string schedule = item["schedule"];
        Course c (courseId, courseName, schedule);
        courseList.push_back(c);
    }
    cout << "Đã đọc thành công " << courseList.size() << " học phần:\n\n";

    input.open("input/Student.json");
    if (!input.is_open()) {
        cout << "Lỗi, không thể mở được file Student.json\n";
        return 1;
    }
    input >> jsonData;
    input.close();

    vector<Student> studentList;

    for (const auto& student : jsonData["students"]) {
        string studentId = student["studentId"];
        string studentName = student["studentName"];
        Student s(studentId, studentName);
        for (const auto& attendingCourse : student["attendingCourses"]) {
            for (auto& course : courseList) {
                if (attendingCourse == course.getCourseId()) s.addAttendingCourse(course);
            }
        }
        for (const auto& completedCourse : student["completedCourses"]) {
            for (auto& course : courseList) {
                if (completedCourse == course.getCourseId()) s.addCompletedCourse(course);
            }
        }
        studentList.push_back(s);
    }
    
    

    cout << "Đăng nhập: \n";
    cout << "1. Sinh viên.\n";
    cout << "2. Admin.\n";
    int userInput;
    while (true) {
        cin >> userInput;
        if (userInput == 1) {
            cout << "Nhập mã số sinh viên: \n";
            cin >> userInput;

        }
    }
    return 0;
}