/*
 * String.cpp
 *
 *  Created on: Oct 30, 2013
 *      Author: xhan5
 */


#include "String.hpp"
#include <iostream>
using namespace std;

namespace xml {
    // non-member function

    bool is_Equal(const char *str1, size_t size1, const char *str2, size_t size2) {
        if (size1 != size2) {
            return false;
        } else
            for (size_t i = 0; i < size2; i++) {
                if (str1[i] != str2[i])
                    return false;
            }
        return true;
    }

    //Comparision with std::string

    bool operator==(const std::string &str1, const String &str2) {
        return is_Equal(str1.c_str(), str1.size(), str2.get_StringContent(), str2.get_StringSize());
    }

    bool operator==(const String &str1, const std::string &str2) {
        return operator==(str2, str1);
    }

    bool operator!=(const std::string &str1, const String &str2) {
        return !operator==(str1, str2);
    }

    bool operator!=(const String &str1, const std::string &str2) {
        return !operator==(str2, str1);
    }

    // comparision with C String

    bool operator==(const char *str1, const String &str2) {
        return is_Equal(str1, str2.get_StringSize(), str2.get_StringContent(), str2.get_StringSize());
    }

    bool operator==(const String &str1, const char *str2) {
        return operator==(str2, str1);
    }

    bool operator!=(const char *str1, const String &str2) {
        return !operator==(str1, str2);
    }

    bool operator!=(const String &str1, const char *str2) {
        return !operator==(str2, str1);
    }

    //Output operator

    ostream &operator<<(ostream &os, const String &str) {
        string temp_str = str;
        os << temp_str;
        return os;
    }


    //  member function

    // constructor

    String::String() : Str_size(0), Str_content(NULL) {
    }

    // Copy constructor.

    String::String(const String &str) : Str_size(str.get_StringSize()), Str_content(str.get_StringBegin()) {
    }


    // Conversion to std::string.

    String::operator std::string() const {
        std::string str(Str_content, Str_size);
        return str;
    }

    // Assignment.

    String& String::operator=(const String &str) {
        this->Str_content = str.get_StringBegin();
        this->Str_size = str.get_StringSize();
        return *this;
    }

    bool String::operator==(const String &str) {
        return is_Equal(Str_content, Str_size, str.get_StringContent(), str.get_StringSize());
    }

    bool String::operator<(const String &str) {
        return (Str_content < str.get_StringContent());
    }

    const char* String::get_StringBegin() const {
        return Str_content;
    }

    const char* String::get_StringEnd() const {
        return Str_content + Str_size;
    }

    const char* String::get_StringContent() const {
        return Str_content;
    }

    size_t String::get_StringSize() const {
        return Str_size;
    }

    void String::Init() {
        Str_content = NULL;
        Str_size = 0;
    }

    void String::set_StringBegin(const char *start) {
        Str_content = start;
        Str_size = 1;
    }

    void String::set_StringEnd(const char *end) {
        Str_size = end - Str_content;
    }

    void String::increase_Stringsize() {
        Str_size++;
    }

    void String::set_StringSize(size_t sz) {
        Str_size = sz;
    }

}
