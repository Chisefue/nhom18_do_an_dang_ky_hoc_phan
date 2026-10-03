#ifndef _SET_H
#define _SET_H

/* template <typename T>
class DIYSet {
    private:
        T* data;
        int _capacity;
        int _size;

        void expand() {
            if (_capacity == 0) _capacity = 4;
            else _capacity *= 2;
            T* newData = new T[_capacity];
            for (int i = 0; i < _size; i++) {
                newData[i] = data[i];
            } 
            delete[] data;
            data = newData;
        }
    public:
        DIYSet() {
            data = nullptr;
            _capacity = 0;
            _size = 0;
        }
        ~DIYSet() {
            delete[] data;
        }
        int count(const T& val) {
            for (i = 0; i < _size; i++) {
                if (data[i] == val) return 1;
            }
            return 0;
        }

        void insert(const T& val) {
            if (count(val) == 1) return;
            if (_size == _capacity) expand();
            data[_size] = val;
            _size++;
        }


        int size() const { return _size; }
        void clear() { _size = 0; } 

        T* begin() { return data; }
        T* end() { return data + _size; }
        T* find(const T& val) {
            for(int = 0; i < _size; i++) {
                if (data[i] == val) return data + i;
            }
            return end();
        }

        const T* find(const T& val) const {
            for(int = 0; i < _size; i++) {
                if (data[i] == val) return data + i;
            }
            return end();
        }

        const T* begin() const { return data; }
        const T* end() const { return data + _size; } 
};
 */

template <typename T>
class DIYSet {
    private:
        // Cấu trúc Node của Cây Nhị Phân
        struct Node {
            T data;
            Node* left;
            Node* right;
            Node* parr
            Node(T val, Node* parrent = nullptr){
                data = val;
                left = nullptr;
                right = nullptr;
                parr = parrent;
            }
        };
        

        Node* root;
        int tree_size;

        // 1. Hàm đệ quy thêm phần tử (O(log N))
        Node* insertRec(Node* node, const T& val) {
            if (node == nullptr) {
                tree_size++;
                return new Node(val);
            }
            if (val < node->data) {
                node->left = insertRec(node->left, val);
            } else if (val > node->data) {
                node->right = insertRec(node->right, val);
            }
            // Nếu val == node->data (đã tồn tại) -> Bỏ qua, đảm bảo tính duy nhất của Set
            return node;
        }

        // 2. Hàm đệ quy tìm kiếm (O(log N))
        bool searchRec(Node* node, const T& val) const {
            if (node == nullptr) return false;
            if (node->data == val) return true;
            if (val < node->data) return searchRec(node->left, val);
            return searchRec(node->right, val);
        }

        void destroyTree(Node* node) {
        if (node != nullptr) {
            destroyTree(node->left);
            destroyTree(node->right);
            delete node;
        }
    }

    public:
        DIYSet() {
            root = nullptr;
            tree_size = 0;
        }
        ~DIYSet() {
            destroyTree(root);
        }

        void insert(const T& val) {
            if (!root) {
                root = new Node(val);
                tree_size++;
                return;
            }
            
            Node* curr = root;
            Node* parent = nullptr;
            
            
            while (curr != nullptr) {
                parr = curr;
                if (val < curr->data) {
                    curr = curr->right;
                } else if (val > curr->data) {
                    curr = curr->left;
                } else return;
            }
            
            if (val < parr->val) {
                parr->next = new Node(val, parr);
            }
        }

        int count(const T& val) const {
            Node* curr = root;
            while (curr) {
                if (val == curr->data) return 1;
                if (val < curr->data) curr = curr->left;
                else curr = curr->right;
            }
            return 0;
        }

        int size() const { return tree_size; }
        void clear() {
            destroyTree(root);
            root = nullptr;
            tree_size = 0;
        }
        //trỏ
        class Iterator {
        private:
            Node* curr;
        public:
            Iterator(Node* node) {
                curr(node);
            }

            // Lấy dữ liệu
            const T& operator*() const {
                return curr->data;
            }

            // Toán tử so sánh dừng vòng lặp
            bool operator!=(const Iterator& other) const { return curr != other.curr; }

            // Duyệt phần tử kế tiếp
            Iterator& operator++() {
                if (!curr) return *this;

                // Trường hợp 1 Có cây con phải
                if (curr->right != nullptr) {
                    curr = curr->right;
                    while (curr->left != nullptr) curr = curr->left;
                } 
                // Leo lên cha ở trường hợp 2
                else {
                    Node* p = curr->parent;
                    while (p != nullptr && curr == p->right) {
                        curr = p;
                        p = p->parent;
                    }
                    curr = p;
                }
                return *this;
            }
            // Thêm bạn (friend) để truy cập biến private nếu cần thiết
            friend class DIYSet;
        };

        // Hàm begin(): Tìm Node nhỏ nhất (nằm ở tận cùng bên trái)
        Iterator begin() const {
            Node* curr = root;
            if (curr) {
                while (curr->left) curr = curr->left;
            }
            return Iterator(curr);
        }

        // Hàm end(): Điểm kết thúc vòng lặp (nullptr)
        Iterator end() const {
            return Iterator(nullptr);
        }

        Iterator find(const T& val) {
            Node* curr = root;
            while(curr != nullptr) {
                if (val < curr->data) {
                    curr = curr->left;
                } else if (val > curr->data) {
                    curr = curr->right;
                } else Iterator(curr);
            }
            return end();
        }
};
#endif