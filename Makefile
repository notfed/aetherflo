all: floc

floc.tab.c floc.tab.hpp: floc.ypp
	bison -d floc.ypp

lex.yy.c: floc.lpp floc.tab.hpp
	flex -d floc.lpp

floc: lex.yy.c floc.tab.c floc.tab.hpp interpreter.cpp interpreter.h symbols.cpp symbols.h
	g++ -g -std=c++11 -o floc floc.tab.cpp lex.yy.c interpreter.cpp symbols.cpp

clean:
	rm -f floc floc.tab.cpp lex.yy.c floc.tab.hpp
