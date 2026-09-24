#ifndef _CLASS_H
#define _CLASS_H

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>

using namespace std;

class Course {
    private:
        string courseId;
        string courseName;
        string schedule;
    public:
        Course() {
            courseId = "NONE";
            courseName = "NONE";
            schedule = "NONE";
        }
        Course(string courseId, string courseName, string schedule) {
            this->courseId = courseId;
            this->courseName = courseName;
            this->schedule = schedule;
        }

        string getCourseId() const { return courseId; }
        string getCourseName() const { return courseName; }
        string getSchedule() const { return schedule; }
};

class Student {
    private:
        string studentId;
        string studentName;
        Course course;
        vector<Course> completedCourse;
    public:
        Student() {
            studentId = "NONE";
            studentName = "NONE";
            course = Course();
            
        }
        Student(string studentId, string studentName, Course course) {
            this->studentId = studentId;
            this->studentName = studentName;
            this->course = course;
        }

        string getStudentId() const { return studentId; }
        string getStudentName() const { return studentName; }
    };

class Classroom {
    private:
        string classId;
        string className;
        vector<Student> studentList;
        int capacity;
    public:
        Classroom() {
            classId = "NONE";
            className  = "NONE";
            capacity = 0;
        }
        Classroom(string classId, string className, int capacity) {
            this->classId = classId;
            this->className = className;
            this->capacity = capacity;
        }

        string getClassId() const { return classId; }
        string getClassName() const { return className; }
        const vector<Student> getStudentList() { return studentList; }
        void addStudent(const Student& sv) { studentList.push_back(sv); }
    };

#endif