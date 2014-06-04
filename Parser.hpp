/*
 * Parser.hpp
 *
 *  Created on: Oct 30, 2013
 *      Author: xhan5
 */


#ifndef XHAN5_PARSER_HPP_
#define XHAN5_PARSER_HPP_
#include "Element.hpp"
#include "Text.hpp"
#include <stack>

namespace xml {

    class Parser {
    public:

        Parser() : Element_Index(0), String_Index(0), Text_Index(0) {
        };
        ~Parser();
        const Element *parse(const char *doc, size_t sz);
        template<typename T> T new_Object(T object, vector<T> object_Vec, unsigned int &index);
        void push_Element2Stack(Element*);
    private:
        unsigned int Element_Index;
        unsigned int String_Index;
        unsigned int Text_Index;

        vector<Element *> Element_Vector;
        vector<String *> String_Vector;
        vector<Text *> Text_Vector;
        stack<Element *>element_Stack; // element match
    };

}

#endif
