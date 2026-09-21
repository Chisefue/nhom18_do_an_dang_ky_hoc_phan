#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

class Course {
    private :
        string CourseId;
        string CourseName;
        string Schedule;
    public : 
        Course() {
            CourseId = "";
            CourseName = "";
            Schedule = "";
        }
        Course(string CourseId, string CourseName, string Schedule) {
            this->CourseId = CourseId;
            this->CourseName = CourseName;
            this->Schedule = Schedule;
        }
};


class Student : public Course {
    private : 
        string studentId;
        string studentName;
        Course course;
        vector<Course> completedCourse; 
    public : Student(string studentId, string studentName, Course Course) : Course() {
        this->studentId = studentId;
        this->studentName = studentName;
    }
};

