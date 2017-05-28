all: floc

floc.tab.c floc.tab.h:	floc.y
	bison -d floc.y

lex.yy.c: floc.l floc.tab.h
	flex -d floc.l

floc: lex.yy.c floc.tab.c floc.tab.h interpreter.cpp interpreter.h aatree.h aatree.c
	g++ -o floc floc.tab.c lex.yy.c interpreter.cpp aatree.c

clean:
	rm floc floc.tab.c lex.yy.c floc.tab.h
