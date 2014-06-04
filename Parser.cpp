/*
 * Parser.cpp
 *
 *  Created on: Oct 30, 2013
 *      Author: xhan5
 */

#include "Parser.hpp"
#include "Element.hpp"
#include <assert.h>

#define DEBUG_FLAG 0
#define DEBUG2_FLAG 0
namespace xml {
    using namespace std;

    const char *input_debug;

    void debug(bool print, const string& message, size_t curr_Index) {
        if (print)
            cout << message << "  " << "Index: " << input_debug[curr_Index] << endl;
    }

    void debug2(bool print, Element* curr_Element, size_t curr_Index) {
        if (print) {
            if (curr_Element) {
                cout << *(curr_Element->get_ElementContent()) << "  Index:" << curr_Index << endl;
            } else {
                cout << "Element is NULL" << endl;
            }
        }
    }

    void set_the_ElementEnd(String* element_Name, Element* curr_Element, const char *input, size_t curr_Index) {
        element_Name->set_StringEnd(input + curr_Index);
        curr_Element->set_ElementNameContent(element_Name);
        curr_Element->set_ElementNameEnd(input + curr_Index);
    }

    // memeber function

    void Parser::push_Element2Stack(Element* curr_Element) {
        if (!element_Stack.empty()) {
            // if the Stack is not empty, copy the all parent namespace to child
            curr_Element->set_MultiMapVec(element_Stack.top() -> get_Nmspace_MapVec());
        } else {
            // if the Stack is empty, it means it is a new element
            // set it own namespace
            curr_Element -> set_SingleMapVec();
        }
        element_Stack.push(curr_Element);
    }

    template<typename T> T Parser::new_Object(T object, vector<T> object_Vec, unsigned int &index) {
        for (size_t i = index; i < object_Vec.size(); i++) {
            if (object_Vec[i] != NULL) {
                object_Vec[i]->Init();
                index++;
                return object_Vec[i];
            }
        }

        object_Vec.push_back(object);
        index++;
        return object;
    }

    Parser::~Parser() {
        // only in the destructor function to release the space
        for (size_t i = 0; i < Element_Vector.size(); i++) {
            delete Element_Vector[i];
        }
        for (size_t i = 0; i < String_Vector.size(); i++) {
            delete String_Vector[i];
        }
        for (size_t i = 0; i < Text_Vector.size(); i++) {
            delete Text_Vector[i];
        }

        //while(!element_Stack.empty()) element_Stack.pop();
    }

