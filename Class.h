#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

class Course {
    private :
        string courseId;
        string courseName;
        string schedule;
    public : 
        Course();
        Course(string CourseId, string CourseName, string Schedule);
};


class Student{
    private : 
        string studentId;
        string studentName;
        Course course;
        vector<Course> completedCourse; 
    public : Student(string studentId, string studentName, Course Course);
};

class Classroom {
    private :
        string classId;
        string className;
        vector<Student> studentList;
        int capacity;
    public :
        Classroom(string classId, string className, int capacity);
};