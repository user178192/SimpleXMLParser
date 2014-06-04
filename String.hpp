/*
 * String.hpp
 *
 *  Created on: Oct 30, 2013
 *      Author: xhan5
 */

#ifndef XHAN5_STRING_HPP_
#define XHAN5_STRING_HPP_

#include "Node.hpp"
#include <iostream>
#include <string.h>
#include <stdlib.h>

namespace xml {
    
    class String {
    public:
        String ();
        // Copy constructor.
        String(const String &);
        // Conversion to std::string.
        operator std::string() const;
        
        // Assignment.
        String &operator=(const String &);
        bool operator< (const String &str);
        bool operator== (const String &str);
        
        const char* get_StringContent () const;
        const char* get_StringBegin () const;
        const char* get_StringEnd () const;
        size_t get_StringSize () const;
        
        void Init();
        
		void set_StringBegin(const char *);
		void set_StringEnd(const char *);
		void set_StringSize(size_t);
		void increase_Stringsize();
        
        
        
    private:
		size_t Str_size;
		const char *Str_content;
        
    };
    
    //Comparision with std::string
    bool operator== (const std::string &, const String &);
    bool operator== (const String &, const std::string &);
    bool operator!= (const std::string &, const String &);
    bool operator!= (const String &, const std::string &);
    
    
    //Comparision with C string
    bool operator== (const char *, const String &);
    bool operator== (const String &, const char *);
    bool operator!= (const char *, const String &);
    bool operator!= (const String &, const char *);
    
    //Output operator
    std::ostream &operator<< (std::ostream &, const String &);
    
}


#endif /* STRING_HPP_ */
