#include <iostream>
#include <cstdlib>
#include <ctime>

template <typename T>
class Treap {
private:
    struct Node {
        T value;
        int priority;
        int count;
        int size;
        
        Node* left;
        Node* right;
        
        Node(T val) : value(val), priority(rand()), count(1), size(1), left(nullptr), right(nullptr) {}
    };
    
    Node* root;
    
    void updateCount(Node* node) {
        if (node) {
            node->count = node->size + getCount(node->left) + getCount(node->right);
        }
    }
    
    int getCount(const Node* node) const {
        return node ? node->count : 0;
    }
    
    void split(Node* currentNode, T key, Node*& left, Node*& right) {
        if (!currentNode) {
            left = right = nullptr;
        } else if (currentNode->value < key) {
            split(currentNode->right, key, currentNode->right, right);
            left = currentNode;
        } else if (currentNode->value > key) {
            split(currentNode->left, key, left, currentNode->left);
            right = currentNode;
        } else {
            left = currentNode->left;
            right = currentNode;
            currentNode->left = nullptr;
        }
        updateCount(currentNode);
    }
    
    Node* merge(Node* left, Node* right) {
        if (!left) return right;
        if (!right) return left;
        
        if (left->priority > right->priority) {
            left->right = merge(left->right, right);
            updateCount(left);
            return left;
        } else {
            right->left = merge(left, right->left);
            updateCount(right);
            return right;
        }
    }
    
    const Node* find(const Node* node, T value) const {
        if (!node) return nullptr;
        if (node->value == value) return node;
        if (value < node->value) return find(node->left, value);
        return find(node->right, value);
    }
    
    Node* find(Node* node, T value) {
        if (!node) return nullptr;
        if (node->value == value) return node;
        if (value < node->value) return find(node->left, value);
        return find(node->right, value);
    }
    
    void clear(Node* node) {
        if (node) {
            clear(node->left);
            clear(node->right);
            delete node;
        }
    }
    
public:
    Treap() : root(nullptr) {
        srand(time(nullptr));
    }
    
    ~Treap() {
        clear(root);
    }
    
    void insert(T value) {
        Node* left = nullptr;
        Node* right = nullptr;
        split(root, value, left, right);
        
        Node* existing = find(right, value);
        if (existing) {
            existing->size++;
            existing->count++;
        } else {
            Node* newNode = new Node(value);
            right = merge(newNode, right);
        }
        
        root = merge(left, right);
    }
    
    bool remove(T value) {
        Node* left = nullptr;
        Node* middle = nullptr;
        Node* right = nullptr;
        
        split(root, value, left, middle);
        split(middle, value + 1, middle, right);
        
        bool removed = false;
        if (middle) {
            if (middle->size > 1) {
                middle->size--;
                middle->count--;
                removed = true;
            } else {
                delete middle;
                middle = nullptr;
                removed = true;
            }
        }
        
        root = merge(left, merge(middle, right));
        return removed;
    }
    
    int countElements() const {
        return getCount(root);
    }
    
    int countValue(T value) const {
        const Node* node = find(root, value);
        return node ? node->size : 0;
    }
};

// int main() {
//     Treap<int> treap;
    
//     treap.insert(5);
//     treap.insert(3);
//     treap.insert(7);
//     treap.insert(5);
//     treap.insert(5);
//     treap.insert(7);
    
//     std::cout << "Total elements: " << treap.countElements() << std::endl; // 6
//     std::cout << "Count of 5: " << treap.countValue(5) << std::endl;      // 3
//     std::cout << "Count of 7: " << treap.countValue(7) << std::endl;      // 2
//     std::cout << "Count of 3: " << treap.countValue(3) << std::endl;      // 1
    
//     treap.remove(5);
//     std::cout << "\nAfter removing one 5:" << std::endl;
//     std::cout << "Total elements: " << treap.countElements() << std::endl; // 5
//     std::cout << "Count of 5: " << treap.countValue(5) << std::endl;      // 2
    
//     treap.remove(3);
//     std::cout << "\nAfter removing 3:" << std::endl;
//     std::cout << "Total elements: " << treap.countElements() << std::endl; // 4
//     std::cout << "Count of 3: " << treap.countValue(3) << std::endl;      // 0
    
//     return 0;
// }