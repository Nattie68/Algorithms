#include <iostream>
#include <queue>
#include <unordered_map>
#include <vector>

struct Node {
    char ch;
    int freq;
    Node* left;
    Node* right;
    Node(char ch, int freq)
        : ch(ch), freq(freq), left(nullptr), right(nullptr) {}
    Node(char ch, int freq, Node* left, Node* right)
        : ch(ch), freq(freq), left(left), right(right) {}
};

struct compare {
    bool operator()(Node* l, Node* r) {
        return l->freq > r->freq;
    }
};

void printCodes(Node* root, std::string str,
                std::unordered_map<char, std::string>& huffmanCode)
{
    if (root == nullptr)
        return;

    if (!root->left && !root->right) {
        if (str.empty()) {
            huffmanCode[root->ch] = "0";
        } else {
            huffmanCode[root->ch] = str;
        }
    }

    printCodes(root->left, str + "0", huffmanCode);
    printCodes(root->right, str + "1", huffmanCode);
}

void buildHuffmanTree(std::string text)
{
    if (text.empty()) return;

    std::unordered_map<char, int> freq;
    for (char ch : text) {
        freq[ch]++;
    }

    std::priority_queue<Node*, std::vector<Node*>, compare> pq;
    for (auto pair : freq) {
        pq.push(new Node(pair.first, pair.second));
    }

    while (pq.size() > 1) {
        Node* left = pq.top(); 
        pq.pop();
        Node* right = pq.top(); 
        pq.pop();
        int sum = left->freq + right->freq;
        pq.push(new Node('\0', sum, left, right));
    }

    Node* root = pq.top();
    std::unordered_map<char, std::string> huffmanCode;
    printCodes(root, "", huffmanCode);

    int encodedSize = 0;
    for (char ch : text) {
        encodedSize += huffmanCode[ch].size();
    }

    std::cout << huffmanCode.size() << " " << encodedSize << "\n";
    
    for (auto pair : huffmanCode) {
        std::cout << pair.first << ": " << pair.second << "\n";
    }

    for (char ch : text) {
        std::cout << huffmanCode[ch];
    }
    std::cout << "\n";
}

int main()
{
    std::string text;
    std::cin >> text;
    buildHuffmanTree(text);
    return 0;
}