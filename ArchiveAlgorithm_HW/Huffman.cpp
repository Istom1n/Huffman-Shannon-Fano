//
// Created by Ivan Istomin on 15/11/16.
//

#include "Huffman.h"
#include <queue>

void Huffman::makeCodes(struct Node* root, string str, map<char32_t, string>& huffmanCodes) {
    if (!root) return;

    if (root->data != 0) {
        huffmanCodes.insert({root->data, str});

        countOfOperations += 1;

        return;
    }

    makeCodes(root->left, str + "0", huffmanCodes);
    makeCodes(root->right, str + "1", huffmanCodes);

    countOfOperations += 6;
}

void Huffman::startHuffman(map<char32_t, int>& letters, BitOutput*& writer, map<char32_t, string> &huffmanCodes) {
    struct Node* left, * right, * top;

    priority_queue<Node*, vector<Node*>, compare> queue;

    for (auto it = letters.begin(); it != letters.end(); ++it) {
        queue.push(new Node(it->first, it->second));

        countOfOperations += 1;
    }

    while (queue.size() != 1) {
        left = queue.top();
        queue.pop();

        right = queue.top();
        queue.pop();

        top = new Node(0, left->freq + right->freq);
        top->left = left;
        top->right = right;
        queue.push(top);

        countOfOperations += 9;
    }

    root = queue.top();
    makeCodes(root, "", huffmanCodes);
    encodeNode(root, writer);

    countOfOperations += 7;
}

void Huffman::encodeNode(Node* node, BitOutput*& writer) {
    if (node->isLeafNode()) {
        (*writer).writeBit(1);
        (*writer).writeChar32(node->data);

        countOfOperations += 2;
    } else {
        (*writer).writeBit(0);

        encodeNode(node->left, writer);
        encodeNode(node->right, writer);

        countOfOperations += 3;
    }

    countOfOperations += 1;
}

Huffman::Node* Huffman::readNode(BitInput*& reader) {
    int tmp = (*reader).readBit();

    if (tmp == 1) {
        countOfOperations += 3;

        return new Node((*reader).readChar32(), nullptr, nullptr);
    } else {
        Node* left = readNode(reader);
        Node* right = readNode(reader);

        root = new Node(0, left, right);

        countOfOperations += 5;

        return root;
    }
}

void Huffman::read(BitInput*& reader, basic_ofstream<char32_t>& outputStream, int length) {
    Node* state = root;
    int lengthState = 0;

    while (!reader->eof) {
        while (!state->isLeafNode()) {
            if (reader->readBit() == 0) {
                state = state->left;

                countOfOperations += 1;
            } else {
                state = state->right;

                countOfOperations += 1;
            }

            countOfOperations += 1;
        }

        countOfOperations += 5;

        ++lengthState;
        outputStream << state->data;

        if (lengthState == length - 1) break;

        countOfOperations += 1;

        state = root;
    }
}