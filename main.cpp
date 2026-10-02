#include "httplib.h"
#include "json.hpp" // File json.hpp trong thư mục lib/ của bạn

using json = nlohmann::json;

int main() {
    httplib::Server svr;

    // Lắng nghe request GET từ HTML
    svr.Get("/api/timkiem", [](const httplib::Request& req, httplib::Response& res) {
        
        // 1. Logic tìm kiếm môn học của bạn ở đây...
        json ket_qua = {
            {{"id", "IT001"}, {"name", "Cấu trúc dữ liệu và giải thuật"}, {"credits", 3}, {"schedule", "Thứ 2, Tiết 1-3"}}
        };

        // 2. DÒNG QUAN TRỌNG NHẤT ĐỂ SỬA LỖI CORS
        res.set_header("Access-Control-Allow-Origin", "*");

        // 3. Trả dữ liệu JSON về cho web HTML
        res.set_content(ket_qua.dump(), "application/json");
    });

    std::cout << "Server C++ dang chay tai http://localhost:8080..." << std::endl;
    svr.listen("localhost", 8080);
    return 0;
}