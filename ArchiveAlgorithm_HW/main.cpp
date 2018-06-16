/**
 * KDZ-1 in the discipline of Algorithms and data structures
 * Istomin Ivan Viktorovich, BPI154(2), 15.11.2016
 * CLion 2016.3, GCC 6.2
 *
 * The structure of the project:
 *
 *     ArchiveAlgorithm_HW/
 *     |-- BitIO.cpp       (Bitwise reading of the file)
 *     |-- BitIO.h
 *     |-- Huffman.cpp     (The Huffman Algorithm)
 *     |-- Huffman.h
 *     |-- ShannonFano.cpp (The Shannon-Fano Algorithm)
 *     |-- ShannonFano.h
 *     |-- Utils.cpp       (Helper functions)
 *     |-- Utils.h
 *     `-- main.cpp        (Main file)
 *
 * What has been done and what is not:
 * [+] Implementation of the archiving/extracting Huffman algorithm
 * [+] Implementation of archiving/extracting Shannon - Fano algorithm
 * [+] Counting operations
 * [+] The analysis of the results, comprehensiveness of the report
 */

#include <string>
#include <iostream>
#include <fstream>
#include <getopt.h>
#include <map>
#include <algorithm>

#include "Huffman.h"
#include "ShannonFano.h"
#include "Utils.h"

using namespace std;

int countOfOperations = 0;

void HuffmanZip(string path) {
    map<char32_t, int> letters;
    map<char32_t, string> huffmanCodes;
    basic_ifstream<char32_t> ifs(path, ios::in);

    int length = 0;

    char32_t c;
    while (!ifs.eof()) {
        ifs.get(c);
        letters[c]++;
        length++;

        countOfOperations += 4;
    }

    Huffman* h = new Huffman();

    ofstream out(getFileName(path).append(".haff"), ios::binary | ios::out);
    out.write((const char*)& length, sizeof(length));

    BitOutput* bitOutput = new BitOutput(out);

    h->startHuffman(letters, bitOutput, huffmanCodes);

    ifs.clear();
    ifs.seekg(0, ifs.beg);

    while (!ifs.eof()) {
        ifs.get(c);
        bitOutput->writeBits(huffmanCodes[c]);

        countOfOperations += 3;
    }

    bitOutput->forceWrite();

    countOfOperations += h->countOfOperations;
    countOfOperations += bitOutput->countOfOperations;
    countOfOperations += 20; // Number of operations throughout the method

    ifs.close();
    out.close();

    delete h;
    delete bitOutput;

    cout << countOfOperations << endl;
}

void HuffmanUnzip(string path) {
    Huffman* h = new Huffman();

    // Unziped text file
    basic_ofstream<char32_t> out(getFileName(path).append("-unz-h.txt"), ios::out);

    ifstream in(path, ios::binary | ios::in);

    int length;
    in.read((char*)& length, sizeof(length));

    BitInput* bitInput = new BitInput(in);

    h->readNode(bitInput);
    h->read(bitInput, out, length);

    countOfOperations += h->countOfOperations;
    countOfOperations += bitInput->countOfOperations;
    countOfOperations += 13; // Number of operations throughout the method

    out.close();
    in.close();

    delete h;
    delete bitInput;

    cout << countOfOperations << endl;
}

void ShannonFanoZip(string path) {
    map<char32_t, int> letters;
    basic_ifstream<char32_t> ifs(path, ios::in);
    int length = 0;

    char32_t c;
    while (!ifs.eof()) {
        ifs.get(c);
        letters[c]++;
        length++;

        countOfOperations += 4;
    }

    ShannonFano* f = new ShannonFano();
    vector<ShannonFano::Letter> lettersVector;

    for (auto it = letters.begin(); it != letters.end(); ++it) {
        lettersVector.push_back({it->first, it->second});
        countOfOperations += 1;
    }

    sort(lettersVector.begin(), lettersVector.end(), [countOfOperations](const ShannonFano::Letter &p1, const ShannonFano::Letter &p2) {
        countOfOperations += 1;
        return (p1.freq > p2.freq);
    });

    f->startShannonFano(lettersVector, 0, letters.size() - 1);

    ofstream out(getFileName(path).append(".shan"), ios::binary | ios::out);
    out.write((const char*)& length, sizeof(length));

    BitOutput* bitOutput = new BitOutput(out);

    ifs.clear();
    ifs.seekg(0, ifs.beg);

    f->buildTree(lettersVector);
    f->encodeNode(f->root, bitOutput);

    while (!ifs.eof()) {
        ifs.get(c);
        auto letterCode = find_if(lettersVector.begin(), lettersVector.end(),
                                  [c, countOfOperations](const ShannonFano::Letter &p) {
                                      countOfOperations += 1;

                                      return p.letter == c;
                                  });

        for (char u : letterCode->code) {
            bitOutput->writeBit(to_bool(u));

            countOfOperations += 2;
        }

        countOfOperations += 1;
    }

    bitOutput->forceWrite();

    countOfOperations += f->countOfOperations;
    countOfOperations += bitOutput->countOfOperations;
    countOfOperations += 21; // Number of operations throughout the method

    ifs.close();
    out.close();

    delete f;
    delete bitOutput;

    cout << countOfOperations << endl;
}

void ShannonFanoUnzip(string path) {
    ShannonFano* f = new ShannonFano();

    basic_ofstream<char32_t> out(getFileName(path).append("-unz-s.txt"), ios::out);

    ifstream in(path, ios::binary | ios::in);

    int length;
    in.read((char*)& length, sizeof(length));

    BitInput* bitInput = new BitInput(in);

    f->readNode(bitInput);
    f->read(bitInput, out, length);

    countOfOperations += f->countOfOperations;
    countOfOperations += bitInput->countOfOperations;
    countOfOperations += 14; // Number of operations throughout the method

    out.close();
    in.close();

    delete f;
    delete bitInput;

    cout << countOfOperations << endl;
}

int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "");

    int arg = getopt(argc, argv, "h:a:s:u:");

    switch (arg) {
        case 'h':
            HuffmanZip(optarg);
            break;
        case 'a':
            HuffmanUnzip(optarg);
            break;
        case 's':
            ShannonFanoZip(optarg);
            break;
        case 'u':
            ShannonFanoUnzip(optarg);
            break;
        case '?':
            cout << "This argument is invalid." << endl;
            cout << "Please, use:" << endl;
            cout << "-h [--path] \t Huffman Zip" << endl;
            cout << "-a [--path] \t Huffman Unzip" << endl;
            cout << "-s [--path] \t Shannon-Fano Zip" << endl;
            cout << "-u [--path] \t Shannon-Fano Unzip" << endl;
            break;
    };


    return 0;
}