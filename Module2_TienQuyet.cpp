#include "Class.h"

void PrerequisiteService::addPrerequisite(const string& targetCourse, const string& prereqCourse) {
    if (!prereqCourse.empty()) {
        prereqGraph[targetCourse].push_back(prereqCourse);
    }
}

PrereqResult PrerequisiteService::checkEligibility(const string& courseCode, const DIYSet<string>& completedCourse) const {
    auto it = prereqGraph.find(courseCode);
    if (it == prereqGraph.end() || it->second.empty()) {
        return {true, {}, "Hợp lệ: Môn học không yêu cầu môn tiên quyết."};
    }

    DIYVector<string> missing;
    for (const string& req : it->second) {
        if (completedCourse.find(req) == completedCourse.end()) {
            missing.push_back(req);
        }
    }

    if (!missing.empty()) {
        string msg = "Từ chối đăng ký: Sinh viên chưa hoàn thành môn tiên quyết: ";
        for (int i = 0; i < missing.size(); i++) {
            msg += missing[i] + (i + 1 < missing.size() ? ", " : ".");
        }
        return {false, missing, msg};
    }

    return {true, {}, "Đủ điều kiện tiên quyết."};
}