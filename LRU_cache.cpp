#include <unordered_map>

template <typename T>

class LRU_cache
{
private:
    struct Node
    {
        size_t key;
        T value;
        Node* prev=nullptr;
        Node* next=nullptr;
    };
    size_t size_;
    std::unordered_map<size_t, Node*> keys;
    Node *head=nullptr, *tail=nullptr;

    void move_to_tail(Node* node) 
    {
        if (node == tail) return;
        
        if (node == head) {
            head = head->next;
            if (head) head->prev = nullptr;
        } 
        else {
            node->prev->next = node->next;
            if (node->next) node->next->prev = node->prev;
        }
        
        tail->next = node;
        node->prev = tail;
        node->next = nullptr;
        tail = node;
    }

    void remove_head() {
        if (!head) return;
        
        Node* to_remove = head;
        keys.erase(to_remove->key);
        
        if (head == tail) {
            head = tail = nullptr;
        } else {
            head = head->next;
            head->prev = nullptr;
        }
        
        delete to_remove;
    }
public:
    LRU_cache(size_t size) {size_=size;}

    ~LRU_cache() {
        while (head) {
            Node* cur = head;
            head = head->next;
            delete cur;    
        }
    }

    void add(size_t key, T value) {
        if (keys.find(key) != keys.end()) {
            Node* existing = keys[key];
            existing->value = value;
            move_to_tail(existing);
            return;
        }
        
        if (keys.size() >= size_) {
            remove_head();
        }
        
        if (!tail) {
            head=new Node {key,value};
            tail=head;
        } else {
            tail->next = new Node {key,value};
            tail = tail->next;
        }
        keys[key]=tail; 
    }

    void delet(size_t key) 
    {
         auto it = keys.find(key);
        if (it == keys.end()) return;
        
        Node* to_delete = it->second;
        
        if (to_delete == head) {
            remove_head();
            return;
        }
        
        if (to_delete == tail) {
            tail = tail->prev;
            tail->next = nullptr;
        } 
        else {
            to_delete->prev->next = to_delete->next;
            to_delete->next->prev = to_delete->prev;
        }
        
        keys.erase(key);
        delete to_delete;
    }

    T get(size_t key) {
        auto it = keys.find(key);
        if (it == keys.end()) {
            throw std::out_of_range("Key not found in cache");
        }
        
        move_to_tail(it->second);
        return it->second->value;
    }
};
