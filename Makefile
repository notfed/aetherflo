all: floc

floc.tab.c floc.tab.hpp:	floc.ypp
	bison -d floc.ypp

lex.yy.c: floc.lpp floc.tab.hpp
	flex -d floc.lpp

floc: lex.yy.c floc.tab.c floc.tab.hpp interpreter.cpp interpreter.h aatree.h aatree.c
	g++ -o floc floc.tab.cpp lex.yy.c interpreter.cpp aatree.c

clean:
	rm -f floc floc.tab.cpp lex.yy.c floc.tab.hpp
