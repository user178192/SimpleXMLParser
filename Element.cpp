/*
 * Element.cpp
 *
 *  Created on: Oct 30, 2013
 *      Author: xhan5
 */

#include "Node.hpp"
#include "Element.hpp"
#include "Text.hpp"
#include "Visitor.hpp"

namespace xml {

    const String & Element::name() const {
        return Element_Name;
    }

    const String & Element::nmspace() const {
        return Element_Nmspace_URI;
    }

    size_t Element::n_children() const {
        return Child_Vec.size();
    }

    const Node* Element::child(size_t i) const {
        return Child_Vec[i];
    }

    bool Element::is_Element(const Node *node) {
        return ( Element::to_Element(node) != NULL);
    }

    const Element* Element::to_Element(const Node *node) {
        const Element *element = dynamic_cast<const Element *> (node);
        return element ? element : NULL;
    }

    void Element::accept(Visitor *v) const {
        v -> start_element_visit(*this);
        for (size_t i = 0; i < Child_Vec.size(); i++) {
            if (Element::is_Element(Child_Vec[i])) {
                Element::to_Element(Child_Vec[i]) -> accept(v);
            } else {
                const Text *text = Text::to_Text(Child_Vec[i]); // if it is text
                v -> visit_text(*text); // visit text
            }
        }
        v -> end_element_visit(*this);
    }

    void Element::Init() {
        Child_Vec.clear();
        Element_Nmspace_Map.clear();
        Element_Nmspace_MapVec.clear();
    }

    void Element::add_Child(Node* Chile_Node) {
        Child_Vec.push_back(Chile_Node);
    }

    void Element::add_NamespaceMap(const String &key, const String &value) {
        const string str_temp = key;
        pair<const string, const String> content = make_pair(str_temp, value);
        Element_Nmspace_Map.insert(content);
    }

    void Element::set_ElementNameBegin(const char * begin) {
        Element_Name.set_StringBegin(begin);
    }

    void Element::set_ElementNameEnd(const char * end) {
        Element_Name.set_StringEnd(end);
    }

    void Element::set_ElementNameContent(String *content) {
        Element_Name_Content = content;
    }

    void Element::set_NamespaceBegin(const char *begin) {
        Element_Nmspace_Prefix.set_StringBegin(begin);
    }

    void Element::set_NamespaceEnd(const char *end) {
        Element_Nmspace_Prefix.set_StringEnd(end);
    }

    void Element::set_Namespace_Prefix(const String &prefix) {
        Element_Nmspace_Prefix = prefix;
    }

    // get element name

    String* Element::get_ElementContent() {
        return Element_Name_Content;
    }

    String Element::get_NamespaceURI(const string & key) {
        str_Str_map::iterator iter;
        if (key.size() > 0) {
            for (int i = Element_Nmspace_MapVec.size() - 1; i >= 0; i--) {
                iter = Element_Nmspace_MapVec[i]->find(key);
                if (Element_Nmspace_MapVec[i] != NULL && iter != Element_Nmspace_MapVec[i]->end()) {
                    return iter->second;
                } else {
                    continue;
                }
            }
        }
        return Empty_Nmspace;
    }

    str_Str_map Element::get_Curr_Map() {
        return Element_Nmspace_Map;
    }

    void Element::set_MultiMapVec(const vector<str_Str_map*> &nmspace_Vec) {
        Element_Nmspace_MapVec = nmspace_Vec;
        if (Element_Nmspace_Map.size() > 0) {
            Element_Nmspace_MapVec.push_back(&Element_Nmspace_Map);
        }
        Element_Nmspace_URI = get_NamespaceURI(Element_Nmspace_Prefix);
    }

    void Element::set_SingleMapVec() {
        if (Element_Nmspace_Map.size() > 0) {
            Element_Nmspace_MapVec.push_back(&Element_Nmspace_Map);
        }
    }

    bool Element::keyExist(const String &key) {
        const string str_temp = key;
        return (bool)(Element_Nmspace_Map.find(str_temp) != Element_Nmspace_Map.end());
    }

    vector<str_Str_map*> Element::get_Nmspace_MapVec() {
        return Element_Nmspace_MapVec;
    }

    String Element::get_ElementPrefix() {
        return Element_Nmspace_Prefix;
    }

}


