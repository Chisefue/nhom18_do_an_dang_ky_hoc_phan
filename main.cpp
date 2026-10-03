// Backend C++ cho đồ án đăng ký học phần – gọi trực tiếp logic của 4 module.
// Build:  g++ -std=c++17 main.cpp -o server   (Windows/MinGW thêm: -lws2_32 -lmswsock)
// Chạy:   ./server   rồi mở http://localhost:8080
#include "lib/httplib.h"
#include "lib/json.hpp"
#include <fstream>
#include <mutex>
#include "Class.h"
#include "Module1_TimKiem.cpp"
#include "Module2_TienQuyet.cpp"
#include "Module3_TruyXuat.cpp"
#include "Module4_TrungLich.cpp"

using json = nlohmann::json;

static DIYVector<Course> courses;
static DIYVector<Student> students;
static DIYVector<Classroom> classrooms;
static PrerequisiteService prereq;
static mutex mtx;

// ---------- tiện ích ----------
static string baseId(const string& id) { return id.substr(0, id.find('_')); }
static string upper(string s) { for (auto& c : s) c = toupper((unsigned char)c); return s; }
static Student* findStudent(const string& id) { for (unsigned i = 0; i < students.size(); i++) if (students[i].getStudentId() == id) return &students[i]; return nullptr; }
static Course* findCourse(const string& id) { for (unsigned i = 0; i < courses.size(); i++) if (courses[i].getCourseId() == id) return &courses[i]; return nullptr; }
static Classroom* findClass(const string& id) { for (unsigned i = 0; i < classrooms.size(); i++) if (classrooms[i].getClassId() == id) return &classrooms[i]; return nullptr; }
static bool inClass(Classroom& c, const string& sid) { auto l = c.getStudentList(); for (unsigned i = 0; i < l.size(); i++) if (l[i].getStudentId() == sid) return true; return false; }
static int classCount(Classroom& c) { return (int)c.getStudentList().size(); }

// Bắt output cout của các module (module in thông báo ra console) để trả về cho web
struct CoutCapture {
    stringstream ss; streambuf* old;
    CoutCapture() { old = cout.rdbuf(ss.rdbuf()); }
    ~CoutCapture() { cout.rdbuf(old); }
    string text() {
        string out, line; stringstream in(ss.str());
        while (getline(in, line)) {
            if (!line.empty() && line.back() == '\r') line.pop_back();
            if (line.empty() || line[0] == '=' || line.find("Hệ thống từ chối") != string::npos) continue;
            out += (out.empty() ? "" : " | ") + line;
        }
        return out;
    }
};

static void saveData() {
    json js; js["students"] = json::array();
    for (unsigned i = 0; i < students.size(); i++) {
        Student& s = students[i]; json a = json::array(), d = json::array();
        auto at = s.getAttendingCourse();
        for (unsigned k = 0; k < at.size(); k++) a.push_back(baseId(at[k].getCourseId()));
        for (auto& c : s.getCompletedCourseId()) d.push_back(c);
        js["students"].push_back({{"studentId", s.getStudentId()}, {"studentName", s.getStudentName()}, {"attendingCourses", a}, {"completedCourses", d}});
    }
    ofstream("input/Student.json") << js.dump(2);
    json jc; jc["classrooms"] = json::array();
    for (unsigned i = 0; i < classrooms.size(); i++) {
        Classroom& c = classrooms[i]; json ids = json::array(); auto l = c.getStudentList();
        for (unsigned k = 0; k < l.size(); k++) ids.push_back(l[k].getStudentId());
        jc["classrooms"].push_back({{"classId", c.getClassId()}, {"className", c.getClassName()}, {"capacity", c.getCapacity()}, {"studentIds", ids}});
    }
    ofstream("input/Classroom.json") << jc.dump(2);
}

