PROJECT = prog2
SRC_FILES = 	\
	parser_test.cpp 	\
	Node.hpp		\
	Visitor.hpp		\
	Element.cpp		\
	Element.hpp		\
	Parser.cpp		\
	Parser.hpp		\
	String.cpp		\
	String.hpp		\
	Text.cpp		\
	Text.hpp		\
	XmlGenerator.cpp	\
	XmlGenerator.hpp	\
	gen.cpp			\
	Makefile		\
	README

.PHONY: all
all: parser_test gen

.PHONY: gen
gen:
	g++ -O -Wall -Wno-unused -o gen XmlGenerator.cpp gen.cpp

.PHONY: parser_test
parser_test:
	g++ -g -Wall -Wno-unused -o parser_test parser_test.cpp Element.cpp Parser.cpp String.cpp Text.cpp

submit:	$(SRC_FILES)
	tar zcvf $(PROJECT).tar.gz $(SRC_FILES)

clean:
	rm -rf gen parser_test parser_test.dSYM *.o *.tar.gz