    const Element* Parser::parse(const char *input, size_t sz) {
        size_t curr_Index = 0;
        Text *curr_Text = NULL;
        String *curr_String = NULL, *element_Name = NULL, *key = NULL, *uri = NULL;
        String *end_MatchContent = NULL;
        Element *curr_Element = NULL, *root = NULL, *parent = NULL;

        string xmlns("xmlns"); //key word for xmlns
        string commnet("--"); //key word for com_begin

        bool is_Root = true;

        input_debug = input;

        enum {
            ST_XML_START = 1,
            ST_ELEMENT_START,
            ST_COMMENT_START,
            ST_NAMESPACE_START,
            ST_NAMESPACE_NAME,
            ST_NAMESPACE_URI_START,
            ST_NAMESPACE_URI_CONTENT,
            ST_NAMESPACE_URI_END, // will start other namespace
            ST_XMLNS_START,
            ST_ELEMENT_NAME_OR_NAMESPACE_START,
            ST_ELEMENT_NAME,
            ST_COMMENT_END,
            ST_ELEMENT_END,
            ST_ELEMENT_NAME_START,
            ST_SPACE_AFTER_ELEMENT_NAME,
            ST_MATCH_ELEMENT_NAME,
            ST_TEXT,
            ST_ERROR
        } state = ST_XML_START;

        while (curr_Index <= sz) {
            char ch = input[curr_Index];
            switch (state) {
                case ST_XML_START:
                {
                    debug(DEBUG_FLAG, "ST_XML_START", curr_Index);
                    while (isspace(ch)) {
                        ch = input[++curr_Index];
                    }

                    if (ch == '<') {
                        state = ST_ELEMENT_START;
                    } else
                        if (ch == ' ') {
                        debug(DEBUG_FLAG, "ERROR1_IN_ST_XML_START", curr_Index);
                        state = ST_ERROR;
                    } else {
                        debug(DEBUG_FLAG, "ERROR2_IN_ST_XML_START", curr_Index);
                        state = ST_ERROR;
                    }
                }
                    break;

                case ST_ELEMENT_START:
                {
                    debug(DEBUG_FLAG, "ST_ELEMENT_START", curr_Index);
                    if (ch == '!') {
                        state = ST_COMMENT_START;
                    } else
                        if (ch == '_' || isalpha(ch)) {
                        String *ptr_String = new String();
                        element_Name = new_Object(ptr_String, String_Vector, String_Index);
                        element_Name->set_StringBegin(input + curr_Index);

                        if (is_Root) {
                            //allocate element for root
                            Element *ptr_Root = new Element();
                            curr_Element = new_Object(ptr_Root, Element_Vector, Element_Index);
                            root = curr_Element;
                            is_Root = false;
                        } else {
                            Element *ptr_Element = new Element();
                            curr_Element = new_Object(ptr_Element, Element_Vector, Element_Index);
                        }

                        curr_Element->set_ElementNameBegin(input + curr_Index);
                        curr_Element->set_NamespaceBegin(" ");
                        state = ST_ELEMENT_NAME_OR_NAMESPACE_START;
                    } else if (ch == '/') {
                        state = ST_MATCH_ELEMENT_NAME; // check the stack
                    } else {
                        debug(DEBUG_FLAG, "ERROR ST_ELEMENT_START", curr_Index);
                        state = ST_ERROR;
                    }

                }
                    break;

                case ST_MATCH_ELEMENT_NAME:
                {
                    debug(DEBUG_FLAG, "ST_MATCH_ELEMENT_NAME", curr_Index);

                    String *ptr_String = new String(); // get the string for </XXXX>
                    end_MatchContent = new_Object(ptr_String, String_Vector, String_Index);
                    end_MatchContent -> set_StringBegin(input + curr_Index);

                    while (ch == ':' || ch == '_' || isalnum(ch)) {
                        ch = input[++curr_Index];
                    }

                    end_MatchContent -> set_StringEnd(input + curr_Index);

                    // check the stack to match or not

                    Element* test_element = element_Stack.top();

                    if (!(*(test_element->get_ElementContent()) == *end_MatchContent)) {
                        cout << "Error in XML input, exiting..." << endl;
                        exit(1);
                    } else {
                        if (!element_Stack.empty()) {
                            assert(root != NULL);
                            if (root == test_element) {
                                element_Stack.pop();
                                return test_element;
                            } else {
                                element_Stack.pop();
                                parent = element_Stack.top();
                                parent ->add_Child((Node *) test_element);
                                //test_element = NULL;
                            }
                        }
                    }
                    state = ST_ELEMENT_END;
                }
                    break;

                case ST_ELEMENT_NAME_OR_NAMESPACE_START:
                {
                    debug(DEBUG_FLAG, "ST_ELEMENT_NAME_OR_NAMESPACE_START", curr_Index);
                    while (isalnum(ch) || ch == '_') {
                        ch = input[++curr_Index];

                    }

                    if (ch == ':') {
                        curr_Element->set_ElementNameEnd(input + curr_Index);
                        curr_Element->set_Namespace_Prefix(curr_Element->name());
                        state = ST_ELEMENT_NAME_START;
                        // if it is a :
                        // before the : is the prefix
                        // after the : is the element name

                    } else
                        if (isspace(ch)) {
                        set_the_ElementEnd(element_Name, curr_Element, input, curr_Index);
                        state = ST_SPACE_AFTER_ELEMENT_NAME;
                    } else
                        if (ch == '>') {
                        // push the element name to stack
                        set_the_ElementEnd(element_Name, curr_Element, input, curr_Index);
                        debug2(DEBUG2_FLAG, curr_Element, curr_Index);
                        push_Element2Stack(curr_Element);
                        curr_Element = NULL;
                        state = ST_ELEMENT_END;
                    } else {
                        debug(DEBUG_FLAG, "ST_ELEMENT_NAME_OR_NAMESPACE_START", curr_Index);
                        state = ST_ERROR;
                    }
                }
                    break;

                case ST_ELEMENT_NAME_START:
                {
                    debug(DEBUG_FLAG, "ST_ELEMENT_NAME_START", curr_Index);
                    if (isspace(ch)) {
                        --curr_Index;
                        state = ST_SPACE_AFTER_ELEMENT_NAME;
                    }
                    if (isalnum(ch) || ch == '_') {
                        curr_Element->set_ElementNameBegin(input + curr_Index);
                        state = ST_ELEMENT_NAME;
                    } else {
                        debug(DEBUG_FLAG, "ERROR_IN_ST_ELEMENT_NAME_START", curr_Index);
                        state = ST_ERROR;
                    }

                }
                    break;

                case ST_ELEMENT_NAME:
                {
                    debug(DEBUG_FLAG, "ST_ELEMENT_NAME", curr_Index);

                    while (isalnum(ch) || ch == '_') {
                        ch = input[++curr_Index];
                    }

                    if (isspace(ch)) {
                        set_the_ElementEnd(element_Name, curr_Element, input, curr_Index);
                        debug2(DEBUG2_FLAG, curr_Element, curr_Index);
                        element_Name = NULL;
                        state = ST_SPACE_AFTER_ELEMENT_NAME;
                    }
                    else if (ch == '>') {
                        set_the_ElementEnd(element_Name, curr_Element, input, curr_Index);
                        debug2(DEBUG2_FLAG, curr_Element, curr_Index);
                        push_Element2Stack(curr_Element);
                        curr_Element = NULL;
                        state = ST_ELEMENT_END;
                    }
                    else {
                        debug(DEBUG_FLAG, "ERROR_IN_ST_ELEMENT_NAME", curr_Index);
                        state = ST_ERROR;
                    }
                }
                    break;

                case ST_COMMENT_START:
                {
                    debug(DEBUG_FLAG, "ST_COMMENT_START", curr_Index);
                    int index = 0;
                    while (index < 2) // check the "--"
                    {
                        if (commnet[index] == input[curr_Index++]) {
                            index++;
                        } else {
                            debug(DEBUG_FLAG, "ERROR_IN_ST_COMMENT_START", curr_Index);
                            state = ST_ERROR;
                            break;
                        }
                    }
                    if (index == 2)
                        state = ST_COMMENT_END;
                }
                    break;


                case ST_SPACE_AFTER_ELEMENT_NAME:
                {
                    debug(DEBUG_FLAG, "ST_SPACE_AFTER_ELEMENT_NAME", curr_Index);
                    while (isspace(ch)) {
                        ch = input[++curr_Index];
                    }

                    if (ch == 'x') {
                        --curr_Index;
                        state = ST_XMLNS_START;
                    } else if (ch == '>') {
                        push_Element2Stack(curr_Element);
                        state = ST_ELEMENT_END;
                    } else
                        if (isgraph(ch)) {
                        // if the there are other word follow the space
                        // Spaces not allowed in element name
                        debug(DEBUG_FLAG, "ERROR1_IN_ST_XMLNS_START", curr_Index);
                        state = ST_ERROR;
                    }

                }
                    break;

                case ST_XMLNS_START:
                {
                    debug(DEBUG_FLAG, "ST_XMLNS_START", curr_Index);
                    int index = 0;
                    while (index < 5) {
                        if (xmlns[index] == input[curr_Index++]) {
                            index++;
                        } else {
                            debug(DEBUG_FLAG, "ERROR1_IN_ST_XMLNS_START", curr_Index);
                            state = ST_ERROR;
                            break;
                        }
                    }

                    ch = input[curr_Index];
                    if (ch == ':') {
                        state = ST_NAMESPACE_START;
                    } else {
                        debug(DEBUG_FLAG, "ERROR2_IN_ST_XMLNS_START", curr_Index);
                        state = ST_ERROR;
                    }
                }
                    break;

                case ST_NAMESPACE_START:
                {
                    debug(DEBUG_FLAG, "ST_NAMESPACE_START", curr_Index);
                    if (ch == ' ') {
                        state = ST_ERROR;
                    } else
                        if (isalnum(ch) || ch == '_') {
                        String *ptr_String = new String();
                        key = new_Object(ptr_String, String_Vector, String_Index);
                        key->set_StringBegin(input + curr_Index);
                        key->set_StringSize(1);
                        state = ST_NAMESPACE_NAME;
                    } else {
                        debug(DEBUG_FLAG, "ERROR_ST_NAMESPACE_START", curr_Index);
                        state = ST_ERROR;
                    }
                }
                    break;

                case ST_NAMESPACE_NAME:
                {
                    debug(DEBUG_FLAG, "ST_NAMESPACE_NAME", curr_Index);
                    while (isalnum(ch) || ch == '_') {
                        key->increase_Stringsize();
                        ch = input[++curr_Index];
                    }

                    if (ch == '=') { // here shouled check the key is unique
                        if (!curr_Element->keyExist(*key)) {
                            state = ST_NAMESPACE_URI_START;
                        } else {
                            state = ST_ERROR;
                        }
                    } else {
                        debug(DEBUG_FLAG, "ERROR_ST_NAMESPACE_NAME", curr_Index);
                        state = ST_ERROR;
                    }

                }
                    break;

                case ST_NAMESPACE_URI_START:
                {
                    debug(DEBUG_FLAG, "ST_NAMESPACE_URI_START", curr_Index);
                    if (ch == '"') {
                        if (input[++curr_Index] != '"') {
                            --curr_Index;
                            String *ptr_String = new String();
                            uri = new_Object(ptr_String, String_Vector, String_Index);
                            uri->set_StringBegin(input + curr_Index);
                            uri->set_StringSize(1);
                            state = ST_NAMESPACE_URI_CONTENT;
                        }
                    } else {
                        debug(DEBUG_FLAG, "ERROR2_ST_NAMESPACE_URI_START", curr_Index);
                        state = ST_ERROR;
                    }

                }
                    break;

                case ST_NAMESPACE_URI_CONTENT:
                {
                    debug(DEBUG_FLAG, "ST_NAMESPACE_URI_CONTENT", curr_Index);
                    while (ch != '"' && isascii(ch)) {
                        ch = input[++curr_Index]; // should save the URL in here
                        uri->increase_Stringsize();
                    }

                    if (ch == '"') {
                        curr_Element->add_NamespaceMap(*key, *uri);
                        state = ST_NAMESPACE_URI_END;
                    } else {
                        debug(DEBUG_FLAG, "ERROR_ST_NAMESPACE_URI_CONTENT", curr_Index);
                        state = ST_ERROR;
                    }
                }
                    break;

                case ST_NAMESPACE_URI_END:
                {
                    debug(DEBUG_FLAG, "ST_NAMESPACE_URI_END", curr_Index);
                    while (isspace(ch)) {
                        ch = input[++curr_Index];
                    }

                    if (ch == 'x') {
                        --curr_Index;
                        state = ST_XMLNS_START;
                    } else if (ch == '>') {
                        push_Element2Stack(curr_Element);
                        curr_Element = NULL;
                        state = ST_ELEMENT_END;
                    } else {
                        debug(DEBUG_FLAG, "ERROR_ST_NAMESPACE_URI_END", curr_Index);
                        state = ST_ERROR;
                    }
                }
                    break;

                case ST_COMMENT_END:
                {
                    debug(DEBUG_FLAG, "ST_COMMENT_END", curr_Index);
                    int index = 0;
                    while (ch != '-') {
                        ch = input[++curr_Index];
                    }

                    while (index < 2) // check the "--"
                    {
                        if (commnet[index] == input[curr_Index++]) {
                            ch = input[curr_Index];
                            index++;
                        } else {
                            debug(DEBUG_FLAG, "ERROR1_IN_ST_COMMENT_END", curr_Index);
                            state = ST_ERROR;
                            break;
                        }
                    }

                    if (ch == '>') {
                        state = ST_ELEMENT_END;
                    } else {
                        debug(DEBUG_FLAG, "ERROR2_IN_ST_COMMENT_END", curr_Index);
                        state = ST_ERROR;
                    }
                }
                    break;


                case ST_TEXT:
                {
                    debug(DEBUG_FLAG, "ST_TEXT", curr_Index);
                    while (ch != '<') {
                        ch = input[++curr_Index];
                        curr_String->increase_Stringsize();
                    }

                    if (ch == '<') {
                        Text *ptr_Text = new Text();
                        curr_Text = new_Object(ptr_Text, Text_Vector, Text_Index);
                        curr_Text -> add_Text(curr_String);
                        curr_Element = element_Stack.top();
                        curr_Element->add_Child(curr_Text);
                        curr_String = NULL;
                        curr_Element = NULL;
                        state = ST_ELEMENT_START;
                    }
                }
                    break;

                case ST_ELEMENT_END:
                {
                    debug(DEBUG_FLAG, "ST_ELEMENT_END", curr_Index);
                    if (ch == '<') {
                        state = ST_ELEMENT_START;
                    } else {
                        String* ptr_String = new String();
                        curr_String = new_Object(ptr_String, String_Vector, String_Index);
                        curr_String->set_StringBegin(input + curr_Index);
                        curr_String->set_StringSize(1);
                        state = ST_TEXT;
                    }

                }
                    break;

                case ST_ERROR:
                {
                    if (!is_Root) {
                        delete root;
                    }
                    cout << "Error in XML input, exiting..." << endl;
                    exit(1);
                }
                    break;

                default:
                    break;
            }

            curr_Index++;
        }

        cout << "Error in XML input, exiting..." << endl;
        exit(1);
    }

}