static bool loadData() {
    try {
        json jc, js, jr;
        ifstream("input/Course.json") >> jc; ifstream("input/Student.json") >> js; ifstream("input/Classroom.json") >> jr;
        for (auto& c : jc["courses"]) {
            Course co(c["courseId"], c["courseName"], c["schedule"]);
            for (auto& p : c["prerequisiteCoursesId"]) { co.addPrerequisiteCourseId(p); prereq.addPrerequisite(c["courseId"], p); }
            courses.push_back(co);
        }
        for (auto& s : js["students"]) {
            Student st(s["studentId"], s["studentName"]);
            for (auto& d : s["completedCourses"]) st.addCompletedCourseId(d);
            students.push_back(st);
        }
        for (auto& c : jr["classrooms"]) {
            Classroom cl(c["classId"], c["className"], c["capacity"]);
            Course* co = findCourse(c["classId"]);
            for (auto& sid : c["studentIds"]) {
                Student* s = findStudent(sid);
                if (s) { cl.addStudent(*s); if (co) s->addAttendingCourse(*co); }
            }
            classrooms.push_back(cl);
        }
        for (unsigned i = 0; i < courses.size(); i++)   // môn chưa có lớp trong file -> tạo lớp mặc định
            if (!findClass(courses[i].getCourseId())) classrooms.push_back(Classroom(courses[i].getCourseId(), "TBA", 60));
        return true;
    } catch (const exception& e) { cerr << "Loi doc du lieu input/*.json: " << e.what() << endl; return false; }
}

// Trạng thái đăng ký 1 môn của 1 sinh viên (Module 2 + Module 4, không thay đổi dữ liệu)
static json evalStatus(Student& s, Course& c) {
    Classroom* cl = findClass(c.getCourseId());
    if (cl && inClass(*cl, s.getStudentId())) return {{"state", "enrolled"}, {"text", "Đã đăng ký"}};
    if (s.getCompletedCourseId().count(baseId(c.getCourseId()))) return {{"state", "completed"}, {"text", "Đã hoàn thành"}};
    auto at = s.getAttendingCourse();
    for (unsigned i = 0; i < at.size(); i++)
        if (baseId(at[i].getCourseId()) == baseId(c.getCourseId())) return {{"state", "other"}, {"text", "Đã đăng ký lớp khác của học phần này"}};
    if (cl && classCount(*cl) >= cl->getCapacity()) return {{"state", "full"}, {"text", "Lớp đã đủ sĩ số"}};
    PrereqResult pr = prereq.checkEligibility(c.getCourseId(), s.getCompletedCourseId());   // Module 2
    if (!pr.isEligible) return {{"state", "prereq"}, {"text", pr.message}};
    Student tmp = s; DIYVector<Course> req; req.push_back(c);
    CoutCapture cap; bool ok = RegistrationService::registerCourses(tmp, req);               // Module 4
    if (!ok) return {{"state", "conflict"}, {"text", cap.text()}};
    return {{"state", "ok"}, {"text", "Có thể đăng ký"}};
}

static void reply(httplib::Response& res, const json& j) {
    res.set_header("Access-Control-Allow-Origin", "*");
    res.set_header("Access-Control-Allow-Headers", "Content-Type");
    res.set_content(j.dump(-1, ' ', false, json::error_handler_t::replace), "application/json; charset=utf-8");
}

