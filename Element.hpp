/*
 * Element.hpp
 *
 *  Created on: Oct 30, 2013
 *      Author: xhan5
 */

#ifndef XHAN5_ELEMENT_HPP_
#define XHAN5_ELEMENT_HPP_

#include "String.hpp"
#include "Node.hpp"
#include <vector>
#include <map>

namespace xml {
    using namespace std;
    typedef map<string, String> str_Str_map;
    class Visitor;

    class Element : public Node {
    public:
        // Returns the name of the element, not including the prefix.
        const String &name() const;

        // Returns the full namespace URI (not the prefix) of the element.
        const String &nmspace() const;

        // Returns the number of children nodes.
        size_t n_children() const;

        // Returns the i'th child.
        const Node *child(size_t i) const;

        // Accepts a visitor.
        void accept(Visitor *) const;

        // Convenience functions that internally will use RTTI.
        static bool is_Element(const Node *);
        static const Element *to_Element(const Node *);



        void Init();
        void add_Child(Node* chile_node);
        void add_NamespaceMap(const String &, const String &);

        // set element name
        void set_ElementNameBegin(const char *);
        void set_ElementNameEnd(const char *);
        void set_ElementNameContent(String*);

        // set namespace(prefix)
        void set_NamespaceBegin(const char *);
        void set_NamespaceEnd(const char *);
        // set the Prefix
        void set_Namespace_Prefix(const String&);

        //get the namespace prefix
        String get_ElementPrefix();

        // get element name
        String* get_ElementContent();

        // use map to search the namespace URI
        String get_NamespaceURI(const string &);
        // return the curr node namespace map
        str_Str_map get_Curr_Map();
        // return the all mapvec in this node
        vector<str_Str_map*> get_Nmspace_MapVec();


        void set_MultiMapVec(const vector<str_Str_map*> &);
        void set_SingleMapVec();
        bool keyExist(const String &);


    private:
        String Element_Name;
        String Element_Nmspace_Prefix;
        String *Element_Name_Content; // Prefix + Element_Name
        String Element_Nmspace_URI;
        String Empty_Nmspace;

        //data struct
        str_Str_map Element_Nmspace_Map;
        vector<str_Str_map*> Element_Nmspace_MapVec;
        Element* Parent;
        vector<Node*> Child_Vec;
    };

}


#endif /* ELEMENT_HPP_ */
