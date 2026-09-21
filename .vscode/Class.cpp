#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

class Course {
    private :
        string CourseId;
        string CourseName;
        string Schedule;
    public : Course(string CourseId, string CourseName, string Schedule) {
        this->CourseId = CourseId;
        this->CourseName = CourseName;
        this->Schedule = Schedule;
    }
};


class Student : public Course {
    private : 
        string studentId;
        string studentName;
        vector<Course> completedCourse; // Key = HocSinh Val = DanhSachDaHocs 
};

