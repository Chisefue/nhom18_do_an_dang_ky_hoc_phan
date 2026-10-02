#ifndef _DIYVector_H
#define _DIYVector_H
#include <stdexcept>

template <typename T>
class DIYVector { 
    private:
        T* arr;
        unsigned int _capacity;
        unsigned int _size;

        void expand() {
            if (_capacity == 0) _capacity = 1;
            else _capacity = _capacity * 2;
            T* tmp = new T[_capacity];
            for (int i = 0; i < _size; i++) {
                tmp[i] = arr[i];
            }
            delete[] arr;
            arr = tmp;
        }
    public: 
        DIYVector() {
            arr = nullptr;
            _capacity = 0;
            _size = 0;
        }
        ~DIYVector() {
            delete[] arr;
        }

        DIYVector(const DIYVector& otherDIYVector) { //Sao chep DIYVector
            _capacity = otherDIYVector._capacity;
            _size = otherDIYVector._size;
            arr = new T[_capacity];
            for (int i = 0; i < _size; i++) {
                arr[i] = otherDIYVector.arr[i];
            }
        }
        const DIYVector& operator=(const DIYVector &otherDIYVector) {
            if (this != &otherDIYVector) {
                delete[] arr;
                _capacity = otherDIYVector._capacity;
                _size = otherDIYVector._size;
                arr = new T[_capacity];
                for (int i = 0; i < _size; i++) {
                    arr[i] = otherDIYVector.arr[i];
                }
            }
            return *this;
        }

        void push_back(const T& val) {
            if (_size >= _capacity) {
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
        unsigned int capacity() const { return _capacity; }
        bool empty() const { return _size == 0; }
        void clear()     {
            _size = 0;
        }

        T* begin() { return arr; }
        T* end() { return arr + _size; }

        const T* begin() const { return arr; }
        const T* end() const {return arr + _size;}
};

#endif