int main() {
    if (!loadData()) return 1;
    httplib::Server svr;

    svr.Options(".*", [](const httplib::Request&, httplib::Response& res) { reply(res, json::object()); });
    svr.Get("/", [](const httplib::Request&, httplib::Response& res) {
        ifstream f("dangky.html", ios::binary); stringstream b; b << f.rdbuf();
        res.set_content(b.str(), "text/html; charset=utf-8");
    });

    svr.Get("/api/sinhvien", [](const httplib::Request&, httplib::Response& res) {
        lock_guard<mutex> g(mtx); json a = json::array();
        for (unsigned i = 0; i < students.size(); i++) a.push_back({{"id", students[i].getStudentId()}, {"name", students[i].getStudentName()}});
        reply(res, a);
    });

    // Module 1: tìm kiếm theo tiền tố (QuickSort + Binary search) + trạng thái đăng ký (Module 2, 4)
    svr.Get("/api/hocphan", [](const httplib::Request& req, httplib::Response& res) {
        lock_guard<mutex> g(mtx);
        string q = upper(req.get_param_value("q"));
        Student* s = findStudent(req.get_param_value("student_id"));
        DIYVector<Course> list = courses;
        { CoutCapture cap;
          if (q.empty()) quickSort(list, 0, (int)list.size() - 1); else list = searchCourseModule(list, q); }
        json a = json::array();
        for (unsigned i = 0; i < list.size(); i++) {
            Course& c = list[i]; Classroom* cl = findClass(c.getCourseId());
            json pre = json::array(); for (unsigned k = 0; k < c.getPrerequisiteCourses().size(); k++) pre.push_back(c.getPrerequisiteCourses()[k]);
            json st = s ? evalStatus(*s, c) : json{{"state", "none"}, {"text", ""}};
            a.push_back({{"id", c.getCourseId()}, {"name", c.getCourseName()}, {"schedule", c.getSchedule()}, {"prereq", pre},
                         {"count", cl ? classCount(*cl) : 0}, {"capacity", cl ? cl->getCapacity() : 0}, {"state", st["state"]}, {"text", st["text"]}});
        }
        reply(res, a);
    });

    svr.Get("/api/tkb", [](const httplib::Request& req, httplib::Response& res) {
        lock_guard<mutex> g(mtx); json a = json::array();
        Student* s = findStudent(req.get_param_value("student_id"));
        if (s) { auto at = s->getAttendingCourse(); for (unsigned i = 0; i < at.size(); i++) a.push_back({{"id", at[i].getCourseId()}, {"name", at[i].getCourseName()}, {"schedule", at[i].getSchedule()}}); }
        reply(res, a);
    });

    // Module 2 + 4: đăng ký
    svr.Post("/api/dangky", [](const httplib::Request& req, httplib::Response& res) {
        lock_guard<mutex> g(mtx);
        try {
            json b = json::parse(req.body);
            Student* s = findStudent(b.value("student_id", "")); Course* c = findCourse(b.value("course_id", ""));
            if (!s || !c) return reply(res, {{"success", false}, {"error_message", "Không tìm thấy sinh viên hoặc lớp học phần."}});
            json st = evalStatus(*s, *c);
            if (st["state"] != "ok") return reply(res, {{"success", false}, {"error_message", st["text"]}});
            DIYVector<Course> r; r.push_back(*c);
            { CoutCapture cap; RegistrationService::registerCourses(*s, r); }
            findClass(c->getCourseId())->addStudent(*s);
            saveData();
            reply(res, {{"success", true}, {"message", "Đăng ký thành công: " + c->getCourseName() + " [" + c->getCourseId() + "]"}});
        } catch (const exception& e) { reply(res, {{"success", false}, {"error_message", string("Yêu cầu không hợp lệ: ") + e.what()}}); }
    });

    svr.Post("/api/huy", [](const httplib::Request& req, httplib::Response& res) {
        lock_guard<mutex> g(mtx);
        try {
            json b = json::parse(req.body);
            Student* s = findStudent(b.value("student_id", "")); Classroom* cl = findClass(b.value("course_id", ""));
            if (!s || !cl || !inClass(*cl, s->getStudentId())) return reply(res, {{"success", false}, {"error_message", "Sinh viên chưa đăng ký lớp này."}});
            cl->removeStudent(s->getStudentId()); s->removeAttendingCourse(cl->getClassId()); saveData();
            reply(res, {{"success", true}, {"message", "Đã hủy đăng ký " + cl->getClassId()}});
        } catch (const exception& e) { reply(res, {{"success", false}, {"error_message", e.what()}}); }
    });

    // Module 3: truy xuất danh sách lớp
    svr.Get("/api/truyxuatlop", [](const httplib::Request& req, httplib::Response& res) {
        lock_guard<mutex> g(mtx);
        if (!req.has_param("id")) {   // không có id -> danh sách tất cả lớp
            json a = json::array();
            for (unsigned i = 0; i < classrooms.size(); i++) { Course* c = findCourse(classrooms[i].getClassId());
                a.push_back({{"class_id", classrooms[i].getClassId()}, {"name", c ? c->getCourseName() : ""}, {"room", classrooms[i].getClassName()}, {"count", classCount(classrooms[i])}, {"capacity", classrooms[i].getCapacity()}}); }
            return reply(res, a);
        }
        string id = req.get_param_value("id");
        if (!kiemTraDinhDang(id)) return reply(res, {{"ok", false}, {"error", "Mã lớp không hợp lệ! (Yêu cầu: 0 < độ dài <= 13 và có dạng MaHocPhan_SoThuTuLop)"}});
        Classroom* cl = findClass(id);
        if (!cl) return reply(res, {{"ok", false}, {"error", "[LỖI] Không tìm thấy lớp học phần có mã: " + id}});
        Course* c = findCourse(id); json st = json::array(); auto l = cl->getStudentList();
        for (unsigned i = 0; i < l.size(); i++) st.push_back({{"id", l[i].getStudentId()}, {"name", l[i].getStudentName()}});
        reply(res, {{"ok", true}, {"class_id", id}, {"name", c ? c->getCourseName() : ""}, {"room", cl->getClassName()}, {"count", (int)l.size()}, {"capacity", cl->getCapacity()}, {"students", st}});
    });

    cout << "Backend C++ dang chay tai http://localhost:8080  (mo link nay tren trinh duyet)\n";
    svr.listen("localhost", 8080);
    return 0;
}
