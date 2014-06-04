/*
 * Text.cpp
 *
 *  Created on: Oct 30, 2013
 *      Author: xhan5
 */


#include "String.hpp"
#include "Text.hpp"

namespace xml {

    const String& Text::str() const {
        return *text;
    }

    bool Text::is_Text(const Node *node) {
        return (Text::to_Text(node) != NULL);
    }

    const Text* Text::to_Text(const Node *node) {
        const Text *new_Text = dynamic_cast<const Text *> (node);
        return new_Text ? new_Text : NULL;
    }

    void Text::add_Text(String *new_Text) {
        text = new_Text;
    }

    void Text::Init() {
        text = NULL;
    }
}

