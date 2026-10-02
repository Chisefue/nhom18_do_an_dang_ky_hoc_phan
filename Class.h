#ifndef _CLASS_H
#define _CLASS_H

#include <iostream>
#include <string>
#include <unordered_map>
#include <set>
#include <sstream>
#include <algorithm>
#include "Vector.h"


using namespace std;



class Course {
    private:
        string courseId;
        string courseName;
        string schedule;
        DIYVector<string> prerequisiteCourses; 
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
            //Học phần tiên quyết đang ở dạng string, tức là mã học phần, khi làm nhớ
            //Lấy học phần dựa vào id, có sẵn DIYVector
        void addPrerequisiteCourseId (string course) { prerequisiteCourses.push_back(course); }
};

class Student {
    private:
        string studentId;
        string studentName;
        DIYVector<Course> attendingCourse;
        set <string> completedCourseId;
    public:
        Student() {
            studentId = "NONE";
            studentName = "NONE";
        }
        Student(string studentId, string studentName) {
            this->studentId = studentId;
            this->studentName = studentName;
        }

        string getStudentId() const { return studentId; }
        string getStudentName() const { return studentName; }
        DIYVector<Course> getAttendingCourse() { return attendingCourse; }
        const set<string>& getCompletedCourseId() const { return completedCourseId; }

        void addAttendingCourse(Course course) { attendingCourse.push_back(course); }
        void addCompletedCourseId(const string& courseId) { completedCourseId.insert(courseId); }
    };

class Classroom {
    private:
        string classId;
        string className;
        DIYVector<Student> studentList;
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
        const DIYVector<Student> getStudentList() { return studentList; }
        void addStudent(const Student& sv) { studentList.push_back(sv); }
};


struct PrereqResult {
    bool isEligible;
    DIYVector<string> missing;
    string message;
};

class PrerequisiteService {
private: 
    // Đồ thị môn tiên quyết: Mã môn -> Danh sách các môn tiên quyết
    unordered_map<string, DIYVector<string>> prereqGraph;
public:
    PrerequisiteService() = default;
    void addPrerequisite(const string& targetCourse, const string& prereqCourse);
    PrereqResult checkEligibility(const string& courseCode, const set<string>& completedCourse) const;
};

#endif