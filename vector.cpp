#include <stdexcept>

template <typename T> 
class vector {
    private:
        T* arr;
        unsigned int capacity;
        unsigned int _size;

        void expand() {
            if (capacity == 0) capacity = 1;
            else capacity = capacity * 2;
            T* tmp = new T[capacity];
            for (int i = 0; i < _size; i++) {
                tmp[i] = arr[i];
            }
            delete[] arr;
            arr = tmp;
        }
    public: 
        vector() {
            arr = nullptr;
            capacity = 0;
            _size = 0;
        }
        ~vector() {
            delete[] arr;
        }

        vector(vector& otherVector) { //Sao chep vector
            capacity = otherVector.capacity;
            _size = otherVector._size;
            arr = new T[capacity];
            for (int i = 0; i < _size; i++) {
                arr[i] = otherVector.arr[i];
            }
        }
        const vector& operator=(vector &otherVector) {
            if (this != &otherVector) {
                delete[] arr;
                capacity = otherVector.capacity;
                _size = otherVector._size;
                arr = new T[capacity];
                for (int i = 0; i < _size; i++) {
                    arr[i] = otherVector.arr[i];
                }
            }
            return *this;
        }

        void push_back(const T& val) {
            if (_size >= capacity) {
                expand();
            }
            arr[_size] = val;
            _size++;
        }
        void pop_back() {
            if (_size > 0) _size--;
        }

        T& operator[](unsigned int index) {
            if (index >= _size) throw std::out_of_range("Phần tử nằm ngoài mảng");
            return arr[index];
        }
        const T& operator[](unsigned int index) const {
            if (index >= _size) throw std::out_of_range("Phần tử nằm ngoài mảng");
            return arr[index];
        }

        unsigned int size() const { return _size; }
        unsigned int capacity() const { return capacity; }
        bool empty() const { return _size == 0; }
        void clear()     {
            _size = 0;
        }

        T* begin() { return arr; }
        T* end() { return arr + _size; }
};
