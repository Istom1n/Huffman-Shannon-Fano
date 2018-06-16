//
// Created by Ivan Istomin on 04/12/16.
//

#include "Utils.h"

bool to_bool(const char &c) {
    return c != '0';
}

string getFileName(const string& s) {

    char sep = '/';

#ifdef _WIN32
    sep = '\\';
#endif

    size_t i = s.rfind(sep, s.length());
    size_t j = s.rfind('.', s.length());

    if (i != string::npos) {
        return s.substr(i + 1, s.length() - i - (s.length() - j + 1));
    } else if (j != string::npos) {
        return s.substr(0, s.length() - (s.length() - j));
    }

    return "";
}