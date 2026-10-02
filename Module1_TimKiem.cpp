#include "Class.h"

using namespace std;


// ==========================================
// HÀM 3: HỖ TRỢ TÌM KIẾM (PREFIX CHECK)
// ==========================================
bool isPrefix(const string& S, const string& courseId) {
    // Nếu chuỗi tìm kiếm dài hơn mã học phần, chắc chắn không phải tiền tố
    if (S.length() > courseId.length()) return false;

    // So sánh từng ký tự một từ đầu chuỗi
    for (int i = 0; i < S.length(); i++) {
        if (S[i] != courseId[i]) {
            return false; // Trật một ký tự là sai luôn
        }
    }
    return true; // Nếu khớp hết các ký tự của S
}

// ==========================================
// HÀM 1: THUẬT TOÁN SẮP XẾP (QUICK SORT)
// ==========================================
// Hàm comparator: Trả về true nếu mã học phần a đứng trước b theo bảng chữ cái A-Z
bool compareCourse(const Course& a, const Course& b) {
    return a.getCourseId() < b.getCourseId();
}

// Hàm chia mảng (Partition) phục vụ cho Quick Sort
int partition(DIYVector<Course>& arr, int low, int high) {
    Course pivot = arr[high]; // Chọn phần tử cuối làm mốc (pivot)
    int i = (low - 1); // Chỉ số của phần tử nhỏ hơn pivot

    for (int j = low; j <= high - 1; j++) {
        // Nếu mã học phần hiện tại nhỏ hơn pivot (đứng trước theo A-Z)
        if (compareCourse(arr[j], pivot)) {
            i++;
            // Đổi chỗ đưa phần tử nhỏ hơn về bên trái
            Course tmp = arr[i];
            arr[i] = arr[j];
            arr[j] = tmp;
        }
    }
    // Đưa pivot về đúng vị trí ở giữa
    Course temp = arr[i + 1];
    arr[i + 1] = arr[high];
    arr[high] = temp;

    return (i + 1); // Trả về vị trí của pivot
}

// Hàm Quick Sort đệ quy
void quickSort(DIYVector<Course>& arr, int low, int high) {
    if (low < high) {
        // pi là chỉ số chia mảng, arr[pi] đã nằm đúng vị trí
        int pi = partition(arr, low, high);

        // Đệ quy sắp xếp nửa bên trái và nửa bên phải của pivot
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

// ==========================================
// HÀM 2: THUẬT TOÁN TÌM KIẾM (BINARY SEARCH)
// ==========================================
// Tìm vị trí ĐẦU TIÊN khớp hoàn toàn hoặc khớp tiền tố
int findFirstMatch(const DIYVector<Course>& arr, const string& S) {
    int left = 0;
    int right = arr.size() - 1;
    int first_pos = -1; // Biến lưu kết quả, mặc định -1 là không tìm thấy

    while (left <= right) {
        int mid = left + (right - left) / 2; // Tìm điểm giữa mảng
        string midId = arr[mid].getCourseId();

        if (isPrefix(S, midId)) {
            first_pos = mid; // Đã tìm thấy một phần tử khớp!
            // QUAN TRỌNG: Không dừng lại. Tiếp tục tìm bên nửa TRÁI 
            // để xem có phần tử nào khớp mà xuất hiện sớm hơn không.
            right = mid - 1;
        }
        else if (midId < S) {
            // Nếu phần tử ở giữa đứng trước S theo bảng chữ cái, 
            // thì mục tiêu phải nằm ở nửa BÊN PHẢI.
            left = mid + 1;
        }
        else {
            // Nếu phần tử ở giữa đứng sau S, mục tiêu nằm ở nửa BÊN TRÁI.
            right = mid - 1;
        }
    }
    return first_pos;
}

// ==========================================
// MODULE MAIN: GỌI CÁC HÀM VÀ XỬ LÝ RÀNG BUỘC
// ==========================================
DIYVector<Course> searchCourseModule(DIYVector<Course>& arr, string S) {
    DIYVector<Course> found;
    int count = 0;
    // 1. Ràng buộc đầu vào: 0 < S.length() < 10
    if (S.length() <= 0 || S.length() > 13) {
        cout << "Thong bao: Khong tim thay hoc phan (Do dai chuoi khong hop le)." << endl;
        return {};
    }

    // 2. Phải đảm bảo dữ liệu đã được sắp xếp A-Z trước khi tìm kiếm nhị phân
    // Trong thực tế, bạn chỉ cần gọi quickSort 1 lần sau khi load file JSON.
    quickSort(arr, 0, arr.size() - 1);

    // 3. Tìm vị trí đầu tiên khớp với chuỗi S
    int startIndex = findFirstMatch(arr, S);

    // 4. Xuất kết quả (Output)
    if (startIndex == -1) {
        cout << "Thong bao: Khong tim thay hoc phan" << endl;
    }
    else {
        cout << "\nDanh sach cac hoc phan tim thay cho tu khoa '" << S << "':\n";
        // Bắt đầu từ vị trí tìm được, in ra tất cả các học phần có cùng tiền tố S
        for (int i = startIndex; i < arr.size(); i++) {
            if (isPrefix(S, arr[i].getCourseId())) {
                count++;
                cout << count << ". " << arr[i].getCourseId() << " " << arr[i].getCourseName() << endl;
                found.push_back(arr[i]);
            }
            else {
                // Do mảng đã được sắp xếp, nếu gặp một mã không còn chứa tiền tố S nữa
                // thì các mã phía sau chắc chắn cũng không chứa, nên ta ngắt vòng lặp (break) luôn cho tối ưu.
                break;
            }
        }
        
    }
    return found;
}
