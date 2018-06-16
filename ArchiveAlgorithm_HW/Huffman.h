//
// Created by Ivan Istomin on 15/11/16.
//

#ifndef ARCHIVEALGORITHM_HW_HUFFMAN_H
#define ARCHIVEALGORITHM_HW_HUFFMAN_H

#include <map>
#include <string>
#include "BitIO.h"

using namespace std;

class Huffman {
    struct Node {
        char32_t data;
        int freq;
        Node* left, * right;

        Node(char32_t data, int freq) {
            left = right = nullptr;
            this->data = data;
            this->freq = freq;
        }

        Node(char32_t data, Node* left, Node* right) {
            this->data = data;
            this->left = left;
            this->right = right;
        }

        bool isLeafNode() {
            return left == nullptr;
        };
    };

    struct compare {
        bool operator()(Node* l, Node* r) {
            return l->freq > r->freq;
        }
    };

public:
    Node* root;
    int countOfOperations = 0;

    void startHuffman(map<char32_t, int>& letters, BitOutput*& writer, map<char32_t, string> &huffmanCodes);
    void makeCodes(struct Node* root, string str, map<char32_t, string>& huffmanCodes);
    void encodeNode(Node* node, BitOutput*& writer);
    Node* readNode(BitInput*& reader);
    void read(BitInput*& reader, basic_ofstream<char32_t>& outputStream, int length);
};


#endif //ARCHIVEALGORITHM_HW_HUFFMAN_H
