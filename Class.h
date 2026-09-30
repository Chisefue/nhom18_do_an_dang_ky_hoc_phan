#ifndef _CLASS_H
#define _CLASS_H

#include <iostream>
#include <string>
#include <vector>
#include <unordered_map>
#include <set>
#pragma once

using namespace std;

template <typename T> 
class DIYVector {
    private:
        T* arr;
        int capacity;
        int size;

        void expand() {
            if (capacity == 0) capacity = 1;
            else capacity = capacity * 2;
            T* tmp = new T[capacity];
            for (int i = 0; i < size; i++) {
                tmp[i] = arr[i];
            }
            delete[] arr;
            arr = tmp;
        }
    public: 
        DIYVector() {
            arr = nullptr;
            capacity = 0;
            size = 0;
        }
        ~DIYVector() {
            delete[] arr;
        }

        DIYVector(DIYVector& otherVector) { //Sao chep vector
            capacity = otherVector.capacity;
            size = otherVector.size;
            arr = new T[capacity];
            for (int i = 0; i < size; i++) {
                arr[i] = otherVector.arr[i];
            }
        }
        const DIYVector& operator=(DIYVector &otherVector) {
            if (this != &otherVector) {
                delete[] arr;
                capacity = otherVector.capacity;
                size = otherVector.size;
                arr = new T[capacity];
                for (int i = 0; i < size; i++) {
                    arr[i] = otherVector.arr[i];
                }
            }
            return *this;
        }

};

class Course {
    private:
        string courseId;
        string courseName;
        string schedule;
        vector<string> prerequisiteCourses;
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
            //Lấy học phần dựa vào id, có sẵn vector
        void addPrerequisiteCourseId (string course) { prerequisiteCourses.push_back(course); }
};

class Student {
    private:
        string studentId;
        string studentName;
        vector<Course> attendingCourse;
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
        vector<Course> getAttendingCourse() { return attendingCourse; }
        const set<string>& getCompletedCourseId() const { return completedCourseId; }

        void addAttendingCourse(Course course) { attendingCourse.push_back(course); }
        void addCompletedCourseId(const string& courseId) { completedCourseId.insert(courseId); }
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


struct PrereqResult {
    bool isEligible;
    vector<string> missing;
    string message;
};

class PrerequisiteService {
private: 
    // Đồ thị môn tiên quyết: Mã môn -> Danh sách các môn tiên quyết
    unordered_map<string, vector<string>> prereqGraph;
public:
    PrerequisiteService() = default;
    void addPrerequisite(const string& targetCourse, const string& prereqCourse);
    PrereqResult checkEligibility(const string& courseCode, const set<string>& completedCourse) const;
};

#endif