#include <iostream>
#include <algorithm>
using namespace std;

struct Node {
    int key;
    Node* left;
    Node* right;
    int height;
    int size;
};

int height(Node* node) {
    if (node == nullptr)
        return 0;
    return node->height;
}

int size(Node* node) {
    if (node == nullptr)
        return 0;
    return node->size;
}

Node* newNode(int key) {
    Node* node = new Node();
    node->key = key;
    node->left = nullptr;
    node->right = nullptr;
    node->height = 1;
    node->size = 1;
    return node;
}

Node* rightRotate(Node* y) {
    Node* x = y->left;
    Node* T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    y->size = size(y->left) + size(y->right) + 1;
    x->size = size(x->left) + size(x->right) + 1;

    return x;
}

Node* leftRotate(Node* x) {
    Node* y = x->right;
    Node* T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    x->size = size(x->left) + size(x->right) + 1;
    y->size = size(y->left) + size(y->right) + 1;

    return y;
}

int getBalance(Node* node) {
    if (node == nullptr)
        return 0;
    return height(node->left) - height(node->right);
}

Node* insert(Node* node, int key) {
    if (node == nullptr)
        return newNode(key);

    if (key < node->key)
        node->left = insert(node->left, key);
    else if (key > node->key)
        node->right = insert(node->right, key);
    else 
        return node;

    node->height = 1 + max(height(node->left), height(node->right));
    node->size = 1 + size(node->left) + size(node->right);

    int balance = getBalance(node);

    
    if (balance > 1 && key < node->left->key)
        return rightRotate(node);

    if (balance < -1 && key > node->right->key)
        return leftRotate(node);

    if (balance > 1 && key > node->left->key) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    if (balance < -1 && key < node->right->key) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}

Node* kthSmallest(Node* root, int k) {
    if (root == nullptr)
        return nullptr;

    int leftSize = size(root->left);

    if (k == leftSize + 1) 
        return root;
    else if (k <= leftSize) 
        return kthSmallest(root->left, k);
    else 
        return kthSmallest(root->right, k - leftSize - 1);
}

void inorder(Node* root) {
    if (root != nullptr) {
        inorder(root->left);
        cout << root->key << " ";
        inorder(root->right);
    }
}

// int main() {
//     Node* root = nullptr;
    
//     int arr[] = {10, 20, 30, 40, 50, 25};
//     int n = sizeof(arr) / sizeof(arr[0]);
    
//     for (int i = 0; i < n; i++) {
//         root = insert(root, arr[i]);
//     }
    
//     cout << "Inorder traversal of the constructed AVL tree is: ";
//     inorder(root);
//     cout << endl;
    
//     int k = 3;
//     Node* kth = kthSmallest(root, k);
//     if (kth != nullptr) {
//         cout << "The " << k << "-th smallest element is " << kth->key << endl;
//     } else {
//         cout << "Invalid k" << endl;
//     }
    
//     return 0;
// }