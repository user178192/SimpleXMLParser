/*
 * Text.hpp
 *
 *  Created on: Oct 30, 2013
 *      Author: xhan5
 */

#ifndef XHAN5_TEXT_HPP_
#define XHAN5_TEXT_HPP_


namespace xml{
	using namespace std;
	class Text : public Node
    {
	public:
	    // Returns the text content as a string.
	    const String &str() const;
	    // Convenience functions that internally will use RTTI.
	    static bool is_Text(const Node *);
	    static const Text *to_Text(const Node *);

	    void add_Text(String *);
	    void Init();
	private:
		String *text;
	};
}


#endif /* TEXT_HPP_ */
